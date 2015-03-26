/// \file
/// \brief Singleton design pattern
///
/// \author Peter 'png' Hille <peter@das-system-networks.de>
#ifndef SINGLETON_HH
#define SINGLETON_HH 1

namespace dsn {
/// \brief Template for singleton classes
///
/// This template can be used to implement the "singleton" design pattern
/// on any class.
template <class Derived> class Singleton {
public:
    /// \brief Access singleton instance
    ///
    /// Returns a reference to the instance of this singleton.
    static Derived& getInstance() {
        static Derived instance;
        return instance;
    }

protected:
    /// \brief Default constructor
    ///
    /// \note This ctor is protected so that derived classes can implement
    /// their own logics for object initialization while still maintaining
    /// the impossibility of direct ctor calls!
    Singleton() {}

private:
    /// \brief Copy constructor
    ///
    /// \note This ctor is private to prevent multiple instances of the same
    /// singleton from being created through object assignments!
    Singleton(const Singleton&) {}
};

}

#endif // !SINGLETON_HH
