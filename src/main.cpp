#include <array>
#include <chrono>
#include <functional>
#include <memory>
#include <vector>
#include <deque>
#include <ftxui/dom/elements.hpp>  
#include <ftxui/screen/screen.hpp>
#include <string>
#include <thread>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Checkbox, Renderer, Horizontal, Vertical, Menu, Radiobox, Tab, Toggle
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/event.hpp"              // for Event, Event::Custom
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/screen/color.hpp"  
#include "ftxui/screen/box.hpp"  
#include "ftxui/dom/node.hpp"

#include <syslog.h>

int main(int argc, const char *argv[]) {

    using namespace ftxui;
    std::deque<std::string> entries{};

    auto input_option = InputOption();
    std::string input_add_content;
    input_option.on_enter = [&] {
        if (entries.size() > 25) {
            entries.pop_front();
        }
        entries.push_back(input_add_content);
        input_add_content = "";
        syslog(LOG_ERR, "After enter: %s\n", input_add_content.c_str());
    };
    Component input = Input(&input_add_content, "", input_option);

    auto renderInput = [&](const std::string &line) {
        return hbox({
                text("Line:"),
                separator(),
                text(line),
                });
    };

    auto text_render = [&] {
        Elements elements;
        for (auto &entry : entries) {
            elements.push_back(renderInput(entry));
        }
        return elements;
    };

    auto display_component = Container::Vertical({
        input});


    auto renderer = Renderer(display_component, [&] {
        auto input_win = window(text("Input: "), input->Render());
        syslog(LOG_ERR, "WTF %s\n", input_add_content.c_str());
        return vbox({
                    vbox(text_render()) | frame | size(HEIGHT, EQUAL, 25) | border, 
                    input_win});
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer);

    return 0;
}
