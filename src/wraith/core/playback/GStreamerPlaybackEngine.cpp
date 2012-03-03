/*
 * Copyright 2010 Thomas Mullaly
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
#include <math.h>

#include <stdexcept>

#include <QDebug>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "GStreamerPlaybackEngine.hpp"

#define SLICE_SIZE 735

namespace wraith { namespace playback {

static GstStaticCaps vis_data_sink_caps = GST_STATIC_CAPS (
    "audio/x-raw-float, "
    "rate = (int) 44100, "
    "channels = (int) 2, "
    "endianness = (int) BYTE_ORDER, "
    "width = (int) 32"
);

static void init_gst() {
    static bool init = false;
    if(!init) {
        gst_init(0, 0);
        g_set_application_name("wraithmedia");
        init = true;
    }
}

const int GStreamerPlaybackEngine::SAMPLES_PER_SECOND = 45;

const GStreamerPlaybackEngine::PlaybackEngineID GStreamerPlaybackEngine::playbackEngineID(
    boost::lexical_cast<PlaybackEngine::PlaybackEngineID>("6856c655-0360-4803-8577-97651fe89b09")
);

GStreamerPlaybackEngine::GStreamerPlaybackEngine()
    : PlaybackEngine()
    , IEqualizer()
    , IVisualizationSource()
    , m_current_state(PLAYBACK_STATE_UNKNOWN)
    , m_valid(false)
    , m_supports_custom_pipelines(false)
    , m_muted(false)
    , m_playbin()
    , m_audiobin()
    , m_audio_tee()
    , m_equalizer()
    , m_preamp()
    , m_spectrum()
    , m_restore_volume(0.0)
    , m_equalizer_enabled(false)
    , m_visualizer_enabled(false)
    , m_num_of_bands()
    , m_vis_threshold(-60)
    , m_vis_resampler(0)
    , m_vis_buffer(0)
    , m_vis_enabled(false)
    , m_vis_thawing(false)
    , m_vis_fft(0)
    , m_vis_fft_buffer(0)
    , m_vis_fft_sample_buffer(0)
    , m_stateMutex()
    , m_playbackCallbacksMutex()
    , m_visualizationCallbacksMutex()
    , m_playbackCallbacks()
    , m_visualizationCallbacks()
{
    init_gst();
    setupPipeline();
}

GStreamerPlaybackEngine::~GStreamerPlaybackEngine() 
{
    destroyPipeline();
    m_playbackCallbacks.clear();
    m_visualizationCallbacks.clear();
}

GStreamerPlaybackEngine::PlaybackEngineID
GStreamerPlaybackEngine::engineID() const {
   return playbackEngineID;
}

std::string GStreamerPlaybackEngine::engineName() const {
    return std::string("GStreamer PlaybackEngine");
}

PlaybackEngineCapabilities
GStreamerPlaybackEngine::engineCapabilities() const {
    // TODO: Implement
    return PlaybackEngineCapabilities();
}

PlaybackState GStreamerPlaybackEngine::state() const {
    return m_current_state;
}

// TODO Add checking to make sure the path is valid.
void GStreamerPlaybackEngine::openMedia(std::string const& path) {
    // Sanity checks.
    if(path.empty()) {
        qDebug() << "[GStreamerPlaybackEngine] Warning: Empty or null path was passed to openMedia(path).";
        return;
    }

    if(m_current_state == PLAYBACK_STATE_UNKNOWN) {
        qDebug() << "[GStreamerPlaybackEngine] Warning: Attempted to open a media file while in an unknown state.";
        return;
    }

    qDebug() << "Opening";
    
    // Prep the playbin to play.
    gst_element_set_state(m_playbin, GST_STATE_READY);
    g_object_set(G_OBJECT(m_playbin), "uri", path.c_str(), NULL);

    // Change the state to ready.
    setState(PLAYBACK_STATE_READY);
}

void GStreamerPlaybackEngine::play() {
    // We only start playing during certain conditions.
    switch(m_current_state) {
    case PLAYBACK_STATE_STOPPED:
    case PLAYBACK_STATE_PAUSED:
    case PLAYBACK_STATE_READY:
        gst_element_set_state(m_playbin, GST_STATE_PLAYING);
        break;
    default:
        break;
    }
}

void GStreamerPlaybackEngine::pause() {
    if(m_current_state == PLAYBACK_STATE_PLAYING) {
        gst_element_set_state(m_playbin, GST_STATE_PAUSED);
    }
}

void GStreamerPlaybackEngine::togglePlayPause() {
    switch(m_current_state) {
    case PLAYBACK_STATE_PLAYING:
        pause();
        break;
    case PLAYBACK_STATE_STOPPED:
    case PLAYBACK_STATE_PAUSED:
    case PLAYBACK_STATE_READY:
        play();
        break;
    default:
        break;
    }
}

void GStreamerPlaybackEngine::stop() {
    if(m_current_state == PLAYBACK_STATE_PLAYING || m_current_state == PLAYBACK_STATE_PAUSED) {
        gst_element_set_state(m_playbin, GST_STATE_NULL);
    }
}

// TODO Figure out a way to store the duration so we
// don't have to keep checking.
boost::int64_t GStreamerPlaybackEngine::duration() {
    if(m_current_state == PLAYBACK_STATE_UNKNOWN ||
       m_current_state == PLAYBACK_STATE_IDLE ||
       m_current_state == PLAYBACK_STATE_ERROR)
    {
        return 0;
    }

    // Query for the duration of the file.
    GstFormat format = GST_FORMAT_TIME;
    gint64 dur = 0;
    if(gst_element_query_duration(m_playbin, &format, &dur)) {
        return (dur / GST_MSECOND);
    }

    return 0;
}

boost::int64_t GStreamerPlaybackEngine::position() {
    if(m_current_state == PLAYBACK_STATE_UNKNOWN ||
       m_current_state == PLAYBACK_STATE_IDLE ||
       m_current_state == PLAYBACK_STATE_ERROR)
    {
        return 0;
    }

    // Attempt to retreive the current position.
    gint64 pos = 0;
    GstFormat format = GST_FORMAT_TIME;
    if(gst_element_query_position(m_playbin, &format, &pos)) {
        // Convert into milliseconds.
        return (pos / GST_MSECOND);
    }

    return 0;
}

void GStreamerPlaybackEngine::setPosition(boost::int64_t time_ms) {
    if(canSeek()) {
        if(!gst_element_seek(m_playbin, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, time_ms * GST_MSECOND, GST_SEEK_TYPE_NONE,
                         GST_CLOCK_TIME_NONE)) {
            qDebug() << "Failed";
        }
    }
}

bool GStreamerPlaybackEngine::canSeek() {
    if(m_current_state == PLAYBACK_STATE_UNKNOWN ||
       m_current_state == PLAYBACK_STATE_IDLE ||
       m_current_state == PLAYBACK_STATE_ERROR)
    { return false; }

    GstQuery *query;
    gboolean seekable = false;

    // Used to query if the media supports seeking.
    query = gst_query_new_seeking(GST_FORMAT_TIME);
    if(!gst_element_query(m_playbin, query)) {
        return duration() > 0l;
    }

    // Retrieve the results from the query.
    gst_query_parse_seeking(query, NULL, &seekable, NULL, NULL);
    gst_query_unref(query);

    return seekable;
}

double GStreamerPlaybackEngine::volume() const {
    if(m_current_state == PLAYBACK_STATE_UNKNOWN) {
        return 0.0;
    }

    double vol = 0.0;
    g_object_get(G_OBJECT(m_playbin), "volume", &vol, NULL);

    // Normalize the volume before we return it.
    // We have to do this since the playbin stores its volume
    // in the range [0, 1]
    return vol * 100.0;
}

void GStreamerPlaybackEngine::setVolume(double vol) {
    if(m_current_state == PLAYBACK_STATE_UNKNOWN) {
        return;
    }

    // clamp.
    vol = (vol > 100.0) ? 100.0 : (vol < 0.0) ? 0.0 : vol;

    // normalize.
    vol /= 100.0;

    // set.
    g_object_set(G_OBJECT(m_playbin), "volume", vol, NULL);
}

bool GStreamerPlaybackEngine::isMuted() const {
    return m_muted;
}

void GStreamerPlaybackEngine::toggleMute() {
    if(m_current_state == PLAYBACK_STATE_UNKNOWN) {
        return;
    }

    if(m_muted) {
        g_object_set(G_OBJECT(m_playbin), "volume", m_restore_volume, NULL);
    } else {
        // Backup the current volume.
        g_object_get(G_OBJECT(m_playbin), "volume", &m_restore_volume, NULL);
        g_object_set(G_OBJECT(m_playbin), "volume", 0.0, NULL);
    }

    m_muted = !m_muted;
}

void GStreamerPlaybackEngine::registerEventsCallback(IPlaybackEventsCallback *callback) {
    if(!callback)
        return;

    m_playbackCallbacksMutex.lock();
    m_playbackCallbacks.insert(callback);
    m_playbackCallbacksMutex.unlock();
}

void GStreamerPlaybackEngine::unregisterEventsCallback(IPlaybackEventsCallback *callback) {
    if(!callback)
        return;

    m_playbackCallbacksMutex.lock();
    m_playbackCallbacks.erase(callback);
    m_playbackCallbacksMutex.unlock();
}

bool GStreamerPlaybackEngine::hasEqualizer() const {
    return m_equalizer != 0;
}

size_t GStreamerPlaybackEngine::numberOfBands() {
    verifyEqualizer();
    return static_cast<size_t>(gst_child_proxy_get_children_count(GST_CHILD_PROXY(m_equalizer)));
}

double GStreamerPlaybackEngine::amplifierLevel() {
    verifyEqualizer();
    double level;
    g_object_get(m_preamp, "volume", &level, NULL);
    return level;
}

void GStreamerPlaybackEngine::setAmplifierLevel(double value) {
    verifyEqualizer();

    value = pow(10.0, value / 20.0);
    g_object_set(m_preamp, "volume", value, NULL);
}

std::pair<int,int> GStreamerPlaybackEngine::bandRanges() {
    verifyEqualizer();

    GParamSpec *spec = g_object_class_find_property(G_OBJECT_GET_CLASS(m_equalizer), "band0::gain");
    if(!spec)
        spec = g_object_class_find_property(G_OBJECT_GET_CLASS(m_equalizer), "band0");

    if(spec && G_IS_PARAM_SPEC_DOUBLE(spec)) {
        GParamSpecDouble *dspec = (GParamSpecDouble*)spec;
        return std::make_pair(static_cast<int>(dspec->minimum), static_cast<int>(dspec->maximum));
    }

    return std::make_pair(0,0);
}

std::vector<double> GStreamerPlaybackEngine::equalizerFrequencies() {
    verifyEqualizer();
    std::vector<double> frequencies;

    int count = gst_child_proxy_get_children_count(GST_CHILD_PROXY(m_equalizer));
    for(int i = 0; i < count; ++i) {
        double freq;
        GstObject *band = gst_child_proxy_get_child_by_index(GST_CHILD_PROXY(m_equalizer), i);
        g_object_get(G_OBJECT(band), "freq", &freq, NULL);
        frequencies.push_back(freq);
        gst_object_unref(band);
    }

    return frequencies;
}

void GStreamerPlaybackEngine::setBandGain(size_t band, double gain) {
    verifyEqualizer();

    if(band > gst_child_proxy_get_children_count(GST_CHILD_PROXY(m_equalizer)))
        throw std::logic_error("Band number is out of range.");

    GstObject *band_obj = gst_child_proxy_get_child_by_index(GST_CHILD_PROXY(m_equalizer), band);
    g_object_set(G_OBJECT(band_obj), "gain", gain, NULL);
    gst_object_unref(band_obj);
}

double GStreamerPlaybackEngine::bandGain(size_t band) {
    verifyEqualizer();

    if(band > gst_child_proxy_get_children_count(GST_CHILD_PROXY(m_equalizer)))
        throw std::logic_error("Band number is out of range.");

    double gain;
    GstObject *band_obj = gst_child_proxy_get_child_by_index(GST_CHILD_PROXY(m_equalizer), band);
    g_object_get(G_OBJECT(band_obj), "gain", &gain, NULL);
    gst_object_unref(band_obj);
    return gain;
}

bool GStreamerPlaybackEngine::isVisualizationEnabled() const {
    return m_vis_enabled;
}

void GStreamerPlaybackEngine::setVisualizationEnabled(bool enable) {
    blockVisPipeline(!enable);
    m_vis_enabled = enable;
}

bool GStreamerPlaybackEngine::isVisualizationSupported() const {
    return m_vis_resampler != 0;
}

int GStreamerPlaybackEngine::visualizationDataThreshold() {
    return m_vis_threshold;
}

void GStreamerPlaybackEngine::registerVisualizationDataCallback(IVisualizationDataCallback *callback) {
    if(!callback)
        return;

    m_visualizationCallbacksMutex.lock();
    m_visualizationCallbacks.insert(callback);
    m_visualizationCallbacksMutex.unlock();
}

void GStreamerPlaybackEngine::unregisterVisualizationDataCallback(IVisualizationDataCallback *callback) {
    if(!callback)
        return;

    m_visualizationCallbacksMutex.lock();
    m_visualizationCallbacks.erase(callback);
    m_visualizationCallbacksMutex.unlock();
}

void GStreamerPlaybackEngine::handleBusMessage(GstMessage *msg) {
    switch(GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
        onPlaybackComplete();
        break;
    case GST_MESSAGE_STATE_CHANGED:
        onStateChanged(msg);
        break;
    default:
        break;
    }
}

void GStreamerPlaybackEngine::handleVisPipelineEventProbe(GstPad *pad, GstEvent *event) {
    switch (GST_EVENT_TYPE(event)) {
        case GST_EVENT_FLUSH_START:
        case GST_EVENT_FLUSH_STOP:
        case GST_EVENT_SEEK:
        case GST_EVENT_NEWSEGMENT:
        case GST_EVENT_CUSTOM_DOWNSTREAM:
            m_vis_thawing = TRUE;

        default: break;
    }

    if (m_vis_enabled)
        return;

    switch (GST_EVENT_TYPE(event)) {
    case GST_EVENT_EOS:
    case GST_EVENT_CUSTOM_DOWNSTREAM_OOB:
        blockVisPipeline(FALSE);
        break;
    case GST_EVENT_CUSTOM_DOWNSTREAM:
    case GST_EVENT_NEWSEGMENT:
        blockVisPipeline(TRUE);
        break;

    default: break;
    }
}

/* This function was taken from Banshee's 'banshee-player-vis.c' */
void GStreamerPlaybackEngine::handleVisPCMHandoff(GstElement *sink, GstBuffer *buffer, GstPad *pad) {
    GstStructure *structure;
    gint channels, wanted_size;
    gfloat *data;

    if(m_vis_thawing) {
        // Flush our buffers out.
        gst_adapter_clear(m_vis_buffer);
        memset(m_vis_fft_sample_buffer, 0, sizeof(gfloat) * SLICE_SIZE);

        m_vis_thawing = FALSE;
    }
    
    structure = gst_caps_get_structure(gst_buffer_get_caps(buffer), 0);
    gst_structure_get_int(structure, "channels", &channels);

    wanted_size = channels * SLICE_SIZE * sizeof (gfloat);

    gst_adapter_push(m_vis_buffer, gst_buffer_copy(buffer));

    while((data = (gfloat *)gst_adapter_peek(m_vis_buffer, wanted_size)) != NULL) {
        gfloat *deinterlaced = static_cast<gfloat*>(g_malloc(wanted_size));
        gfloat *specbuf = g_new(gfloat, SLICE_SIZE * 2);

        gint i, j;

        memcpy(specbuf, m_vis_fft_sample_buffer, SLICE_SIZE * sizeof(gfloat));

        for(i = 0; i < SLICE_SIZE; i++) {
            gfloat avg = 0.0f;

            for (j = 0; j < channels; j++) {
                gfloat sample = data[i * channels + j];

                deinterlaced[j * SLICE_SIZE + i] = sample;
                avg += sample;
            }

            avg /= channels;
            specbuf[i + SLICE_SIZE] = avg;
        }

        memcpy(m_vis_fft_sample_buffer, &specbuf[SLICE_SIZE], SLICE_SIZE * sizeof(gfloat));

        gst_fft_f32_window(m_vis_fft, specbuf, GST_FFT_WINDOW_HAMMING);
        gst_fft_f32_fft(m_vis_fft, specbuf, m_vis_fft_buffer);

        for (i = 0; i < SLICE_SIZE; i++) {
            gfloat val;

            GstFFTF32Complex cplx = m_vis_fft_buffer[i];

            val = cplx.r * cplx.r + cplx.i * cplx.i;
            val /= SLICE_SIZE * SLICE_SIZE;
            val = 10.0f * log10f(val);

            val = (val + 60.0f) / 60.0f;
            if (val < 0.0f)
                val = 0.0f;

            specbuf[i] = val;
        }

        onVisualizationData(channels, SLICE_SIZE, deinterlaced, SLICE_SIZE, specbuf);

        g_free(deinterlaced);
        g_free(specbuf);

        gst_adapter_flush(m_vis_buffer, wanted_size);
    }
}

void GStreamerPlaybackEngine::handleVisPipelineBlocked(GstPad *pad, gboolean blocked) {
    if(!blocked)
        m_vis_thawing = true;
}

gboolean GStreamerPlaybackEngine::gst_bus_callback(GstBus *bus, GstMessage *msg, gpointer data) {
    GStreamerPlaybackEngine *engine = static_cast<GStreamerPlaybackEngine*>(data);
    engine->handleBusMessage(msg);
    return true;
}

gboolean GStreamerPlaybackEngine::gst_vis_pipeline_event_probe(GstPad *pad, GstEvent *event, gpointer userdata)
{
    GStreamerPlaybackEngine *engine = static_cast<GStreamerPlaybackEngine*>(userdata);
    engine->handleVisPipelineEventProbe(pad, event);
    return true;
}

void GStreamerPlaybackEngine::gst_vis_pcm_handoff(GstElement *sink, GstBuffer *buffer, GstPad *pad, gpointer userdata)
{
    GStreamerPlaybackEngine *engine = static_cast<GStreamerPlaybackEngine*>(userdata);
    engine->handleVisPCMHandoff(sink, buffer, pad);
}

void GStreamerPlaybackEngine::gst_vis_pipeline_blocked(GstPad *pad, gboolean blocked, gpointer userdata) {
    GStreamerPlaybackEngine *engine = static_cast<GStreamerPlaybackEngine*>(userdata);
    engine->handleVisPipelineBlocked(pad, blocked);
}

void GStreamerPlaybackEngine::setupPipeline() {
    // This handles the playing of media files. This is also
    // the minimum requirement to support playback.
    m_playbin = gst_element_factory_make("playbin2", "playbin");
    if(!m_playbin) {
        qDebug() << "[GStreamerPlaybackEngine] Error: Failed to create the playbin.";
        return;
    }

    // We support basic playback now.
    m_current_state = PLAYBACK_STATE_IDLE;
    m_valid = true;

    // Attach a watch to the playbins bus so we recieve events.
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(m_playbin));
    gst_bus_add_watch(bus, gst_bus_callback, this);
    gst_object_unref(bus);

    // The audiobin will contain all of the audio components.
    m_audiobin = gst_bin_new("audiobin");
    if(!m_audiobin) {
        qDebug() << "[GStreamerPlaybackEngine] Warning: Unable to setup the custom audiobin.";
        return;
    }

    // Pipeline to the sound card:
    //  tee ! queue ! audiosink

    // Create the audiotee so other audio pipelines can 
    // connect to it.
    m_audio_tee = gst_element_factory_make("tee", "audio_tee");
    if(!m_audio_tee) {
        qDebug() << "[GStreamerPlaybackEngine] Warning: Failed to create the audio tee.";

        // No point in having the audiobin wasting space.
        gst_object_unref(GST_OBJECT(m_audiobin));
        m_audiobin = 0;
        return;
    }

    GstElement *queue = gst_element_factory_make("queue", "queue");
    if(!queue) {
        qDebug() << "[GStreamerPlaybackEngine] Warning: Failed to create the audio queue.";

        // No point in having the audiobin.
        gst_object_unref(GST_OBJECT(m_audiobin));
        m_audiobin = 0;
        return;
    }

    GstElement *audiosink = gst_element_factory_make("autoaudiosink", "audiosink");
    if(!audiosink) {
        qDebug() << "[GStreamerPlaybackEngine] Warning: Failed to create the audiosink.";

        gst_object_unref(GST_OBJECT(m_audiobin));
        m_audiobin = 0;
        return;
    }

    // Add our custom audio components to the audiobin.
    gst_bin_add_many(GST_BIN(m_audiobin), m_audio_tee, queue, audiosink, NULL);

    if(!setupEqualizer(queue, audiosink)) {
        // Now link the queue, and audiosink together.
        gst_element_link(queue, audiosink);
    }

    // Link the queue and audiotee together.
    gst_pad_link(gst_element_get_request_pad(m_audio_tee, "src0"),
                 gst_element_get_pad(queue, "sink"));

    // Now ghostpad the audiobin so audio data can be passed
    // into it.
    GstPad* teepad = gst_element_get_pad(m_audio_tee, "sink");
    gst_element_add_pad(m_audiobin, gst_ghost_pad_new("sink", teepad));
    gst_object_unref(teepad);

    // Now make our custom audiobin the "audio-sink" for the playbin.
    g_object_set(G_OBJECT(m_playbin), "audio-sink", m_audiobin, NULL);

    // We support custom audio pipelines now.
    m_supports_custom_pipelines = true;

    setupVisualization();
}

bool GStreamerPlaybackEngine::setupEqualizer(GstElement *queue, GstElement *sink) {
    m_equalizer = gst_element_factory_make("equalizer-10bands", "equalizer");
    if(!m_equalizer) return false;

    GstElement *convert1, *convert2;
    convert1 = gst_element_factory_make("audioconvert", "convert1");
    convert2 = gst_element_factory_make("audioconvert", "convert2");
    m_preamp = gst_element_factory_make("volume", "preamp");

    gst_bin_add_many(GST_BIN(m_audiobin), convert1, convert2, m_preamp, m_equalizer, NULL);

    gst_element_link_many(queue, convert1, m_preamp, m_equalizer, convert2, sink, NULL);

    qDebug() << "[GStreamerPlaybackEngine] Info: Equalizer setup.";
    return true;
}

/* The following code is mostly a copy from Banshee's "banshee-player-vis.c" file. */
void GStreamerPlaybackEngine::setupVisualization() {
    GstElement *fakesink, *converter, *resampler, *audiosinkqueue;
    GstCaps *caps;
    GstPad *teepad;
    GstPad *pad;
    gint wanted_size;

    m_vis_buffer = 0;
    m_vis_fft = gst_fft_f32_new(SLICE_SIZE * 2, false);
    m_vis_fft_buffer = g_new(GstFFTF32Complex, SLICE_SIZE+1);
    m_vis_fft_sample_buffer = g_new0(gfloat, SLICE_SIZE);

    audiosinkqueue = gst_element_factory_make("queue", "vis-queue");

    pad = gst_element_get_static_pad(audiosinkqueue, "sink");
    gst_pad_add_event_probe(pad, G_CALLBACK(gst_vis_pipeline_event_probe), this);
    gst_object_unref(GST_OBJECT(pad));

    resampler = gst_element_factory_make("audioresample", "vis-resample");
    converter = gst_element_factory_make("audioconvert", "vis-convert");
    fakesink = gst_element_factory_make("fakesink", "vis-sink");

    wanted_size = 2 * SLICE_SIZE * sizeof(gfloat);

    if (audiosinkqueue == NULL || resampler == NULL || converter == NULL || fakesink == NULL) {
        qDebug() << "[GStreamerPlaybackEngine]: Could not construct visualization pipeline, a fundamental element could not be created";
        return;
    }

    g_object_set(G_OBJECT(audiosinkqueue),
                "leaky", 2,
                "max-size-buffers", 0,
                "max-size-bytes", 0,
                "max-size-time", GST_SECOND * 5, NULL);

    g_signal_connect(G_OBJECT(fakesink), "handoff", G_CALLBACK(gst_vis_pcm_handoff), this);

    g_object_set(G_OBJECT(fakesink),
                "signal-handoffs", true,
                "sync", true,
                "max-lateness", GST_SECOND / 120,
                "ts-offset", -GST_SECOND / 60,
                "async", false, NULL);

    gst_bin_add_many(GST_BIN(m_audiobin), audiosinkqueue, resampler, converter, fakesink, NULL);

    pad = gst_element_get_static_pad(audiosinkqueue, "sink");
    teepad = gst_element_get_request_pad(m_audio_tee, "src%d");
    gst_pad_link(teepad, pad);
    gst_object_unref(GST_OBJECT(teepad));
    gst_object_unref(GST_OBJECT(pad));

    gst_element_link_many(audiosinkqueue, resampler, converter, NULL);

    caps = gst_static_caps_get(&vis_data_sink_caps);
    gst_element_link_filtered(converter, fakesink, caps);
    gst_caps_unref(caps);

    m_vis_buffer = gst_adapter_new();
    m_vis_resampler = resampler;

    blockVisPipeline(true);
}

void GStreamerPlaybackEngine::blockVisPipeline(bool block) {
    GstPad *queue_sink;

    if(!m_vis_resampler)
        return;

    queue_sink = gst_element_get_static_pad(m_vis_resampler, "src");
    gst_pad_set_blocked_async(queue_sink, block, gst_vis_pipeline_blocked, this);
    gst_object_unref(GST_OBJECT(queue_sink));
}

void GStreamerPlaybackEngine::destroyPipeline() {
    if(m_playbin) {
        // Stop whatever the playbin is doing.
        gst_element_set_state(m_playbin, GST_STATE_NULL);

        // Destory the playbin.
        gst_object_unref(m_playbin);
        m_playbin = 0;

        if(m_vis_buffer) {
            gst_object_unref(m_vis_buffer);
            m_vis_buffer = 0;
        }

        if(m_vis_fft) {
            gst_fft_f32_free(m_vis_fft);
            m_vis_fft = 0;
        }

        if(m_vis_fft_buffer) {
            g_free(m_vis_fft_buffer);
            m_vis_fft_buffer = 0;
        }

        if(m_vis_fft_sample_buffer) {
            g_free(m_vis_fft_sample_buffer);
            m_vis_fft_sample_buffer = 0;
        }

        m_vis_resampler = 0;
        m_vis_enabled = FALSE;
        m_vis_thawing = FALSE;
    }
}

void GStreamerPlaybackEngine::onPlaybackComplete() {
    setState(PLAYBACK_STATE_STOPPED);
}

void GStreamerPlaybackEngine::onStateChanged(GstMessage *msg) {
    // Parse the new states out of the message.
    GstState oldState, newState, pendingState;
    gst_message_parse_state_changed(msg, &oldState, &newState, &pendingState);

    // TODO Handle the other states.
    switch(newState) {
    case GST_STATE_PAUSED:
        setState(PLAYBACK_STATE_PAUSED);
        break;
    case GST_STATE_PLAYING:
        setState(PLAYBACK_STATE_PLAYING);
        break;
    case GST_STATE_NULL:
        setState(PLAYBACK_STATE_IDLE);
        break;
    case GST_STATE_READY:
        setState(PLAYBACK_STATE_READY);
        break;
    default:
        break;
    }
}

void GStreamerPlaybackEngine::onVisualizationData(std::size_t channels,std::size_t samples,
                                                  float *pcm, std::size_t bands, float *spectrum) {
    typedef IVisualizationDataCallback::VisualizationDataPtr VisualizationDataPtr;

    VisualizationDataPtr visData(new visualization::VisualizationData(channels, samples, pcm, bands, spectrum));

    typedef VisualizationCallbacksSet::iterator Iter;

    m_visualizationCallbacksMutex.lock();
    for(Iter it = m_visualizationCallbacks.begin(); it != m_visualizationCallbacks.end(); ++it)
        (*it)->onVisualizationData(visData);
    m_visualizationCallbacksMutex.unlock();
}

void GStreamerPlaybackEngine::setState(PlaybackState state) {
    m_stateMutex.lock();
    if(m_current_state == state) {
        m_stateMutex.unlock();
        return;
    }

    PlaybackState old = m_current_state;
    m_current_state = state;
    m_stateMutex.unlock();

    typedef PlaybackCallbacksSet::iterator Iter;

    m_playbackCallbacksMutex.lock();
    for(Iter it = m_playbackCallbacks.begin(); it != m_playbackCallbacks.end(); ++it)
        (*it)->onStateChanged(state, old);
    m_playbackCallbacksMutex.unlock();
}

void GStreamerPlaybackEngine::verifyEqualizer() {
    if(!hasEqualizer())
        throw std::logic_error("GStreamerPlaybackEngine doesn't support an equalizer.");
}

} } // namespace wraith::playback
