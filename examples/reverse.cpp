#include <cstdlib>
#include <vector>
#include <dsnutil/make_reverse.h>
#include <dsnutil/pretty_print.h>

int main()
{
    std::vector<int> v1{ { 1, 2, 3, 4, 5 } };
    std::vector<int> v2{ { 5, 4, 3, 2, 1 } };

    std::cout << "v1: " << v1 << std::endl
              << "v2: " << v2 << std::endl;

    auto rev = dsn::make_reverse(v1);
    std::vector<int> v3{ rev.begin(), rev.end() };
    std::cout << "v3: " << v3 << std::endl;

    if (v2 != v3) {
        std::cerr << "v3 != v2, test failed!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
