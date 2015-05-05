#ifndef NULL_DELETER_H
#define NULL_DELETER_H

namespace dsn {
struct null_deleter {
    void operator()(const void*) const {}
};
}

#endif // NULL_DELETER_H
