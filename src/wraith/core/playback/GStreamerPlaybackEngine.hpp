/*
 * Copyright 2010,2011 Thomas Mullaly
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
#ifndef WRAITH_PLAYBACK_GSTREAMER_PLAYBACK_ENGINE_HPP
#define WRAITH_PLAYBACK_GSTREAMER_PLAYBACK_ENGINE_HPP

#include <gst/gst.h>
#include <gst/base/gstadapter.h>

extern "C" {
#include <gst/fft/gstfftf32.h>
}

#include <boost/unordered_set.hpp>
#include <boost/thread/mutex.hpp>

#include <wraith/core/playback/PlaybackEngine.hpp>
#include <wraith/core/playback/IEqualizer.hpp>
#include <wraith/core/playback/IVisualizationSource.hpp>

namespace wraith { namespace playback {

/**
 * TODO: Move this to its own library at some point.
 */
class GStreamerPlaybackEngine : public PlaybackEngine, public IEqualizer, public IVisualizationSource {
private:
    typedef boost::mutex Mutex;
    typedef boost::unordered_set<IPlaybackEventsCallback*> PlaybackCallbacksSet;
    typedef boost::unordered_set<IVisualizationDataCallback*> VisualizationCallbacksSet;

public:
    typedef PlaybackEngine::PlaybackEngineID PlaybackEngineID;

    static const PlaybackEngineID playbackEngineID;

    GStreamerPlaybackEngine();
    virtual ~GStreamerPlaybackEngine();

    virtual PlaybackEngineID            engineID() const;
    virtual std::string                 engineName() const;
    virtual PlaybackEngineCapabilities  engineCapabilities() const;

    /**
     * Returns the current state of the engine.
     */
    virtual PlaybackState state() const;

    /**
     * Attempts to open the given media file with the given path.
     */
    virtual void openMedia(std::string const& path);

    /** @name Playback manipulation. */
    //@{
    /**
     * Resumes/starts playback.
     */
    virtual void play();

    /**
     * Pauses playback so it can be resumed from the same position
     * at a later time.
     */
    virtual void pause();

    /**
     * Toggles the state of the player. If the player is currently
     * in a playing state then this function will pause playback
     * and if the player is paused or stopped this will start/resume
     * playback.
     *
     * This function does nothing if the player is not in one of the states
     * described above.
     */
    virtual void togglePlayPause();

    /**
     * Stops the playing the current media file.
     */
    virtual void stop();
    //@}

    /** @name Media time information. */
    //@{
    /**
     * Returns the duration (in milliseconds) of the currently loaded media file
     * or 0 if no media file is loaded.
     */
    virtual boost::int64_t duration();

    /**
     * Returns the current position (in milliseconds) of the currently loaded
     * media file, or 0 if no media file is loaded.
     */
    virtual boost::int64_t position();
    //@}

    /** @name Seek controls. */
    //@{
    /**
     * Returns whether the current media file supports seek operations.
     */
    virtual bool canSeek();

    /**
     * Seeks to the specified time position in the media file. If the media
     * file does not support seeking this function does nothing.
     */
    virtual void setPosition(boost::int64_t pos);
    //@}

    /** @name Volume Control. */
    //@{
    /**
     * Returns the current volume of the PlaybackEngine.
     * This function will return a value in the range of [0.0, 100.0].
     */
    virtual double volume() const;

    /**
     * Sets the volume of the PlaybackEngine. The acceptable volume range
     * is 0.0 >= vol <= 100.0. Any values outside this range will be clamped
     * to the nearest acceptable value.
     */
    virtual void setVolume(double vol);

    virtual void toggleMute();
    virtual bool isMuted() const;
    //@}

    virtual void registerEventsCallback(IPlaybackEventsCallback *callback);
    virtual void unregisterEventsCallback(IPlaybackEventsCallback *callback);

    /** @name Equalizer functions. */
    //@{
    /** Returns whether the equalizer is supported. */
    virtual bool hasEqualizer() const;

    /**
     * Returns the number of bands that the equalizer has.
     */
    virtual size_t numberOfBands();

    /**
     * Gets/Sets the amplifier level of the equalizer.
     */
    virtual double  amplifierLevel();
    virtual void    setAmplifierLevel(double level);

    /**
     * Returns the number of bands that the equalizer has.
     */
    virtual std::pair<int,int>    bandRanges();

    /**
     * Returns the frequency of each equalizer bands.
     */
    virtual std::vector<double> equalizerFrequencies();

    /**
     * Gets/Sets the gain of a given equalizer band.
     */
    virtual void    setBandGain(size_t band, double gain);
    virtual double  bandGain(size_t band);
    //@}

    /** @name Visualization functions. */
    //@{
    virtual bool    isVisualizationEnabled() const;
    virtual void    setVisualizationEnabled(bool enable);
    virtual bool    isVisualizationSupported() const;
    virtual int     visualizationDataThreshold();
    virtual void    registerVisualizationDataCallback(IVisualizationDataCallback *callback);
    virtual void    unregisterVisualizationDataCallback(IVisualizationDataCallback *callback);
    //@}

private:
    static gboolean gst_bus_callback(GstBus *bus, GstMessage *msg, gpointer data);
    static gboolean gst_vis_pipeline_event_probe(GstPad *pad, GstEvent *event, gpointer userdata);
    static void     gst_vis_pcm_handoff(GstElement *sink, GstBuffer *buffer, GstPad *pad, gpointer userdata);
    static void     gst_vis_pipeline_blocked(GstPad *pad, gboolean blocked, gpointer userdata);
    static const int SAMPLES_PER_SECOND;

    void handleBusMessage(GstMessage *msg);
    void handleVisPipelineEventProbe(GstPad *pad, GstEvent *event);
    void handleVisPCMHandoff(GstElement *sink, GstBuffer *buffer, GstPad *pad);
    void handleVisPipelineBlocked(GstPad *pad, gboolean blocked);

    /**
     * Used to setup the GStreamer playback pipeline.
     */
    void setupPipeline();
    bool setupEqualizer(GstElement *queue, GstElement *sink);
    void setupVisualization();
    void blockVisPipeline(bool block);

    /**
     * Destroys the pipeline that was created for playback.
     */
    void destroyPipeline();

    /**
     * Called by handleBusMessage when an EOS signal is recieved.
     */
    void onPlaybackComplete();

    /**
     * Called by handleBusMessage when a StateChanged signal is recieved.
     */
    void onStateChanged(GstMessage *msg);

    void onVisualizationData(std::size_t channels, std::size_t samples, float *pcm,
                             std::size_t bands, float *spectrum);

    /**
     * Sets the state of the player.
     *
     * @note This does not perform any checks to ensure that
     * the PlaybackEngine will *actually* be in this state after this
     * function returns.
     */
    void setState(PlaybackState state);

    void verifyEqualizer();

private:
    PlaybackState m_current_state;

    bool m_valid;
    bool m_supports_custom_pipelines;
    bool m_muted;

    GstElement *m_playbin;
    GstElement *m_audiobin;
    GstElement *m_audio_tee;
    GstElement *m_equalizer;
    GstElement *m_preamp;
    GstElement *m_spectrum;

    // Used to restore the volume of the PlaybackEngine
    // when it is unmuted.
    double m_restore_volume;

    bool    m_equalizer_enabled;
    bool    m_visualizer_enabled;
    size_t  m_num_of_bands;
    int     m_vis_threshold;

    GstElement *m_vis_resampler;
    GstAdapter *m_vis_buffer;
    gboolean m_vis_enabled;
    gboolean m_vis_thawing;
    GstFFTF32 *m_vis_fft;
    GstFFTF32Complex *m_vis_fft_buffer;
    gfloat *m_vis_fft_sample_buffer;

    Mutex m_stateMutex;
    Mutex m_playbackCallbacksMutex;
    Mutex m_visualizationCallbacksMutex;

    PlaybackCallbacksSet m_playbackCallbacks;
    VisualizationCallbacksSet m_visualizationCallbacks;
};

} } // namespace wraith::playback

#endif
