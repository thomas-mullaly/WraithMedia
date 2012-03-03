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
#include <wraith/core/visualization/Drawable.hpp>

namespace wraith { namespace visualization {

Drawable::Drawable()
    : m_enabled(false)
{ }

Drawable::~Drawable()
{ }

bool Drawable::isEnabled() const {
    return m_enabled;
}

void Drawable::setEnabled(bool enabled) {
    m_enabled = enabled;
}

void Drawable::draw(IRenderingController *controller) {
    if(m_enabled)
        drawFrame(controller);
}

} } // namespace wraith::visualization
