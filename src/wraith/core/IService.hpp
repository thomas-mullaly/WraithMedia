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
#ifndef WRAITH_ISERVICE_HPP
#define WRAITH_ISERVICE_HPP

#include <string>

#include <boost/uuid/uuid.hpp>

#include <wraith/core/IClassFactory.hpp>

namespace wraith {

class IService {
public:
    typedef boost::uuids::uuid ServiceID;

    IService() { }
    virtual ~IService() { }

    virtual std::string name() const = 0;
    virtual ServiceID id() const = 0;
};

template <typename T>
class BasicServiceFactory : public IClassFactory<IService> {
public:
    BasicServiceFactory()
        : m_inst(0)
    { }

    virtual IService* createInstance() {
        if(!m_inst)
            m_inst = new T();

        return m_inst;
    }

    virtual void destroyInstance(IService *inst) {
        if(m_inst)
            delete m_inst;
        m_inst = 0;
    }

    virtual void destroy() {
        destroyInstance(m_inst);
        delete this;
    }

    virtual ~BasicServiceFactory() { }

private:
    T *m_inst;
};

} // namespace wraith

#endif // WRAITH_ISERVICE_HPP
