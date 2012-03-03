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
#ifndef WRAITH_GUI_VISUALIZER_IRENDERER_HPP
#define WRAITH_GUI_VISUALIZER_IRENDERER_HPP

#include <string>

#include <boost/uuid/uuid.hpp>

#include <wraith/core/visualization/IRenderingController.hpp>

namespace wraith { namespace visualization {

class IRenderer {
public:
    typedef boost::uuids::uuid RendererID;

public:
    IRenderer() { }
    virtual ~IRenderer() { }

    virtual void renderFrame(IRenderingController *renderingController) = 0;

    virtual std::string name() const = 0;
    virtual RendererID id() const = 0;
};

} } // namespace wraith::visualization

#endif
