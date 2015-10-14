#ifndef NULL_DELETER_H
#define NULL_DELETER_H

namespace dsn {
/// \brief Dummy deletion functor
///
/// This can be used to provide a dummy deleter for managing external resources
/// in smart pointers & co
///
/// \note The requirement to use this usually indicates design flaws so think twice
/// before doing so! --png
struct null_deleter {
    void operator()(const void*) const {}
};
}

#endif // NULL_DELETER_H
