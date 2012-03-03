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

#ifndef WRAITH_COLLECTION_STREAMABLE_MEDIA_HPP
#define WRAITH_COLLECTION_STREAMABLE_MEDIA_HPP
#include "MediaElement.hpp"

namespace wraith { namespace collection {

/** StreamableMedia
 *  Any media which can be
 */
class StreamableMedia
    :public MediaElement
{
    
public:
    typedef unsigned int DurationType;
    typedef MediaElement BaseType;

    /** Default constructor.
     *  sets duration to 0 seconds.
     */
    StreamableMedia();

    /** Copy Constructor
     *  Copies x into current instance.
     */
    StreamableMedia(StreamableMedia const& x);

    /** Destructor
     *  Frees resources associated with the current object.
     */
    ~StreamableMedia();

    /** assignment operator.
     *  Assign the current value in media into the this instance.
     */
    StreamableMedia const& operator=(StreamableMedia const& media);

    /** Duration
     *  Returns the duration in seconds for which a peice of media will play.
     */
    //@{
    virtual DurationType duration() const;
    virtual DurationType& duration();
    //@}

private:
    DurationType duration_;
};

}}

#endif
