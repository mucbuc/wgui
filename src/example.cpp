#include <iostream>
#include <chrono>
#include <lib/dawn_wrapper/src/dawn_wrapper.h>
#include <lib/glfw_wrapper/src/glfw_wrapper.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include <cassert>
#define ASSERT(p) assert((p))

static void setup_surface(dawn_wrapper::render_wrapper renderer, glfw_wrapper::Window window, bool opaque = true)
{
    ASSERT(renderer);
    ASSERT(window.impl());
    int w = 0, h = 0;
    window.get_window_size(w, h);
    renderer.setup_surface(window.impl(), w, h, opaque);
}

struct States {
    dawn_wrapper::dawn_plugin m_dawn;
    dawn_wrapper::render_wrapper m_render;
    dawn_wrapper::encoder_wrapper m_encoder;

    void init()
    {
        m_render = m_dawn.make_render();
        const auto script = R"(@fragment
        fn fragmentMain(@builtin(position) coords: vec4f) -> @location(0) vec4f {
            return select(vec4f(0, 1, 1, 1), vec4f( 1, 1, 0, 1), coords.x + coords.y < 100);
        }
        )";
        m_render.compile_shader(script, "fragmentMain");
        m_render.init_pipeline();
        m_encoder = m_dawn.make_encoder();
    }
};

int main()
{
    using namespace std;

    auto tick_mark = std::chrono::system_clock::now();
    States states;
#ifndef __EMSCRIPTEN__
    states.init();

    glfw_wrapper::init_glfw(std::cout);
    auto window = glfw_wrapper::Window::make_window(1200, 800, false, true, "wgui");

    setup_surface(states.m_render, window, false);
    while (true) {
        auto next_tick_mark = std::chrono::system_clock::now();
        chrono::duration<float_t> diff(next_tick_mark - tick_mark);
        glfw_wrapper::poll_events();

        tick_mark = next_tick_mark;
       // std::cout << diff.count() << std::endl;
        states.m_render.render(states.m_encoder);
        states.m_dawn.run();
    }
    glfw_wrapper::terminate();
#else
    emscripten_set_main_loop_arg(
        [](void* userData) {
            States * states = reinterpret_cast<States *>(userData);
            if (states->m_dawn) {
                if (!states->m_render) {
                    states->init();
                    states->m_render.setup_surface_html_canvas("#canvas", 1200, 800);
                }

                states->m_render.render(states->m_encoder);
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
