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
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "PlaybackEngineService.hpp"
#include "GStreamerPlaybackEngine.hpp"

namespace wraith { namespace playback {

namespace _private {

/**
 * TODO: Move this somewhere else.
 */
template <typename Engine>
class BasicPlaybackEngineFactory : public IClassFactory<PlaybackEngine> {
public:
    BasicPlaybackEngineFactory()
    { }

    virtual PlaybackEngine* createInstance() {
        return new Engine();
    }

    virtual void destroyInstance(PlaybackEngine* engine) {
        delete engine;
    }

    virtual void destroy()
    { delete this; }
};

} // namespace _private

const PlaybackEngineService::ServiceID PlaybackEngineService::playbackEngineServiceID(
    boost::lexical_cast<ServiceID>("c56ec633-3f30-4365-a070-cbf1a276d9aa")
);

PlaybackEngineService::PlaybackEngineService()
    : m_default_engine(0)
    , m_engines()
    , m_mutex()
{ registerDefaultEngine(); }

PlaybackEngineService::~PlaybackEngineService()
{
    typedef EngineMap::iterator Iter;

    Mutex::scoped_lock lock(m_mutex);

    for(Iter it = m_engines.begin(); it != m_engines.end(); ++it) {
        EnginePair engine = it->second;

        if(engine.second)
            (engine.first)->destroyInstance(engine.second);

        (engine.first)->destroy();
    }

    m_engines.clear();
}

void PlaybackEngineService::registerPlaybackEngine(PlaybackEngineFactoryPtr factory,
                                                  PlaybackEngineID const& engineId,
                                                  bool defaultEngine)
{
    if(!factory)
        return;

    Mutex::scoped_lock lock(m_mutex);

    if(m_engines.find(engineId) != m_engines.end())
        // Already has the engine, what should we do?
        return;

    PlaybackEnginePtr engine = 0;
    if(defaultEngine) {
        engine = factory->createInstance();
        m_default_engine = engine;
    }

    m_engines.insert(std::make_pair(engineId, std::make_pair(factory, engine)));
}

PlaybackEngineService::PlaybackEnginePtr
PlaybackEngineService::playbackEngine(PlaybackEngineID const& engineId) {
    PlaybackEnginePtr ret = 0;
    if(hasPlaybackEngine(engineId)) {
        Mutex::scoped_lock lock(m_mutex);

        EnginePair engine = m_engines[engineId];
        if(!engine.second)
            engine.second = (engine.first)->createInstance();

        ret = engine.second;
    }

    return ret;
}

PlaybackEngineService::PlaybackEnginePtr
PlaybackEngineService::defaultEngine() const
{ return m_default_engine; }

bool PlaybackEngineService::hasPlaybackEngine(PlaybackEngineID const& engineId) {
    Mutex::scoped_lock lock(m_mutex);
    return m_engines.find(engineId) != m_engines.end();
}

PlaybackEngineService::ServiceID
PlaybackEngineService::id() const {
    return PlaybackEngineService::playbackEngineServiceID;
}

std::string PlaybackEngineService::name() const {
    return std::string("PlaybackEngineService");
}

// TODO This should be removed when plugins are working...
void PlaybackEngineService::registerDefaultEngine() {
    registerPlaybackEngine(
        new _private::BasicPlaybackEngineFactory<GStreamerPlaybackEngine>(),
        GStreamerPlaybackEngine::playbackEngineID,
        true
    );
}

} } // namespace wraith::playback
