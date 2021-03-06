#ifndef KVASIR_SHADER__H_
#define KVASIR_SHADER__H_ 1

#ifndef NO_USE_INCLUDES
#include "config.h"
#include "material.h"
#endif

namespace kvasir
{

	struct KV_EXPORT shader_base
	{
		virtual ~shader_base() = 0;
		virtual void use() = 0;
		virtual bool compile(const char **srcs, size_t n_srcs) = 0;
		virtual void u_bool1(const char *name, bool a) = 0;
		virtual void u_bool2(const char *name, bool a, bool b) = 0;
		virtual void u_bool3(const char *name, bool a, bool b, bool c) = 0;
		virtual void u_bool4(const char *name, bool a, bool b, bool c, bool d) = 0;
		virtual void u_int1(const char *name, int a) = 0;
		virtual void u_int2(const char *name, int a, int b) = 0;
		virtual void u_int3(const char *name, int a, int b, int c) = 0;
		virtual void u_int4(const char *name, int a, int b, int c, int d) = 0;
		virtual void u_float1(const char *name, float a) = 0;
		virtual void u_float2(const char *name, float a, float b) = 0;
		virtual void u_float3(const char *name, float a, float b, float c) = 0;
		virtual void u_float4(const char *name, float a, float b, float c, float d) = 0;
		virtual void u_mat2f(const char *name, float m[2][2]) = 0;
		virtual void u_mat3f(const char *name, float m[3][3]) = 0;
		virtual void u_mat4f(const char *name, float m[4][4]) = 0;
		virtual void render(int) = 0;
		virtual void free_shader() = 0;

		void bind_material(material_base *base);
	};

}

#endif