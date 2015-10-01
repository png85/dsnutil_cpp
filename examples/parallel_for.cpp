#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <cstdlib>
#include <vector>

#include <dsnutil/parallel_for.h>

int main()
{
    size_t size = 1e4;
    std::vector<double> v(size);

    dsn::parallel_for(size, [&](const size_t i) {
        v[i] = sin(2*M_PI*i/(double) size);
    });
}
