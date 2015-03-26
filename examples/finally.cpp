#include <iostream>
#include <dsnutil/finally.h>

int main(void) {
    int r = EXIT_FAILURE;

    try {
        dsn::finally block([&]() {
            std::cout << "Hello from finally block :-)" << std::endl;
            r = EXIT_SUCCESS;
        });

        std::cout << "Doing stuff before exception..." << std::endl << std::flush;
        throw std::runtime_error("foo!");
        std::cout << "Doing stuff after exception...";
        return EXIT_FAILURE;
    }

    catch (std::exception& ex) {
        std::cout << "Caught exception: " << ex.what() << std::endl;
    }

    return r;
}
