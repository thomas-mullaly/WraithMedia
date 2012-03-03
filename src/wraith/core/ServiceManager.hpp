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
#ifndef WRAITH_SERVICE_MANAGER_HPP
#define WRAITH_SERVICE_MANAGER_HPP

#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>

#include <wraith/core/IService.hpp>
#include <wraith/core/IClassFactory.hpp>

namespace wraith {

/**
 * Controls all the different services provided by different modules.
 * Components can access the different instances of Services through
 * the methods supplied by this class.
 */
class ServiceManager {
public:
    typedef IService::ServiceID     ServiceID;
    typedef IClassFactory<IService> ServiceFactory;
    typedef ServiceFactory*         ServiceFactoryPtr;
    typedef IService*               ServicePtr;

private:
    // There should only ever be one ServiceManager, so prevent
    // others from being created.
    ServiceManager();
    ServiceManager(ServiceManager const&);
    ~ServiceManager();
    ServiceManager& operator=(ServiceManager const&);

public:
    static ServiceManager*  instance();
    static void             destroy();

    /** Service Query functions. */
    //@{

    /**
     * Returns the Service with the specified or 0, if it doesn't
     * exist.
     */ 
    ServicePtr queryForService(ServiceID const& serviceId);

    /**
     * Returns true if the service with the given ID is registered with
     * the ServiceManager.
     */
    bool hasService(ServiceID const& serviceId);
    //@}

    /**
     * Registers a new Service with the Service manager.
     */
    void registerService(ServiceID const& serviceId, ServiceFactoryPtr factory,
                         bool createOnDemand = true);

private:
    typedef std::pair<ServiceFactoryPtr, ServicePtr>        ServicePair;
    typedef boost::unordered_map<ServiceID, ServicePair>    ServiceMap;

    typedef boost::mutex    Mutex;

    void cleanUpServices();

    static ServiceManager* inst;
    static void createInstance();

    ServiceMap m_services;
    Mutex     m_mutex;
};

} // namespace wraith

#endif
