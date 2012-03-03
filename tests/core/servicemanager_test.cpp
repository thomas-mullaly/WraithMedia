#include <stdexcept>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ServiceManagerTest
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#include <wraith/core/ServiceManager.hpp>

#define DEFINE_EXPECT(func) \
    static bool expect_ ## func = false, called_ ## func = false

#define SET_EXPECT(func) \
    do { called_ ## func = false; expect_ ## func = true; } while(0)

#define CHECK_EXPECT2(func) \
    do { \
        BOOST_CHECK( expect_ ##func ); \
        called_ ## func = true; \
    }while(0)

#define CHECK_EXPECT(func) \
    do { \
        CHECK_EXPECT2(func); \
        expect_ ## func = false; \
    }while(0)

#define CHECK_CALLED(func) \
    do { \
        BOOST_CHECK( called_ ## func ); \
        expect_ ## func = called_ ## func = false; \
    }while(0)

DEFINE_EXPECT(TestService_Create);
DEFINE_EXPECT(TestService_Destroy);
DEFINE_EXPECT(TestServiceFactory_CreateInstance);
DEFINE_EXPECT(TestServiceFactory_DestroyInstance);
DEFINE_EXPECT(TestServiceFactory_Destroy);

using namespace wraith;

BOOST_AUTO_TEST_CASE( ServiceManager_instance_tests )
{
    ServiceManager *ptr1 = ServiceManager::instance();
    BOOST_CHECK( ptr1 != 0 );

    ServiceManager *ptr2 = ServiceManager::instance();
    BOOST_CHECK( ptr1 == ptr2 );

    // Clean up for other tests.
    ServiceManager::destroy();
}

static const IService::ServiceID test_service_id(
    boost::lexical_cast<IService::ServiceID>("47a5727c-dc35-41dc-8fe7-26e3b466322c")
);

static const std::string test_service_name("Test Service");

class TestService : public IService {
public:
    typedef IService::ServiceID ServiceID;

    TestService() {
        CHECK_EXPECT(TestService_Create);
    }

    virtual ~TestService() {
        CHECK_EXPECT(TestService_Destroy);
    }

    virtual ServiceID id() const {
        return test_service_id;
    }

    virtual std::string name() const {
        return test_service_name;
    }
};

class TestServiceFactory : public IClassFactory<IService> {
public:
    TestServiceFactory() : test_service(0) { }
    virtual ~TestServiceFactory() { }

    virtual TestService* createInstance() {
        CHECK_EXPECT(TestServiceFactory_CreateInstance);
        if(!test_service)
            test_service = new TestService();
        return test_service;
    }

    virtual void destroyInstance(IService *service) {
        CHECK_EXPECT(TestServiceFactory_DestroyInstance);
        BOOST_CHECK( service == test_service );
        delete test_service;
        test_service = 0;
    }

    virtual void destroy() {
        CHECK_EXPECT(TestServiceFactory_Destroy);
        delete this;
    }

private:
    TestService *test_service;
};

static void test_register_service(bool onDemand)
{
    ServiceManager *manager = ServiceManager::instance();

    if(!onDemand) {
        SET_EXPECT(TestServiceFactory_CreateInstance);
        SET_EXPECT(TestService_Create);
    }

    manager->registerService(test_service_id, new TestServiceFactory(), onDemand);

    if(!onDemand) {
        CHECK_CALLED(TestServiceFactory_CreateInstance);
        CHECK_CALLED(TestService_Create);
    }
}

BOOST_AUTO_TEST_CASE( ServiceManager_registerService_OnDemand )
{
    ServiceManager *manager = ServiceManager::instance();
    BOOST_REQUIRE( manager != 0 );

    // Test service shouldn't be in the ServiceManager.
    IService *service = manager->queryForService(test_service_id);
    BOOST_CHECK( service == 0 );

    test_register_service(true);

    SET_EXPECT(TestServiceFactory_CreateInstance);
    SET_EXPECT(TestService_Create);

    service = manager->queryForService(test_service_id);

    CHECK_CALLED(TestService_Create);
    CHECK_CALLED(TestServiceFactory_CreateInstance);

    BOOST_REQUIRE( service != 0 );
    // It should be of our service type.
    BOOST_CHECK( service->id() == test_service_id );
    BOOST_CHECK( service->name() == test_service_name );

    SET_EXPECT(TestService_Destroy);
    SET_EXPECT(TestServiceFactory_DestroyInstance);
    SET_EXPECT(TestServiceFactory_Destroy);

    // The service should be destory when the ServiceManager is.
    ServiceManager::destroy();

    CHECK_CALLED(TestService_Destroy);
    CHECK_CALLED(TestServiceFactory_DestroyInstance);
    CHECK_CALLED(TestServiceFactory_Destroy);
}

BOOST_AUTO_TEST_CASE( ServiceManager_registerService )
{
    ServiceManager *manager = ServiceManager::instance();
    BOOST_REQUIRE( manager != 0 );

    IService *service = manager->queryForService(test_service_id);
    BOOST_CHECK( service == 0 );

    test_register_service(false);

    service = manager->queryForService(test_service_id);
    BOOST_CHECK( service != 0 );
    BOOST_CHECK( service->id() == test_service_id );
    BOOST_CHECK( service->name() == test_service_name );

    SET_EXPECT(TestServiceFactory_DestroyInstance);
    SET_EXPECT(TestService_Destroy);
    SET_EXPECT(TestServiceFactory_Destroy);

    ServiceManager::destroy();

    CHECK_CALLED(TestServiceFactory_DestroyInstance);
    CHECK_CALLED(TestService_Destroy);
    CHECK_CALLED(TestServiceFactory_Destroy);
}

BOOST_AUTO_TEST_CASE( ServiceManager_registerService_duplicate )
{
    ServiceManager *manager = ServiceManager::instance();

    test_register_service(false);

    IService *service = manager->queryForService(test_service_id);
    BOOST_CHECK( service != 0 );
    BOOST_CHECK( service->id() == test_service_id );
    BOOST_CHECK( service->name() == test_service_name );

    BOOST_CHECK_THROW( test_register_service(false), std::invalid_argument );

    SET_EXPECT(TestServiceFactory_DestroyInstance);
    SET_EXPECT(TestService_Destroy);
    SET_EXPECT(TestServiceFactory_Destroy);

    ServiceManager::destroy();

    CHECK_CALLED(TestServiceFactory_DestroyInstance);
    CHECK_CALLED(TestService_Destroy);
    CHECK_CALLED(TestServiceFactory_Destroy);
}
