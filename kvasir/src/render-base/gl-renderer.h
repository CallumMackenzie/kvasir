#ifndef KVASIR_GL_RENDERER_H
#define KVASIR_GL_RENDERER_H 1

#include "renderer.h"

namespace kvasir
{

	struct gl_shader_base : shader_base
	{

		bool compile(const char **srcs, size_t n_srcs)
		{
			if (n_srcs != 2)
				return false;
			return s.init(srcs[0], srcs[1]);
		}
		void use() { s.use(); }
		void u_bool1(const char *n, bool a) { s.bool1(n, a); }
		void u_bool2(const char *n, bool a, bool b) { s.bool2(n, a, b); }
		void u_bool3(const char *n, bool a, bool b, bool c) { s.bool3(n, a, b, c); }
		void u_bool4(const char *n, bool a, bool b, bool c, bool d) { s.bool4(n, a, b, c, d); }
		void u_int1(const char *n, int a) { s.int1(n, a); }
		void u_int2(const char *n, int a, int b) { s.int2(n, a, b); }
		void u_int3(const char *n, int a, int b, int c) { s.int3(n, a, b, c); }
		void u_int4(const char *n, int a, int b, int c, int d) { s.int4(n, a, b, c, d); }
		void u_float1(const char *n, float a) { s.float1(n, a); }
		void u_float2(const char *n, float a, float b) { s.float2(n, a, b); }
		void u_float3(const char *n, float a, float b, float c) { s.float3(n, a, b, c); }
		void u_float4(const char *n, float a, float b, float c, float d) { s.float4(n, a, b, c, d); }
		void u_mat2f(const char *n, float m[2][2]) { s.mat2f(n, m); }
		void u_mat3f(const char *n, float m[3][3]) { s.mat3f(n, m); }
		void u_mat4f(const char *n, float m[4][4]) { s.mat4f(n, m); }
		void render(int n_tris) { glDrawArrays(GL_TRIANGLES, 0, n_tris * 3); };

	private:
		shader_vf s;
	};

	struct gl_buffer_base : buffer_base
	{
		void gen_buffer()
		{
			glGenBuffers(1, &vbo);
			glGenVertexArrays(1, &vao);
		}
		void set_data(void *data, size_t len, bool change_often)
		{
			bind_buffer();
			glBufferData(GL_ARRAY_BUFFER, len, data, change_often ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		void sub_data(void *data, size_t offset, size_t len)
		{
			bind_buffer();
			glBufferSubData(GL_ARRAY_BUFFER, offset, len, data);
		}
		void get_data(void *recv, size_t offset, size_t len)
		{
			bind_buffer();
			glGetBufferSubData(GL_ARRAY_BUFFER, offset, len, recv);
		}
		void attrib_ptr(size_t ptr_num, size_t size, size_t step, size_t offset)
		{
			bind_vao();
			bind_buffer();
			glEnableVertexAttribArray(ptr_num);
			glVertexAttribPointer(ptr_num, size, GL_FLOAT, GL_FALSE, step, (void *)offset);
		}
		void bind_buffer() { glBindBuffer(GL_ARRAY_BUFFER, vbo); }
		void bind_vao() { glBindVertexArray(vao); }

	private:
		uint vbo;
		uint vao;
	};

	struct gl_render_base : renderer_base
	{
		bool should_close() { return win.should_close(); }
		void set_clear_colour(long colour) { win.set_clear_colour(colour); }
		void clear() { win.clear(); }
		void swap_buffers() { win.swap_buffers(); }
		void poll_events() { win.poll_events(); }
		void set_position(int x, int y) { win.set_pos(x, y); }
		void get_position(int *x, int *y) { win.get_pos(x, y); }
		void get_size(int *w, int *h) { win.get_size(w, h); }
		void set_size(int w, int h) { win.set_size(w, h); }
		void set_title(const char *name) { win.set_title(name); }
		bool is_fullscreen() { return win.is_fullscreen(); }
		void set_resizable(bool res) { win.set_resizable(res); }
		bool is_resizable() { return win.is_resizable(); }
		bool set_fullscreen() { return win.set_fullscreen(); }
		bool set_windowed() { return win.set_windowed(); }
		bool set_visible(bool b) { return win.set_visible(b); };
		bool is_visible() { return win.is_visible(); }
		bool init(const char *name, int w, int h) { return gl_window::init_ok(win.init(name, w, h)); }
		buffer_base *make_buffer() { return new gl_buffer_base(); }
		shader_base *make_shader() { return new gl_shader_base(); }
		void destroy() { win.destroy(); }

		void render_mesh3d(camera3d &cam, mesh3d &mesh, shader_base *sh)
		{
			if (!sh || !mesh.buffer)
				return;
			mat4f m_projection = cam.perspective(),
				  m_rotation = mat4f::rotation(mesh.rot),
				  m_scale = mat4f::scale(mesh.scale),
				  m_translation = mat4f::translation(mesh.pos),
				  m_view = cam.view().inverse();

			sh->use();
			mesh.buffer->bind_vao();
			sh->u_mat4f("transform", (m_scale * m_rotation * m_translation).m);
			sh->u_mat4f("view", m_view.m);
			sh->u_mat4f("projection", m_projection.m);
			sh->render(mesh.tris.size());
		}

	private:
		gl_window win;
	};

};

#endif