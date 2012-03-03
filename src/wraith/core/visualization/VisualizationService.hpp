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
#ifndef WRAITH_VISUALIZATION_VISUALIZATION_SERVICE_HPP
#define WRAITH_VISUALIZATION_VISUALIZATION_SERVICE_HPP

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/thread/mutex.hpp>

#include <wraith/core/visualization/IVisualizationService.cpp>

namespace wraith { namespace visualization {

/* Used internally. */
struct RendererInfo {
    typedef IVisualizationService::RendererFactoryPtr RendererFactoryPtr;

    RendererFactoryPtr factory;
    std::string        name;
};

class VisualizationService : public IVisualizationService {
public:
    static const ServiceID visualizationServiceID;

    VisualizationService();
    virtual ~VisualizationService();

    virtual void registerRenderer(RendererFactoryPtr rendererFactory, RendererID const& id, std::string const& name);

    virtual IRenderer*  createRenderer(RendererID const& id);
    virtual void        destroyRenderer(IRenderer* renderer);

    virtual bool hasRenderer(RendererID const& id);

    virtual void registerServiceCallback(IVisualizationServiceCallback *callback);
    virtual void unregisterServiceCallback(IVisualizationServiceCallback *callback);

    virtual ServiceID id() const;
    virtual std::string name() const;

private:
    void clearRenderers();
    void clearEventCallbacks();

    void insertRenderer(RendererFactoryPtr rendererFactory, RendererID const& id, std::string const& name);
    void fireRegisteredEvent(RendererID const& id, std::string const& name);

protected:
    typedef boost::unordered_map<RendererID, RendererInfo>          RendererMap;
    typedef boost::unordered_set<IVisualizationServiceCallback*>    EventCallbacks;
    typedef boost::mutex Mutex; 

    RendererMap     m_renderers;
    Mutex           m_renderersMutex;
    EventCallbacks  m_eventCallbacks;
    Mutex           m_eventMutex;
};

} } // namespace wraith::visualization

#endif
