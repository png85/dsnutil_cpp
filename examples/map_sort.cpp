#include <iostream>
#include <string>

#include <dsnutil/throwing_assert.h>
#include <dsnutil/map_sort.h>
#include <dsnutil/pretty_print.h>

int main(void) {
    std::map<std::string, float> stocks = {
        std::pair<std::string, float>("YHOO", 5.0f),
        std::pair<std::string, float>("MSFT", 7.5f),
        std::pair<std::string, float>("GOOG", 9.9f),
        std::pair<std::string, float>("DSN ", 13.37f),
        std::pair<std::string, float>("IBM ", 6.0f),
        std::pair<std::string, float>("ACME", 1.5f)
    };

    std::function<void (std::string)> vector_dump = [&](std::string k) {
        std::cout << "stocks['" << k << "'] = " << stocks[k] << std::endl;
    };

    std::cout << "Unsorted map at start: " << stocks << std::endl;


    // extract map keys into existing vector
    std::vector<std::string> k1;
    dsn::map_keys(stocks, k1);
    std::cout << "Unsorted map keys in existing vector: " << k1 << "; map data is:" << std::endl;
    std::for_each(k1.begin(), k1.end(), vector_dump);


    // extract map keys into new vector
    std::cout << "Unsorted map keys in new vector: " << dsn::map_keys(stocks) << std::endl;
    assert(k1 == dsn::map_keys(stocks));


    // sort map keys into existing vector
    std::vector<std::string> k2;
    dsn::map_sort(stocks, k2);
    std::cout << "Sorted map keys in existing vector: " << k2 << "; map data is:" << std::endl;
    std::for_each(k2.begin(), k2.end(), vector_dump);


    // sort map keys into new vector
    std::cout << "Sorted map keys in new vector: " << dsn::map_sort(stocks) << std::endl;
    assert(k2 == dsn::map_sort(stocks));

    std::function<bool (std::string, std::string)> desc_sort = [&](std::string a, std::string b) {
        return (stocks[a] > stocks[b]);
    };

    // sort map keys into existing vector w/ custom comparator
    std::vector<std::string> k3;
    dsn::map_sort(stocks, k3, desc_sort);
    std::cout << "Sorted map keys (descending) in existing vector: " << k3 << "; map data is:" << std::endl;
    std::for_each(k3.begin(), k3.end(), vector_dump);

    // sort map keys into new vector w/ custom comparator
    std::cout << "Sorted map keys (descending) in new vector: " << dsn::map_sort(stocks, desc_sort) << std::endl;
    assert(k3 == dsn::map_sort(stocks, desc_sort));

    return 0;
}
