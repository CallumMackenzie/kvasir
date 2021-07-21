#include <iostream>
#include "opengl.h"
#include "galg.h"

using namespace galg;

namespace kvasir
{
    void say_hello()
    {
        std::cout << "Hello, from kvasir-engine!\n";
    }

    void test_window_prog()
    {
        gl_window window;
        if (!gl_window::init_ok(window.init()))
            return;
        window.set_clear_colour(0xff00ff);
        while (!window.should_close())
        {
            window.poll_events();
            window.clear();
            window.swap_buffers();
        }
    }

    mat4f get_mat()
    {
        return mat4f::identity();
    }
}