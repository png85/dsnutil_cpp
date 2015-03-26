#include <iostream>
#include <dsnutil/singleton.h>

class Test : public dsn::Singleton<Test> {
    friend class dsn::Singleton<Test>;

private:
    Test() { std::cout << "Singleton ctor" << std::endl << std::flush; }
    ~Test() { std::cout << "Singleton dtor" << std::endl << std::flush; }

public:
    void hello() { std::cout << "Hello from Singleton!" << std::endl; }
};

int main(void) {
    std::cout << "Hello from main()!" << std::endl << std::flush;
    Test::getInstance().hello();
    std::cout << "Goodbye from main()!" << std::endl << std::flush;

    return EXIT_SUCCESS;
}
