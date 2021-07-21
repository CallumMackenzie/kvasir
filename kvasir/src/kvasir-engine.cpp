#include <iostream>
#include "kvasir-engine.h"

using namespace galg;

namespace kvasir
{
    void say_hello()
    {
        std::cout << "Hello, from kvasir-engine!\n";
    }

    void test_window_prog()
    {
        kvasir_engine kvs(renderer_base_type::OPENGL);
        kvasir_engine::result res = kvs.start();
        if (res != kvasir_engine::NO_ERROR)
            std::cerr << "Kvasir engine crashed with code " << res << std::endl;
    }

    mat4f get_mat()
    {
        return mat4f::identity();
    }
}