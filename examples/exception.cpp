#include <dsnutil/exception.h>
#include <iostream>
#include <cstdlib>

int main()
{
    try {
        DSN_DEFAULT_EXCEPTION("Test exception", DSN_EXCEPTION_SOURCE);
        return EXIT_FAILURE;
    }
    catch (dsn::Exception& ex) {
        std::cerr << "Caught expected exception: " << ex.what() << std::endl;
    }

    try {
        DSN_EXCEPTION(dsn::Exception, "Second test exception", DSN_EXCEPTION_SOURCE);
        return EXIT_FAILURE;
    }
    catch (dsn::Exception& ex) {
        std::cerr << "Caught expected exception: " << ex.what() << std::endl;
    }

    try {
        DSN_DEFAULT_EXCEPTION_SIMPLE("Third test exception");
        return EXIT_FAILURE;
    }
    catch (dsn::Exception& ex) {
        std::cerr << "Caught expected exception: " << ex.what() << std::endl;
    }

    try {
        DSN_EXCEPTION_SIMPLE(dsn::Exception, "Fourth test exceptuion");
        return EXIT_FAILURE;
    }
    catch (dsn::Exception& ex) {
        std::cerr << "Caught expected exception: " << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
