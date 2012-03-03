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
#ifndef WRAITH_PLAYBACK_PLAYBACK_STATE_HPP
#define WRAITH_PLAYBACK_PLAYBACK_STATE_HPP

namespace wraith { namespace playback {

/**
 * The possible states of a PlabackEngine.
 */
enum PlaybackState {
    PLAYBACK_STATE_UNKNOWN,
    PLAYBACK_STATE_IDLE,
    PLAYBACK_STATE_READY,
    PLAYBACK_STATE_PLAYING,
    PLAYBACK_STATE_PAUSED,
    PLAYBACK_STATE_STOPPED,
    PLAYBACK_STATE_ERROR
};

} } // namespace wraith::playback

#endif
