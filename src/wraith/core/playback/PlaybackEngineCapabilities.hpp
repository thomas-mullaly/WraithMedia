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
#ifndef WRAITH_PLAYBACK_PLAYBACK_ENGINE_CAPABILITIES_HPP
#define WRAITH_PLAYBACK_PLAYBACK_ENGINE_CAPABILITIES_HPP

#include <list>
#include <string>

namespace wraith { namespace playback {

/**
 * Represents what type of media files a PlaybackEngine supports. For example
 * whether a PlaybackEngine supports videos, and if it does, what types of video
 * formats it supports.
 */
class PlaybackEngineCapabilities {
public:
    typedef std::list<std::string> SupportedExtensions;

    /** Constructors and Destructor. */
    //@{
    PlaybackEngineCapabilities();
    PlaybackEngineCapabilities(bool supports_audio,
                               SupportedExtensions const& audio,
                               bool supports_video,
                               SupportedExtensions const& video);
    virtual ~PlaybackEngineCapabilities();
    //@}

    /** Audio support functions. */
    //@{
    virtual bool                supportsAudio() const;
    virtual SupportedExtensions supportedAudioExtensions() const;
    //@}

    /** Video support functions. */
    //@{
    virtual bool                supportsVideo() const;
    virtual SupportedExtensions supportedVideoExtensions() const;
    //@}

private:
    SupportedExtensions m_audio, m_video;
    bool m_supports_audio, m_supports_video;
};

} } // namespace wraith::playback
#endif
