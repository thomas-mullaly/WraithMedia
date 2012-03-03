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
#include <stdexcept>

#include <wraith/core/ServiceManager.hpp>

namespace wraith {

ServiceManager* ServiceManager::inst = 0;

ServiceManager* ServiceManager::instance() {
    if(!inst) {
        createInstance();
    }
    
    return inst;
}

void ServiceManager::destroy() {
    inst->cleanUpServices();

    delete inst;
    inst = 0;
}

ServiceManager::ServiceManager()
    : m_services()
    , m_mutex()
{ }

ServiceManager::~ServiceManager() { }

ServiceManager::ServicePtr
ServiceManager::queryForService(ServiceID const& serviceId) {
    ServicePtr ret = 0;

    if(hasService(serviceId)) {
        Mutex::scoped_lock lock(m_mutex);

        ServicePair &serv = m_services[serviceId];
        if(!serv.second) {
            // Should we check if the creation succeeded???
            serv.second = (serv.first)->createInstance();
        }

        ret = serv.second;
    }

    return ret;
}

bool ServiceManager::hasService(ServiceID const& serviceId) {
    Mutex::scoped_lock lock(m_mutex);
    return m_services.find(serviceId) != m_services.end();
}

void ServiceManager::registerService(ServiceID const& serviceId, ServiceFactoryPtr factory,
                                     bool createOnDemand)
{
    if(!factory)
        throw std::invalid_argument("\"factory\" cannot be NULL");

    if(hasService(serviceId))
        throw std::invalid_argument("A service with this \"serviceId\" already exists");

    Mutex::scoped_lock lock(m_mutex);

    ServicePtr service = 0;
    if(!createOnDemand)
        service = factory->createInstance();

    ServicePair serviceInfo = std::make_pair(factory, service);
    m_services.insert(std::make_pair(serviceId, serviceInfo));
}

void ServiceManager::cleanUpServices() {
    typedef ServiceMap::iterator Iter;

    m_mutex.lock();
    for(Iter i = m_services.begin(); i != m_services.end(); ++i) {
        ServicePair &pair = i->second;

        /* Destory the instance of the Service (if it was created). */
        if(pair.second) {
            (pair.first)->destroyInstance(pair.second);
        }

        /* Then destory the ClassFactory instance. */
        (pair.first)->destroy();
    }
    m_services.clear();
    m_mutex.unlock();
}

void ServiceManager::createInstance() {
    inst = new ServiceManager();
}

} // namespace wraith
