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
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "VisualizationService.hpp"

namespace wraith { namespace visualization {

const VisualizationService::ServiceID VisualizationService::visualizationServiceID(
    boost::lexical_cast<ServiceID>("254666c0-a742-4ceb-827f-27999b3fba57")
);

VisualizationService::VisualizationService()
    : m_renderers()
    , m_renderersMutex()
    , m_eventCallbacks()
    , m_eventMutex()
{ }

VisualizationService::~VisualizationService() {
    clearEventCallbacks();
    clearRenderers();
}

void VisualizationService::registerRenderer(RendererFactoryPtr rendererFactory,
                                            RendererID const& id, std::string const& name)
{
    if(!rendererFactory)
        throw std::invalid_argument("\"rendererFactory\" cannot be null");

    if(hasRenderer(id))
        throw std::invalid_argument("A renderer with the specified id already exists");

    insertRenderer(rendererFactory, id, name);
    fireRegisteredEvent(id, name);
}

IRenderer* VisualizationService::createRenderer(RendererID const& id) {
    if(!hasRenderer(id))
        throw std::invalid_argument("A renderer with the given \"id\" is not registered with this service");

    m_renderersMutex.lock();
    IRenderer *renderer = m_renderers[id].factory->createInstance();
    m_renderersMutex.unlock();

    return renderer;
}

void VisualizationService::destroyRenderer(IRenderer *renderer) {
    if(!renderer)
        throw std::invalid_argument("\"renderer\" cannot be null");

    if(!hasRenderer(renderer->id()))
        throw std::invalid_argument("Unable to destroy the given renderer");

    m_renderersMutex.lock();
    m_renderers[renderer->id()].factory->destroyInstance(renderer);
    m_renderersMutex.unlock();
}

bool VisualizationService::hasRenderer(RendererID const& id) {
    Mutex::scoped_lock lock(m_renderersMutex);
    return m_renderers.find(id) != m_renderers.end();
}

void VisualizationService::registerServiceCallback(IVisualizationServiceCallback *callback) {
    Mutex::scoped_lock lock(m_eventMutex);
    m_eventCallbacks.insert(callback);
}

void VisualizationService::unregisterServiceCallback(IVisualizationServiceCallback *callback) {
    Mutex::scoped_lock lock(m_eventMutex);
    m_eventCallbacks.erase(callback);
}

VisualizationService::ServiceID VisualizationService::id() const {
    return visualizationServiceID;
}

std::string VisualizationService::name() const {
    return std::string("Visualization service");
}

void VisualizationService::clearRenderers() {
    typedef RendererMap::iterator Iter;

    Mutex::scoped_lock lock(m_renderersMutex);
    for(Iter it = m_renderers.begin(); it != m_renderers.end(); ++it) {
        RendererInfo info = it->second;

        if(info.factory)
            info.factory->destroy();
    }
    m_renderers.clear();
}

void VisualizationService::clearEventCallbacks() {
    Mutex::scoped_lock lock(m_eventMutex);
    m_eventCallbacks.clear();
}

void VisualizationService::insertRenderer(RendererFactoryPtr factory, RendererID const& id, std::string const& name)
{
    RendererInfo info;
    info.factory = factory;
    info.name = name;

    Mutex::scoped_lock lock(m_renderersMutex);
    m_renderers.insert(std::make_pair(id, info));
}

void VisualizationService::fireRegisteredEvent(RendererID const& id, std::string const& name)
{
    typedef EventCallbacks::iterator Iter;

    m_eventMutex.lock();
    for(Iter it = m_eventCallbacks.begin(); it != m_eventCallbacks.end(); ++it)
        (*it)->rendererRegistered(id, name);
    m_eventMutex.unlock();
}

} } // namespace wraith::visualization
