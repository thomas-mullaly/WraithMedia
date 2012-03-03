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
#ifndef WRAITH_COLLECTION_ICOLLECTION_HPP
#define WRAITH_COLLECTION_ICOLLECITON_HPP

#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>

#include <wraith/core/media/ITrack.hpp>

namespace wraith { namespace collection {

class ICollection {
public:
    typedef boost::shared_ptr<media::ITrack> TrackPtr;
    typedef boost::uuids::uuid CollectionID;

public:
    ICollection() { }
    virtual ~ICollection() { }

    virtual void initializeCollection() = 0;
    virtual void uninitalizeCollection() = 0;

    virtual TrackPtr trackForUrl(std::string const& url) = 0;

    virtual std::string collectionName() const = 0;
    virtual CollectionID collectionID() const = 0;
};

} }

#endif
