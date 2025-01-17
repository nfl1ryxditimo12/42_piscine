#include <iostream>

#include "ConfParser.hpp"
#include "Util.hpp"
#include "Socket.hpp"

bool webserv_fatal = false;

int main(int argc, char** argv) {
  if (webserv_fatal) {
    std::cerr << "Fatal error" << std::endl;
    return 1;
  }

  if (argc > 3) {
    std::cerr << "Usage: ./webserv config/example.conf" << std::endl;
    return 1;
  }

  ws::Configure conf;

  try {
    ws::ConfParser config_parser(argc == 1 ? "config/default.conf" : argv[1], ws::Util::get_root_dir());
    config_parser.parse(conf);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Configure: unknown error" << std::endl;
    return 1;
  }

  ws::Socket::init_server(conf);
  ws::Socket::run_server();
}
