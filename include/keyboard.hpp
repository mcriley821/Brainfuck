#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include <iostream>


struct Keyboard
{
  virtual char read() const noexcept = 0; 
};


struct StdKeyboard : public Keyboard
{
  char read() const noexcept override
  {
    char retval;
    std::cin.get(retval);
    return retval;
  }
};


#endif // KEYBOARD_H_
