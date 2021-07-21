#include <iostream>
#include "opengl.h"
#include "galg.h"
#include "fps-manager.h"

using namespace galg;

namespace kvasir
{
    void say_hello()
    {
        std::cout << "Hello, from kvasir-engine!\n";
    }

    void test_window_prog()
    {
        frame_manager f_manager;
        f_manager.set_fps(20);
        gl_window window;
        if (!gl_window::init_ok(window.init()))
            return;
        window.set_clear_colour(0xff00ff);
        for (;;)
        {
            if (f_manager.next_frame_ready())
            {
                if (window.should_close())
                    break;
                window.poll_events();
                window.clear();
                window.swap_buffers();
            }
        }
    }

    mat4f get_mat()
    {
        return mat4f::identity();
    }
}