#include <string>
#include <iostream>
#include <cstdlib>

#include <dsnutil/hexdump.h>

#ifdef WITH_LOG4CPP
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/BasicLayout.hh>
#endif

int main(int argc, char** argv) {
  std::string msg = "libdsnutil/hexdump test\n"
    "(c) 2012 #das-system networks\n";

  std::cout << msg << std::endl;

  int test_mode{1};
  if (argc == 2) {
    test_mode = atoi(argv[1]);
  }

  if (test_mode == 1) {
    std::cout << "step 1: testing normal hexdump() to std::cout"
	      << std::endl;
    hexdump(msg.c_str(), msg.length(), std::cout);
  }

  if (test_mode == 2) {
#ifdef WITH_LOG4CPP
    std::cout << std::endl
	      << "step 2: testing log4cpp output" << std::endl;

    log4cpp::Category& log = 
      log4cpp::Category::getInstance("libdsnutil");
    try {
      log4cpp::Appender* a =
	new log4cpp::OstreamAppender("libdsnutil", &std::cout);
      a->setLayout(new log4cpp::BasicLayout());
      log.addAppender(a);
      log.setPriority(log4cpp::Priority::DEBUG);
    }

    catch (std::bad_alloc& ex) {
      std::cerr << "caught std::bad_alloc when trying to allocate "
		<< "memory for log4cpp objects: "
		<< ex.what();
      return EXIT_FAILURE;
    }

    log.debugStream() << "Initialized log4cpp::Category for stdout output";
    hexdump(msg.c_str(), msg.length(), log.infoStream());

    log4cpp::Category::shutdown();
#endif
  }

  return EXIT_SUCCESS;
}
