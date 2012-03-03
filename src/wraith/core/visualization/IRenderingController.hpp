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
#ifndef WRAITH_VISUALIZATION_IRENDERING_CONTROLLER_HPP
#define WRAITH_VISUALIZATION_IRENDERING_CONTROLLER_HPP

#include <wraith/core/visualization/IPlaybackInfo.hpp>

namespace wraith { namespace visualization {

/* Forward declarations */
class IRenderer;

class IRenderingController {
public:
    IRenderingController() { }
    virtual ~IRenderingController() { }

    virtual IPlaybackInfo::PlaybackInfoPtr playbackInfo() = 0;

    virtual void render() = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual IRenderer*  renderer() = 0;
    virtual void        setRenderer(IRenderer *renderer) = 0;
};

} } // namespace wraith::visualization

#endif
