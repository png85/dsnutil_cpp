#include <cmath>
#include <cstdlib>
#include <vector>

#include <dsnutil/parallel_for.h>

int main() {
    size_t size = 1e8;
    std::vector<double> v(size);

    parallel_for(size, [&](const size_t i) {
        v[i] = sin(2*M_PI*i/(double) size);
    });
}
