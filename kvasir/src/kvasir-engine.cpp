#include <iostream>
#include "kvasir-engine.h"

const char *vshader = "#version 330 core\nlayout (location = 0) in vec3 v_pos;uniform mat4 transform;uniform mat4 view;uniform mat4 projection;void main(){gl_Position=projection*view*transform*vec4(v_pos, 1.0);}\n";
const char *fshader = "#version 330 core\nlayout (location = 0) out vec4 col;void main(){col=vec4(0.0, 1.0, 0.3, 1.0);}\n";

namespace kvasir
{
    struct test_prog : kvasir_engine
    {
        test_prog() : kvasir_engine(renderer_base_type::OPENGL) {}

        camera3d cam;
        mesh3d mesh;
        shader_base *shader = nullptr;

        void on_start()
        {
            cam.aspect = (float)base->get_aspect();
            base->set_clear_colour(0xff80ff);

            mesh.load_from_obj("../res/models/cube.obj");
            mesh.load_to_buffer(base->make_buffer());
            mesh.pos.z() = 4;

            shader = base->make_shader();
            const char *s[2]{vshader, fshader};
            shader->compile(s, 2);
        }
        void on_update()
        {
            mesh.rot += vec3f{1.f, 1.f, 1.f} * (float)time.delta();

            base->clear();
            base->render_mesh3d(cam, mesh, shader);
            base->swap_buffers();
        }
        void on_close()
        {
        }
    };

    void say_hello()
    {
        std::cout << "Hello, from kvasir-engine!\n";
    }

    void test_window_prog()
    {
        test_prog kvs;
        kvasir_engine::result res = kvs.start();
        if (res != kvasir_engine::NO_ERROR)
            std::cerr << "Kvasir engine crashed with code " << res << std::endl;
    }

    mat4f get_mat()
    {
        return mat4f::identity();
    }
}