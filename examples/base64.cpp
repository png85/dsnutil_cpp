#include <cstdlib>
#include <iostream>

#include <dsnutil/base64.h>

int main()
{
    std::string s = "base64 library test...";
    std::string encoded = dsn::base64::encode(s);
    std::string decoded = dsn::base64::decode(encoded);

    std::cout << "source : " << s << std::endl
              << "encoded: " << encoded << std::endl
              << "decoded: " << decoded << std::endl;

    if (decoded != s) {
        std::cout << "Decoded string didn't match input; test failed!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Test succeeded :-)" << std::endl;

    return EXIT_SUCCESS;
}
