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
#ifndef WRAITH_BOOTSTRAPPER_HPP
#define WRAITH_BOOTSTRAPPER_HPP

#include <wraith/core/ServiceManager.hpp>

namespace wraith {

class Bootstrapper {
public:
    Bootstrapper();
    virtual ~Bootstrapper();

    virtual void bootServices();

private:
    template <typename ServiceType>
    void bootService(IService::ServiceID const& serviceId, bool autoStart) {
        ServiceManager *serviceManager = ServiceManager::instance();
        serviceManager->registerService(serviceId, new BasicServiceFactory<ServiceType>(), autoStart);
    }
};

} // namespace wraith

#endif
