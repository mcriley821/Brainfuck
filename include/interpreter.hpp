#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <concepts>
#include <vector>

#include "display.hpp"
#include "keyboard.hpp"


template<typename T>
concept StringLike = requires (T a)
{
  { a[0] } -> std::same_as<char&>;
} || requires (T a)
{
  { a[0] } -> std::same_as<unsigned char&>;
};


class Interpreter
{
public:
  Interpreter(const Display& display, const Keyboard& keyboard, unsigned memory_size = 30'000)
    : _memory(memory_size, '\0')
    , _display(display)
    , _keyboard(keyboard)
  {
  }

  virtual ~Interpreter() = default;

  template<StringLike T>
  void run(const T& program) noexcept
  {
    size_t size;
    if constexpr (std::is_same_v<std::decay_t<T>, char*>)
      size = sizeof(program) - 1;
    else
      size = program.size();
    
    std::vector<char>::iterator it = _memory.begin();
    size_t _PC = 0;

    while (_PC < size) {
      const char op = program[_PC];
      switch (op) {
        case '>':
          ++it;
          break;
        case '<':
          --it;
          break;
        case '+':
          ++*it;
          break;
        case '-':
          --*it;
          break;
        case '.':
          _display.write(*it);
          break;
        case ',':
          *it = _keyboard.read();
          break;
        case '[':
          if (*it == 0) {
            unsigned stack = 1;
            while (stack) {
              stack += program[++_PC] == '[';
              stack -= program[_PC] == ']';
            }
          }
          break;
        case ']':
          if (*it != 0) {
            unsigned stack = 1;
            while (stack) {
              stack += program[--_PC] == ']';
              stack -= program[_PC] == '[';
            }
          }
          break;
      }
      _PC++;
    }
  }

  inline const std::vector<char>& memory() const noexcept {
    return _memory;
  }

private:
  std::vector<char> _memory;

  const Display& _display;
  const Keyboard& _keyboard;
};


#endif // INTERPRETER_H_
