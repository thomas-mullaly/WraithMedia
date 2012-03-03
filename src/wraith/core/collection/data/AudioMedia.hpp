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
#ifndef WRAITH_COLLECTION_MEDIA_MUSIC_HPP
#define WRAITH_COLLECTION_MEDIA_MUSIC_HPP
#include "StreamableMedia.hpp"
#include <QTime>

namespace wraith { namespace collection {

/** AudioMedia
 *  Represents a single audio clip. That means that this can either be a song
 *  or it can some audible noise which isn't actually music.
 *  The primary class of this media type is to put all audio into a single
 *  category inside of media. Music will than be a sub category of that.
 */
class AudioMedia
    :public StreamableMedia
{
public:
    typedef StreamableMedia BaseType;


    /** Default constructor.
     *  Default constructs a Music Media.
     */
    AudioMedia();

    /** Copy Constructor
     *  Creates a copy of AudioMedia instance music as the current Music
     *  instance.
     */
    AudioMedia(AudioMedia const& audio);

    /** Destructor
     *  Frees resources associated with AudioMedia
     */
    ~AudioMedia();
    
    /** Copy assignment 
     *  assigns a copy to the current instance.
     */
    AudioMedia const& operator=(AudioMedia const& audio);

private:
};

} }

#endif
