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
#ifndef WRAITH_VISUALIZATION_IVISUALIZATION_SERVICE_HPP
#define WRAITH_VISUALIZATION_IVISUALIZATION_SERVICE_HPP

#include <wraith/core/IService.hpp>
#include <wraith/core/IClassFactory.hpp>
#include <wraith/core/visualization/IRenderer.hpp>
#include <wraith/core/visualization/IVisualizationServiceCallback.hpp>

namespace wraith { namespace visualization {

class IVisualizationService : public IService {
public:
    typedef IClassFactory<IRenderer>    RendererFactory;
    typedef RendererFactory*            RendererFactoryPtr;
    typedef IRenderer::RendererID       RendererID;

public:
    IVisualizationService() { }
    virtual ~IVisualizationService() { }

    virtual void registerRenderer(RendererFactoryPtr rendererFactory, RendererID const& id, std::string const& rendererName) = 0;

    virtual IRenderer*  createRenderer(RendererID const& id) = 0;
    virtual void        destroyRenderer(IRenderer *renderer) = 0;

    virtual bool hasRenderer(RendererID const& id) = 0;

    virtual void registerServiceCallback(IVisualizationServiceCallback *callback) = 0;
    virtual void unregisterServiceCallback(IVisualizationServiceCallback *callback) = 0;

    virtual ServiceID id() const = 0;
    virtual std::string name() const = 0;
};

} } // namespace wraith::visualization

#endif
