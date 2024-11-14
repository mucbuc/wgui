#include <iostream>
#include <chrono>
#include <lib/dawn_wrapper/src/dawn_wrapper.h>
#include <lib/glfw_wrapper/src/glfw_wrapper.h>

int main()
{
    using namespace std;

    dawn_wrapper::dawn_plugin dawn;

  auto c = dawn.make_compute();

  glfw_wrapper::init_glfw(std::cout);

  auto tick_mark = std::chrono::system_clock::now();

  auto window = glfw_wrapper::Window::make_window(1200, 800, false, true, "wgui");

  while (true) {

      auto next_tick_mark = std::chrono::system_clock::now();
      chrono::duration<float_t> diff(next_tick_mark - tick_mark);
     // if (!instance.update(std::min(0.05f, diff.count()))) {
     // instance.release();
     //   break;
     // }
     //
      glfw_wrapper::poll_events();
      tick_mark = next_tick_mark;
  }

  glfw_wrapper::terminate();
  return 0;
}
