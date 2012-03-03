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

#include "MediaElement.hpp"

wraith::collection::MediaElement::MediaElement()
    : title_("")
    , subTitle_("")
    , genre_("")
    , mediaLocation_("")    
{ }

wraith::collection::MediaElement::MediaElement(MediaElement const& element) 
    : title_(element.title_)
    , subTitle_(element.subTitle_)
    , genre_(element.genre_)
    , mediaLocation_(element.mediaLocation_)
{ }

wraith::collection::MediaElement::MediaElement(QString const& title,
        QString const& subTitle,
        QString const& genre,
        QUrl const& location)
    : title_(title)
    , subTitle_(subTitle)
    , genre_(genre)
    , mediaLocation_(location)
{ }

wraith::collection::MediaElement::~MediaElement() { }

wraith::collection::MediaElement const& wraith::collection::MediaElement::operator=(MediaElement const& elem) {
    title_ = elem.title_;
    subTitle_ = elem.subTitle_;
    genre_ = elem.genre_;
    owners_ = elem.owners_;
    creators_ = elem.creators_;
    mediaLocation_ = elem.mediaLocation_;
    return *this;
}

QString const& wraith::collection::MediaElement::title() const {
    return title_;
}

QString& wraith::collection::MediaElement::title() {
    return title_;
}

QString const& wraith::collection::MediaElement::genre() const {
    return genre_;
}

QString& wraith::collection::MediaElement::genre() {
    return genre_;
}

QUrl const& wraith::collection::MediaElement::location() const {
    return mediaLocation_;
}

QUrl& wraith::collection::MediaElement::location() {
    return mediaLocation_;
}

wraith::collection::MediaElement::OwnerCollection const& wraith::collection::MediaElement::owners() const {
    return owners_;
}

wraith::collection::MediaElement::OwnerCollection& wraith::collection::MediaElement::owners() {
    return owners_;
}

wraith::collection::MediaElement::CreatorCollection const& wraith::collection::MediaElement::creators() const {
    return creators_;
}

wraith::collection::MediaElement::CreatorCollection& wraith::collection::MediaElement::creators() {
    return creators_;
}

