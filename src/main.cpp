#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include "display.hpp"
#include "keyboard.hpp"
#include "interpreter.hpp"


namespace po = boost::program_options;


void print_usage(const po::options_description& opts) noexcept;


int main(int argc, char** argv)
{
  std::string input_file;
  unsigned memory_size;

  po::options_description opts("Options");
  opts.add_options()
    ("help,h", "print this help and exit")
    (
        "size,s"
      , po::value<unsigned>(&memory_size)->default_value(30'000)
      , "specify the size of memory to use"
    )
    (
        "file,f"
      , po::value<std::string>(&input_file)
      , "read input from file"
    )
  ;

  po::variables_map vm;
  po::command_line_parser parser(argc, argv);

  try {
    po::store(parser.options(opts).run(), vm);
    po::notify(vm);
  }
  catch (const po::error& error) {
    std::cerr << error.what() << std::endl;
    print_usage(opts);
    return EXIT_FAILURE;
  }

  if (vm.count("help")) {
    print_usage(opts);
    return EXIT_SUCCESS;
  }

  StdDisplay display;
  StdKeyboard keyboard;
  Interpreter interpreter(display, keyboard, memory_size);

  std::string input;
  if (vm.count("file")) {
    try {
      std::ifstream ifs(input_file);
      ifs >> input;
    }
    catch (const std::exception& exc) {
      std::cerr << exc.what() << std::endl;
      return EXIT_FAILURE;
    }
  }
  else {
    std::cin >> input;
  }
  
  interpreter.run(input);
  std::cout << std::endl;

  return EXIT_SUCCESS;
}


void print_usage(const po::options_description& opts) noexcept
{
  std::cout 
    << "Usage: brainfuck [OPTS]...\n"
    << "Run the provided input through the brainfuck interpreter\n\n"
    << opts
    << std::endl;
}

