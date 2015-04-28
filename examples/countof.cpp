#include <dsnutil/countof.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

int main()
{
    const char str1[] = "Hello, world!";
    std::cout << "str1: '" << str1 << "', countof(str1)=" << dsn::countof(str1) << std::endl;
    if (dsn::countof(str1) != strlen(str1) + 1) {
        std::cout << "countof(str1) != strlen(str1)+1, test failed!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
