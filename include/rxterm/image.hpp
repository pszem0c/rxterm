#ifndef RXTERM_IMAGE_HPP
#define RXTERM_IMAGE_HPP

#include <string>
#include <vector>

#include <rxterm/pixel.hpp>
#include <rxterm/style.hpp>

namespace rxterm {

struct Image {

  std::vector<Pixel> pixels;
  unsigned width;
  unsigned height;

  Image(
    std::vector<Pixel> const& pixels,
    unsigned const width,
    unsigned const height)
    : pixels{pixels}
    , width{width}
    , height{height}
  {}

  Image() = default;
  Image(Image const&) = default;
  Image& operator=(Image const&) = default;


  static Image create(unsigned const& width, unsigned const& height, Pixel const& pixel = Pixel{"", Style::None()}) {
    return {
      std::vector<Pixel>(width*height, pixel),
      width,
      height
    };
  }



  Pixel const& operator()(int x, int y)const {
    return pixels[y*width+x];
  }


  Pixel& operator()(int x, int y) {
    return pixels[y*width+x];
  }


  std::string toString()const {
    std::string str = "";

    auto prev = Style{};
    for (unsigned y=0; y < height; ++y) {
      for (unsigned x=0; x < width; ++x) {
        auto const& pixel = (*this)(x, y);
        auto const current = diff(prev, !pixel.c.empty() ? pixel.style : Style::Default());
        std::string c = !pixel.c.empty() ? pixel.c : " ";
        str += current.toString() + c;
        prev = !pixel.c.empty() ? pixel.style : Style::Default();
      }
      str+="\n";
    }
    return str;
  }
};

Image drawOnBackground(Image canvas, unsigned const& sx, unsigned const& sy, Image const& fg) {
  for(unsigned y=0; y < fg.height; ++y) {
    for(unsigned x=0; x < fg.width; ++x) {
      auto& p = canvas(sx+x, sy+y);
      auto const& q = fg(x, y);
      p = Pixel{
        (!q.c.empty())? q.c : p.c,
        Style{
          q.style.fg,
          (!q.c.empty())? q.style.bg : p.style.bg,
          q.style.font
        }
      };
    }
  }
  return canvas;
}

}


#endif
