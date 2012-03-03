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
#include "PlaybackEngineCapabilities.hpp"

using namespace wraith::playback;

PlaybackEngineCapabilities::PlaybackEngineCapabilities()
    : m_audio()
    , m_video()
    , m_supports_audio(false)
    , m_supports_video(false)
{ }

PlaybackEngineCapabilities::PlaybackEngineCapabilities(bool supports_audio,
                                                       SupportedExtensions const& audio,
                                                       bool supports_video,
                                                       SupportedExtensions const& video)
    : m_audio(audio)
    , m_video(video)
    , m_supports_audio(supports_audio)
    , m_supports_video(supports_video)
{ }

PlaybackEngineCapabilities::~PlaybackEngineCapabilities()
{ }

bool PlaybackEngineCapabilities::supportsAudio() const {
    return m_supports_audio;
}

PlaybackEngineCapabilities::SupportedExtensions
PlaybackEngineCapabilities::supportedAudioExtensions() const {
    return m_audio;
}

bool PlaybackEngineCapabilities::supportsVideo() const {
    return m_supports_video;
}

PlaybackEngineCapabilities::SupportedExtensions
PlaybackEngineCapabilities::supportedVideoExtensions() const {
    return m_video;
}
