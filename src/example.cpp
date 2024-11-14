#include <iostream>
#include <chrono>
#include <lib/dawn_wrapper/src/dawn_wrapper.h>
#include <lib/glfw_wrapper/src/glfw_wrapper.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include <cassert>
#define ASSERT(p) cassert((p))

#ifndef __EMSCRIPTEN__
static void setup_surface(dawn_wrapper::render_wrapper renderer, glfw_wrapper::Window window, bool opaque = true)
{
    ASSERT(renderer);
    int w = 0, h = 0;
    window.get_window_size(w, h);
    renderer.setup_surface(window.impl(), w, h, opaque);
}
#endif

struct States {
    dawn_wrapper::dawn_plugin m_dawn;
    bool m_is_done = false;
};

int main()
{
    using namespace std;

    auto tick_mark = std::chrono::system_clock::now();

#ifndef __EMSCRIPTEN__
    dawn_wrapper::dawn_plugin dawn;
    glfw_wrapper::init_glfw(std::cout);
    auto window = glfw_wrapper::Window::make_window(1200, 800, false, true, "wgui");

    auto renderer = dawn.make_render();

    setup_surface(renderer, window, false);

    while (true) {
        auto next_tick_mark = std::chrono::system_clock::now();
        chrono::duration<float_t> diff(next_tick_mark - tick_mark);
        glfw_wrapper::poll_events();

        tick_mark = next_tick_mark;

        std::cout << diff.count() << std::endl;
    }
    glfw_wrapper::terminate();
#else
    States states;
    emscripten_set_main_loop_arg(
        [](void* userData) {
            States * states = reinterpret_cast<States *>(userData);
            if (states->m_dawn && !states->m_is_done) {
                states->m_is_done = true;
                auto renderer = states->m_dawn.make_render();

                renderer.setup_surface_html_canvas("#canvas", 500, 500);
            }
        },
        & states,
        0, true);
#endif

     // if (!instance.update(std::min(0.05f, diff.count()))) {
     // instance.release();
     //   break;
     // }
     //

  return 0;
}
