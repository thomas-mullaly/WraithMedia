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
#ifndef WRAITH_COLLECTION_MEDIA_ELEMENT_HPP
#define WRAITH_COLLECTION_MEDIA_ELEMENT_HPP
#include <QString>
#include <QUrl>
#include <vector>
#include <set>

namespace wraith { namespace collection {

/** an abstract meida element which contains information common to all different
 *  types of media; name duration etc.
 */
class MediaElement {
public:

    typedef std::set<QString> OwnerCollection;
    typedef std::set<QString> CreatorCollection;

    /** Default constructor
     *  TODO: This may need to be removed in the future if default construction
     *  of this type doesn't make sense.
     */
    MediaElement();
    
    /** Copy Constructor
     *  Copies a media element.
     */
    MediaElement(MediaElement const& element);

    /** Element wise construction
     *  This allows a user to construct a Media element over all of its
     *  information, all at once..
     */
    MediaElement(QString const& title, QString const& subTitle,
        QString const& genre, QUrl const& location);

    /** Destructor
     *  Responsible for freeing all information associated with the MediaElement.
     *  The destructor is virtual because it is intended to be derived from.
     */
    virtual ~MediaElement();


    /** Copy assignement operator
     *  Assigns a copy to of elem to the current MediaElement instance.
     */
    MediaElement const& operator=(MediaElement const& elem);

    /** Simple element utilities.
     *  The utilities apply to all media sub types although the meaning
     *  may change slightly in implementation (which is why everything is
     *  virtual). 
     *  Another reason they are virtual is because there may be implementations
     *  in the future which don't retreive the information until it's asked for.
     */
    //@{

    /** Exists
     *  Returns true if the URL exists and false other wise.
     *  TODO: The exists function my need to be better defined in the future 
     *  such that it determins that the media is stored locally before doing 
     *  exhaustive checking for something which may be located out on the
     *  internet.
     *
     *  This is used for making sure that the file exists in the location
     *  denoted by mediaLocation_.
     */
    virtual bool exists() const = 0;

    /** title
     *  Returns the title of the media.
     */
    virtual QString const& title() const;
    virtual QString& title();

    /** genre
     *  returns the genre of the media.
     */
    virtual QString const& genre() const;
    virtual QString& genre();

    /** location
     *  Returns the QUrl associated with this media.
     */
    virtual QUrl const& location() const;
    virtual QUrl& location();

    /** owners
     *  Returns collection of names of owners of the media.
     */
    virtual OwnerCollection const& owners() const;
    virtual OwnerCollection& owners();


    /** creators
     *  Returns the collection of creators of the media.
     */
    virtual CreatorCollection const& creators() const;
    virtual CreatorCollection& creators();
    //@}


private:
    QString             title_;
    QString             subTitle_;
    QString             genre_;
    QUrl                mediaLocation_;
    OwnerCollection     owners_;
    CreatorCollection   creators_;
};

} }

#endif
