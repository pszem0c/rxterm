#include <string>
#include <thread>
#include <chrono>
#include <memory>

#include <tuple>
#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>
#include <rxterm/reflow.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/flowlayout.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/progress.hpp>
#include <rxterm/components/maxwidth.hpp>
#include <rxterm/components/textprogress.hpp>
#include <rxterm/components/window.hpp>

using namespace rxterm;

auto renderToTerm = [](auto const& vt, Component const& c) {
  // TODO: get actual terminal width
  return vt.flip(c.render(vt.width).toString());
};

int main() {

  using namespace std::chrono_literals;
  using namespace std::string_literals;

  VirtualTerminal vt;

  /*auto superProgressBar = [](auto x, auto y, auto z) -> Window<> {
    return Window<>{100, 10,        
        TextProgress("test1",x),
        TextProgress("test2",y),
        TextProgress("test3",z)
    };
  };*/

  auto view = [](auto x) -> Window<> {
    return Window<>{"title",60, 10,
        std::make_tuple(FlowLayout<>{
          Window<>("",20,7),Window<>("",20,7)
        }, 11, 0)
    };
  };



  for (int i = 0; i < 101; ++i) {
    vt.render(view(0.01*i));
    std::this_thread::sleep_for(200ms);
  }

 // vt = renderToTerm(vt, Window<>(50,10));

  return 0;
}
