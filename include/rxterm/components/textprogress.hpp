#ifndef RXTERM_COMPONENTS_TEXT_PROGRESS_HPP
#define RXTERM_COMPONENTS_TEXT_PROGRESS_HPP
#include <rxterm/image.hpp>
#include <rxterm/components/progress.hpp>
#include <rxterm/components/flowlayout.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>

namespace rxterm {

struct TextProgress {
  float const progress;
  std::string const text;

  TextProgress(std::string text,
    float p)
    : progress{p},
    text{text}
  {}

  Image render(unsigned const width) const {
    unsigned progressWidth = width - text.size() - 2 - 2 - 3 - 1;
    auto percents = clamp(0, 100, (int)(progress*100));
    return FlowLayout<>{
        Text(text),
        Text(" ["),
        MaxWidth(progressWidth, Progress(progress)),
        Text(Style{Color::Transparent}, "] " + std::to_string(percents) + "%"),
    }.render(width);
  }
};

}


#endif
