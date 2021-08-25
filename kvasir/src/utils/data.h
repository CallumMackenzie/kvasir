#ifndef KVASIR_DATA_H
#define KVASIR_DATA_H 1

#define GL_SHADER_ARR(NAMESPACE)                                                                         \
	{                                                                                                    \
		data::opengl_shaders::NAMESPACE ::get_vertex(), data::opengl_shaders::NAMESPACE ::get_fragment() \
	}

namespace kvasir
{
	namespace data
	{
		namespace objects3d
		{
			const char *get_cube_obj();
			const char *get_rect_prism_obj();
		}

		namespace opengl_shaders
		{
			namespace diffuse3d
			{
				const char *get_vertex();
				const char *get_fragment();
			}
			namespace diffuse2d
			{
				const char *get_vertex();
				const char *get_fragment();
			}
		}
	}
}

#endif