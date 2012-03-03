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
#ifndef WRAITH_PLAYBACK_IVISUALIZATION_SOURCE_HPP
#define WRAITH_PLAYBACK_IVISUALIZATION_SOURCE_HPP

#include <wraith/core/playback/IVisualizationDataCallback.hpp>

namespace wraith { namespace playback {

class IVisualizationSource {
public:
    virtual bool isVisualizationSupported() const = 0;
    virtual bool isVisualizationEnabled() const = 0;
    virtual void setVisualizationEnabled(bool enable) = 0;
    virtual int visualizationDataThreshold() = 0;

    virtual void registerVisualizationDataCallback(IVisualizationDataCallback *callback) = 0;
    virtual void unregisterVisualizationDataCallback(IVisualizationDataCallback *callback) = 0;

    IVisualizationSource() { }
    virtual ~IVisualizationSource() { }
};

} } // namespace wraith::playback

#endif // WRAITH_PLAYBACK_IVISUALIZATON_SOURCE_HPP
