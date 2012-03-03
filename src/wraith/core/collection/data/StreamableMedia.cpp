/*
 * Copyright 2010 Brian Bartman
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
#include "StreamableMedia.hpp"

wraith::collection::StreamableMedia::StreamableMedia() 
    :BaseType()
    , duration_(0)
{ }

wraith::collection::StreamableMedia::StreamableMedia(StreamableMedia const& x) 
    :BaseType(x)
    , duration_(x.duration_)
{ }

wraith::collection::StreamableMedia::~StreamableMedia() { }

wraith::collection::StreamableMedia const& wraith::collection::StreamableMedia::operator=(StreamableMedia const& media) {
    *static_cast<BaseType*>(this) = media;
    duration_ = media.duration_;
    return *this;
}

wraith::collection::StreamableMedia::DurationType wraith::collection::StreamableMedia::duration() const {
    return duration_;
}

wraith::collection::StreamableMedia::DurationType& wraith::collection::StreamableMedia::duration() {
    return duration_;
}

