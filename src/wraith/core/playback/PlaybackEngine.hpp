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
#ifndef WRAITH_PLAYBACK_PLAYBACK_ENGINE_HPP
#define WRAITH_PLAYBACK_PLAYBACK_ENGINE_HPP

#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/cstdint.hpp>

#include <wraith/core/playback/PlaybackState.hpp>
#include <wraith/core/playback/IPlaybackEventsCallback.hpp>
#include <wraith/core/playback/PlaybackEngineCapabilities.hpp>

namespace wraith { namespace playback {

/**
 * Abstract base class from which all PlaybackEngine's inherit from.
 *
 * This class defines the core set of features that a PlaybackEngine implementation
 * should have.
 */
class PlaybackEngine {
public:
    typedef boost::uuids::uuid PlaybackEngineID;

    PlaybackEngine();
    virtual ~PlaybackEngine();

    virtual PlaybackState state() const = 0;

    virtual void openMedia(std::string const& path) = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void togglePlayPause() = 0;

    virtual double volume() const = 0;
    virtual void setVolume(double volume) = 0;
    virtual bool isMuted() const = 0;
    virtual void toggleMute() = 0;

    virtual bool canSeek() = 0;
    virtual boost::int64_t duration() = 0;
    virtual boost::int64_t position() = 0;
    virtual void setPosition(boost::int64_t newPosition) = 0;

    virtual PlaybackEngineID engineID() const = 0;
    virtual std::string engineName() const = 0;
    virtual PlaybackEngineCapabilities engineCapabilities() const = 0;

    virtual void registerEventsCallback(IPlaybackEventsCallback *callback) = 0;
    virtual void unregisterEventsCallback(IPlaybackEventsCallback *callback) = 0;
};

} }

#endif // WRAITH_PLAYBACK_PLAYBACK_ENGINE_HPP
