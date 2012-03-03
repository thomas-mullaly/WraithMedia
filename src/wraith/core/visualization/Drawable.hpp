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
#ifndef WRAITH_VISUALIZATION_DRAWABLE_HPP
#define WRAITH_VISUALIZATION_DRAWABLE_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include <wraith/core/visualization/IRenderingController.hpp>

namespace wraith { namespace visualization {

class Drawable {
public:
    typedef boost::shared_ptr<Drawable> DrawablePtr;

    Drawable();
    virtual ~Drawable();

    bool isEnabled() const;
    void setEnabled(bool enabled);

    void draw(IRenderingController *controller);

    virtual std::string name() const = 0;

protected:
    virtual void drawFrame(IRenderingController *controller) = 0;

private:
    bool m_enabled;
};

} } // namespace wraith::visualization

#endif
