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
#ifndef WRAITH_PLAYBACK_PLAYBACK_ENGINE_SERVICE_HPP
#define WRAITH_PLAYBACK_PLAYBACK_ENGINE_SERVICE_HPP

#include <algorithm>

#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>

#include <wraith/core/playback/IPlaybackEngineService.hpp>

namespace wraith { namespace playback {

/**
 * Service used for creating/destroying different versions of PlaybackEngines.
 * Plugins can register their PlaybackEngine implementations with this service,
 * which allows other components to create instances of those PlaybackEngine's.
 */
class PlaybackEngineService : public IPlaybackEngineService {
public:
    static const ServiceID playbackEngineServiceID;

    PlaybackEngineService();
    virtual ~PlaybackEngineService();

    /**
     * Called by a plugin which exposes a PlaybackEngine implementation.
     * The plugin must provide a PlaybackEngineFactory implementation, which
     * can create and destory instances of the PlaybackEngine it's exposes. It
     * Must also specify the Unique ID of the engine which clients can use to
     * access it. The plugin can also specify whether its PlaybackEngine should
     * be the default engine to use.
     */
    virtual void registerPlaybackEngine(PlaybackEngineFactoryPtr factory, PlaybackEngineID const& engineId,
                                        bool defaultEngine);

    /** PlaybackEngine access functions. */
    //@{
    virtual PlaybackEnginePtr playbackEngine(PlaybackEngineID const& engineId);
    virtual PlaybackEnginePtr defaultEngine() const;
    virtual bool hasPlaybackEngine(PlaybackEngineID const& engineId);
    //@}

    /** IService implementation. */
    //@{
    virtual ServiceID id() const;
    virtual std::string name() const;
    //@}

private:
    typedef std::pair<PlaybackEngineFactoryPtr, PlaybackEnginePtr> EnginePair;
    typedef boost::unordered_map<PlaybackEngineID, EnginePair>  EngineMap;
    typedef boost::mutex Mutex;

    void registerDefaultEngine();

    PlaybackEnginePtr   m_default_engine;
    EngineMap           m_engines;
    mutable Mutex       m_mutex;
};

} } // namespace wraith::playback

#endif
