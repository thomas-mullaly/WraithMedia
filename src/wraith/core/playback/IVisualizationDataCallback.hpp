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
#ifndef WRAITH_PLAYBACK_IVISUALIZATION_DATA_CALLBACK_HPP
#define WRAITH_PLAYBACK_IVISUALIZATION_DATA_CALLBACK_HPP

#include <wraith/core/visualization/VisualizationData.hpp>

namespace wraith { namespace playback {

class IVisualizationDataCallback {
public:
    typedef visualization::VisualizationData::VisualizationDataPtr VisualizationDataPtr;

    virtual void onVisualizationData(VisualizationDataPtr data) = 0;
};

} } // namespace wraith::playback

#endif
