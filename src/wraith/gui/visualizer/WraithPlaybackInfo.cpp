/*
 * Copyright 2011 Thomas Mullaly
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
#include "WraithPlaybackInfo.hpp"

#include <stdexcept>

namespace wraith { namespace gui { namespace visualizer {

WraithPlaybackInfo::WraithPlaybackInfo()
    : IPlaybackInfo()
    , m_playbackEngine(0)
{ }

WraithPlaybackInfo::~WraithPlaybackInfo()
{
    using namespace wraith::playback;

    IVisualizationSource *source = dynamic_cast<IVisualizationSource*>(m_playbackEngine);
    if(source)
        source->unregisterVisualizationDataCallback(this);
}

void WraithPlaybackInfo::initialize(playback::PlaybackEngine *playbackEngine) {
    using namespace wraith::playback;

    IVisualizationSource *visSource = dynamic_cast<IVisualizationSource*>(playbackEngine);
    if(!visSource)
        throw std::invalid_argument("PlaybackEngine is not a VisualizationSource");

    visSource->registerVisualizationDataCallback(this);

    m_playbackEngine = playbackEngine;
}

WraithPlaybackInfo::VisualizationDataPtr
WraithPlaybackInfo::visualizationData() {
    return VisualizationDataPtr(new visualization::VisualizationData(0,0,0,0,0));
}

boost::int64_t WraithPlaybackInfo::currentTrackPosition() {
    return -1;
}

void WraithPlaybackInfo::update() {
    // Do something.
}

void WraithPlaybackInfo::onVisualizationData(VisualizationDataPtr data) {
}

} } } // namespace wraith::gui::visualizer
