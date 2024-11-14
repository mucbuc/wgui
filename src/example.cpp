#include <iostream>
#include <chrono>
#include <lib/dawn_wrapper/src/dawn_wrapper.h>
#include <lib/glfw_wrapper/src/glfw_wrapper.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

int main()
{
    using namespace std;

    dawn_wrapper::dawn_plugin dawn;

  auto c = dawn.make_compute();

  auto tick_mark = std::chrono::system_clock::now();

#ifndef __EMSCRIPTEN__
  glfw_wrapper::init_glfw(std::cout);
  auto window = glfw_wrapper::Window::make_window(1200, 800, false, true, "wgui");

  while (true) {
      auto next_tick_mark = std::chrono::system_clock::now();
      chrono::duration<float_t> diff(next_tick_mark - tick_mark);
      glfw_wrapper::poll_events();

      tick_mark = next_tick_mark;

      std::cout << diff.count() << std::endl;
  }
  glfw_wrapper::terminate();
#else
    emscripten_set_main_loop_arg(
        [](void* userData) {
            std::cout << "a " << "\n";
        },
        nullptr,
        0, true);
#endif

     // if (!instance.update(std::min(0.05f, diff.count()))) {
     // instance.release();
     //   break;
     // }
     //

  return 0;
}
