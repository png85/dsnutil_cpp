#define BOOST_TEST_MODULE "dsnutil_cpp-base64"

#include <dsnutil/base64.h>

#include <string>

#include <boost/test/unit_test.hpp>

namespace {
static const std::string REFERENCE_PLAINTEXT{ "Hello, world!" };
static const std::string REFERENCE_ENCODED{ "SGVsbG8sIHdvcmxkIQ==" };
}

BOOST_AUTO_TEST_CASE(base64_encode)
{
    std::string source = REFERENCE_PLAINTEXT;
    std::string encoded = dsn::base64::encode(source);
    BOOST_CHECK(encoded == REFERENCE_ENCODED);
}

BOOST_AUTO_TEST_CASE(base64_decode)
{
    std::string source = REFERENCE_ENCODED;
    std::string decoded = dsn::base64::decode(source);
    BOOST_CHECK(decoded == REFERENCE_PLAINTEXT);
}
