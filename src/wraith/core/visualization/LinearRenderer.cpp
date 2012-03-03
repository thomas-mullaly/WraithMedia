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
#include "LinearRenderer.hpp"

namespace wraith { namespace visualization {

LinearRenderer::LinearRenderer()
    : m_drawables()
{ }

LinearRenderer::~LinearRenderer()
{ m_drawables.clear(); }

void LinearRenderer::renderFrame(IRenderingController *controller) {
    typedef DrawablesList::iterator Iter;

    for(Iter it = m_drawables.begin(); it != m_drawables.end(); ++it)
        (*it)->draw(controller);
}

LinearRenderer::DrawablesList& LinearRenderer::drawables() {
    return m_drawables;
}

} } // namespace wraith::visualization
