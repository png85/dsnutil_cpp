// explicitly disable NDEBUG for release builds
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <dsnutil/throwing_assert.h>
#include <iostream>
#include <cstdlib>

int main()
{
    try {
        assert(1 == 0); // this should throw...
        std::cerr << __FILE__ << ":" << __LINE__ << ": This should not be reached since the assertion 1==0 always fails..." << std::endl;
        return EXIT_FAILURE;
    }

    catch (dsn::Exception& ex) {
        std::cerr << "Caught dsn::Exception: " << ex.what() << std::endl
                  << "This is expected :)" << std::endl;
    }

    try {
        assert(1 == 1); // this should not throw...
    }

    catch (dsn::Exception& ex) {
        std::cerr << "Caught unexpected dsn::Exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cerr << "Assertion tests succeeded." << std::endl;

    return EXIT_SUCCESS;
}
