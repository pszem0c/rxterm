#ifndef RXTERM_COMPONENT_WINDOW_HPP
#define RXTERM_COMPONENT_WINDOW_HPP
#include <algorithm>
#include <tuple>
#include <rxterm/image.hpp>
#include <rxterm/components/component.hpp>
#include <rxterm/components/maxwidth.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/style.hpp>
#include <rxterm/utils.hpp>

namespace rxterm {

template<class T = Component>
struct Window {
    std::vector<std::tuple<T, unsigned, unsigned> > children;
    Pixel bg;
    unsigned width;
    unsigned height;
    std::string title;

    Window(std::string title,
        unsigned width,
        unsigned height)
        : children(),
        bg{Pixel{}},
        width(width),
        height(height),
        title(title)
        {}

    template<class...Xs>
    Window(std::string title,
        unsigned width,
        unsigned height,
        Xs const&...xs)
        : children{xs...},
        bg{Pixel{}},
        width(width),
        height(height),
        title(title)
        {}

    Image render(unsigned const maxWidth) const {
        auto canvas = Image::create(width, height, bg);
        canvas(0, 0).c = "\u250F";
        canvas(width-1, 0).c = "\u2513";
        canvas(0, height-1).c = "\u2517";
        canvas(width-1, height-1).c = "\u251B";
        
        unsigned titleStart = std::max((int)(width/2 - title.size()/2),1);
        unsigned titleEnd = std::min((int)(width/2 + title.size()/2),(int)width-2);

        for (unsigned x = 1; x < width-1; ++x) {
            if (x >= titleStart && x <= titleEnd && titleStart != titleEnd) {
                canvas(x, 0).c = title[x - titleStart]; 
            } else {
                canvas(x, 0).c = "\u2501";
            }
            canvas(x, height-1).c = "\u2501";
        }

        for (unsigned y = 1; y < height-1; ++y) {
            canvas(0, y).c = "\u2503";
            canvas(width-1, y).c = "\u2503";
        }

        for (auto& c : children) {
                auto x = std::get<1>(c);
                auto y = std::get<2>(c);
                auto content = std::get<0>(c).render(width-4-x);
                if (content.height + y + 1 < height) {
                    canvas = drawOnBackground(canvas, 2+x, 1+y, content);
                }
        }
        return canvas;
        //auto content = StackLayout<>(children).render(width-4);
        //return drawOnBackground(canvas,2,1,content);
    }
};

}
#endif
