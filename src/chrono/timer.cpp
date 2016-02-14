#include <dsnutil/chrono/timer.hpp>

dsn::chrono::time_point dsn::chrono::timer::point() const { return m_point; }

void dsn::chrono::timer::reset() { m_point = dsn::chrono::now(); }

dsn::chrono::duration dsn::chrono::timer::elapsed() const { return dsn::chrono::now() - m_point; }
