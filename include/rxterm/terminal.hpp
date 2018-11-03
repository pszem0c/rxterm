#ifndef RXTERM_TERMINAL_HPP
#define RXTERM_TERMINAL_HPP


#include <iostream>
#include <algorithm>
#include <string>
#include <rxterm/utils.hpp>

#ifdef __linux__
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif
namespace rxterm {

struct VirtualTerminal {
  std::string buffer;
  unsigned width;
  unsigned height;

  VirtualTerminal() : buffer() {
    clear();
    #ifdef __linux__
      winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      width = w.ws_col;
      height = w.ws_row;
      system("setterm -cursor off");
    #else
      width = 80;
      heighth = 80;
    #endif
  }
  
  VirtualTerminal(std::string buffer,
    unsigned width, 
    unsigned height)
    :  buffer(buffer),
    width(width),
    height(height)
    {}

  std::string computeTransition(std::string const& next) const {
    if(buffer == next) return "";
    unsigned const n = std::count(buffer.begin(), buffer.end(), '\n');
    return clearLines(n) + "\e[0m" + next;
  }

  static std::string hide() { return "\e[0;8m"; }

  void clear() { std::cout << clearScreen(); }

  VirtualTerminal flip(std::string const& next) const {
    auto const& transition = computeTransition(next);
    if(transition == "") return *this;
    std::cout << transition << hide();
    std::flush(std::cout);
    return VirtualTerminal(next, width, height);
  }
};

}

#endif
