#define BOOST_TEST_MODULE brainfuck Test Module
#include <boost/test/unit_test.hpp>

#include <string>

#include <keyboard.hpp>
#include <display.hpp>
#include <interpreter.hpp>


struct KeyboardTestFixture : public Keyboard
{
  char read() const noexcept override
  {
    did_read = true;
    read_return = std::rand() % 0xff;  // exclude initialization value
    return read_return;
  }

  mutable bool did_read = false;
  mutable char read_return = 0xff;
};


struct DisplayTestFixture : public Display
{
  void write(const char chr) const noexcept override
  {
    did_write = true;
    wrote += chr;
  }

  mutable bool did_write = false;
  mutable std::string wrote = "";
};


struct InterpreterTestFixture
{
  InterpreterTestFixture()
    : interpreter(display, keyboard)
  {}

  Interpreter interpreter;
  KeyboardTestFixture keyboard;
  DisplayTestFixture display;
};


BOOST_FIXTURE_TEST_SUITE( brainfuck_test_suite, InterpreterTestFixture )


BOOST_AUTO_TEST_CASE( test_increment_data_pointer )
{
  interpreter.run(">+");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[1], 1);
}


BOOST_AUTO_TEST_CASE( test_decrement_data_pointer )
{
  interpreter.run(">><+");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[1], 1);
}


BOOST_AUTO_TEST_CASE( test_increment )
{
  interpreter.run("+");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[0], 1);
}


BOOST_AUTO_TEST_CASE( test_decrement )
{
  interpreter.run("++-");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[0], 1);
}


BOOST_AUTO_TEST_CASE( test_write )
{
  interpreter.run("+.");
  BOOST_REQUIRE(display.did_write);
  BOOST_REQUIRE_EQUAL(display.wrote[0], interpreter.memory()[0]);
}


BOOST_AUTO_TEST_CASE( test_read )
{
  interpreter.run(",");
  BOOST_REQUIRE(keyboard.did_read);
  BOOST_REQUIRE_EQUAL(keyboard.read_return, interpreter.memory()[0]); 
}


BOOST_AUTO_TEST_CASE( test_jump_forward )
{
  interpreter.run("[>+<]");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[1], 0);
}


BOOST_AUTO_TEST_CASE( test_jump_backward )
{
  interpreter.run("+>+<[>]+++");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[2], 3);
}


BOOST_AUTO_TEST_CASE( test_nested_jumps )
{
  interpreter.run("+++[>+++[-]<-]");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[0], 0);
}


BOOST_AUTO_TEST_CASE( test_hello_world )
{
  interpreter.run("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++");
  BOOST_REQUIRE(display.did_write);
  BOOST_REQUIRE_EQUAL(display.wrote, "Hello World!");
}


BOOST_AUTO_TEST_CASE( ignores_comments )
{
  interpreter.run("this is a comment ++ ");
  BOOST_REQUIRE_EQUAL(interpreter.memory()[0], 2);
}


BOOST_AUTO_TEST_SUITE_END()
