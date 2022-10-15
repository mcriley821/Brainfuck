#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <iostream>


struct Display
{
  virtual void write(char chr) const noexcept = 0;
};


struct StdDisplay : public Display
{
  void write(char chr) const noexcept override
  {
    std::cout.put(chr);
  }
};


#endif
