/*
 * Copyright 2010 Thomas Mullaly
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
#ifndef WRAITH_ICLASS_FACTORY_HPP
#define WRAITH_ICLASS_FACTORY_HPP

namespace wraith {

/**
 * Generic base class for all class factory implementations. Class factories are
 * used with the plugin system, allowing the plugin module to generate and destory
 * instances of classes it provides implementations for.
 */
template <typename Class>
class IClassFactory {
public:
    IClassFactory() { }
    virtual ~IClassFactory() { }

    /** Methods for creating and destorying the specific Class type. */
    //@{
    virtual Class*  createInstance() = 0;
    virtual void    destroyInstance(Class *inst) = 0;
    //@}

    virtual void    destroy() = 0;
};

} // namespace wraith

#endif
