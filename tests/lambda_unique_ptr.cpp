#define BOOST_TEST_MODULE "dsn::lambda_unique_ptr"

#include <dsnutil/lambda_unique_ptr.hpp>
#include <iostream>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(basic_lambda_unique_ptr)
{
    using ptr_type = dsn::lambda_unique_ptr<void>;
    bool free_executed{ false };

    {
        ptr_type ptr(malloc(10 * sizeof(char)), [&](void* ptr) {
            std::cout << "Hello from lambda in lambda_unique_ptr. Got ptr=" << ptr << std::endl;
            if (ptr != nullptr) {
                free_executed = true;
                free(ptr);
            }
        });
        BOOST_CHECK(ptr.get() != nullptr);
    }

    BOOST_CHECK(free_executed);
}
