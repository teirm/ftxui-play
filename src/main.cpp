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
#include <mutex>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Checkbox, Renderer, Horizontal, Vertical, Menu, Radiobox, Tab, Toggle
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/event.hpp"              // for Event, Event::Custom
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/screen/color.hpp"  
#include "ftxui/screen/box.hpp"  
#include "ftxui/dom/node.hpp"

int main(int argc, const char *argv[]) {
    using namespace ftxui;

    struct user {
        std::string name;
        Color name_color;
        std::string input;
    };

    std::deque<user> entries{};
    std::mutex deque_lock;
    auto input_option = InputOption();
    user main_user{"Tofumimikyu:", Color::Green, {}};
    std::string input_add_content;
    input_option.on_enter = [&] {
        main_user.input = input_add_content;
        deque_lock.lock();
        if (entries.size() > 19) {
            entries.pop_front();
        }
        entries.push_back(main_user);
        deque_lock.unlock();
        input_add_content = "";
    };
    Component input = Input(&input_add_content, "", input_option);

    auto renderInput = [&](const user u) {
        return hbox({
                text(u.name) | color(u.name_color),
                separator(),
                text(u.input),
                });
    };

    auto main_renderer = Renderer(input, [&] {
        auto input_win = window(text("Input: "), 
                            hbox({input->Render()})|inverted);

        
        Elements elements;
        for (auto &entry : entries) {
            elements.push_back(renderInput(entry));
        }

        return vbox({
                    vbox(std::move(elements)) | frame | size(HEIGHT, EQUAL, 20) | border, 
                    input_win });
    });

    auto screen = ScreenInteractive::Fullscreen();
    
    bool run_maru = true;
    user maru_user{"Maru:", Color::BlueViolet, {}};
    std::array<std::string, 3> maru_lines{"Nyan", "Meow", "Fffft"};
    std::thread maru([&] {
        int counter = 0;
        while (run_maru) {
            maru_user.input = maru_lines[counter];          
            counter = (counter + 1) % 3;
            deque_lock.lock();
            if (entries.size() > 19) {
                entries.pop_front();
            }
            entries.push_back(maru_user);
            deque_lock.unlock();
            
            using namespace std::chrono_literals;
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(1s);
        }
    });

    screen.Loop(main_renderer);
    
    run_maru = false;
    maru.join();

    return 0;
}
