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
#ifndef WRAITH_VISUALIZATION_LINEAR_RENDERER_HPP
#define WRAITH_VISUALIZATION_LINEAR_RENDERER_HPP

#include <list>

#include <wraith/core/visualization/IRenderer.hpp>
#include <wraith/core/visualization/Drawable.hpp>

namespace wraith { namespace visualization {

class LinearRenderer : public IRenderer {
public:
    typedef Drawable::DrawablePtr DrawablePtr;
    typedef std::list<DrawablePtr> DrawablesList;

    LinearRenderer();
    virtual ~LinearRenderer();

    virtual void renderFrame(IRenderingController *controller);

    DrawablesList& drawables();

    virtual RendererID id() const = 0;
    virtual std::string name() const = 0;

protected:
    DrawablesList m_drawables;
};

} } // namespace wraith::visualization

#endif
