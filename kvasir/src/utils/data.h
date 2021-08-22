#ifndef KVASIR_DATA_H
#define KVASIR_DATA_H 1

#define GL_SHADER_ARR(NAMESPACE)                                                             \
	{                                                                                        \
		data::opengl_shaders::NAMESPACE ::vertex, data::opengl_shaders::NAMESPACE ::fragment \
	}

namespace kvasir
{
	namespace data
	{
		namespace objects3d
		{
			const char *cube_obj = "v -1.0 1.0 -1.0\nv 1.000 1.0 1.0\nv 1.000 1.000 -1.000\nv -1.000 -1.000 1.000\nv 1.000 -1.000 1.000\nv -1.000 1.000 1.000\nv -1.000 -1.000 -1.000\nv 1.000 -1.000 -1.000\nvt 1.000 0.000000\nvt 0.666667 0.333333\nvt 0.666667 0.000000\nvt 0.333333 0.333333\nvt 0.000000 0.000000\nvt 0.333333 0.000000\nvt 0.333333 0.666667\nvt 0.000000 0.333333\nvt 0.333333 0.333333\nvt 0.666667 0.000000\nvt 0.333333 0.000000\nvt 0.666667 0.666667\nvt 0.333333 0.333333\nvt 0.666667 0.333333\nvt 0.333333 1.000\nvt 0.000000 0.666667\nvt 0.333333 0.666667\nvt 1.000 0.333333\nvt 0.000000 0.333333\nvt 0.000000 0.666667\nvt 0.666667 0.333333\nvt 0.333333 0.666667\nvt 0.000000 1.000\nvn 0.0000 1.0 0.0000\nvn 0.0000 -0.0000 1.0\nvn -1.0 0.0000 0.0000\nvn 0.0000 -1.0 -0.0000\nvn 1.0 0.0000 0.0000\nvn 0.0000 0.0000 -1.0\ns off\nf 1/1/1 2/2/1 3/3/1\nf 2/4/2 4/5/2 5/6/2\nf 6/7/3 7/8/3 4/9/3\nf 8/10/4 4/9/4 7/11/4\nf 3/12/5 5/13/5 8/14/5\nf 1/15/6 8/16/6 7/17/6\nf 1/1/1 6/18/1 2/2/1\nf 2/4/2 6/19/2 4/5/2\nf 6/7/3 1/20/3 7/8/3\nf 8/10/4 5/21/4 4/9/4\nf 3/12/5 2/22/5 5/13/5\nf 1/15/6 3/23/6 8/16/6\n";
			const char *rect_prism_obj = "v -1.000000 0.000000 1.000000\nv -1.000000 0.000000 -1.000000\nv 1.000000 0.000000 1.000000\nv 1.000000 0.000000 -1.000000\nv 0.000000 1.100000 0.000000\nvt 0.375000 0.000000\nvt 0.625000 0.250000\nvt 0.375000 0.250000\nvt 0.625000 0.500000\nvt 0.375000 0.500000\nvt 0.625000 0.750000\nvt 0.375000 0.750000\nvt 0.625000 1.000000\nvt 0.375000 1.000000\nvt 0.125000 0.750000\nvt 0.125000 0.500000\nvn -0.7399 0.6727 0.0000\nvn 0.0000 0.6727 -0.7399\nvn 0.7399 0.6727 0.0000\nvn 0.0000 0.6727 0.7399\nvn 0.0000 -1.0000 0.0000\ns off\nf 1/1/1 5/2/1 2/3/1\nf 2/3/2 5/4/2 4/5/2\nf 4/5/3 5/6/3 3/7/3\nf 3/7/4 5/8/4 1/9/4\nf 4/5/5 1/10/5 2/11/5\nf 4/5/5 3/7/5 1/10/5";
		}

		namespace opengl_shaders
		{
			namespace diffuse3d
			{
				const char *vertex = "#version 330 core\nlayout(location=0) in vec3 v_pos;layout (location = 1) in vec2 v_uv;layout (location = 2) in vec3 v_normal;out vec3 normal;out vec2 uv;uniform mat4 transform;uniform mat4 rot;uniform mat4 view;uniform mat4 projection;void main(){gl_Position=projection*view*transform*vec4(v_pos, 1.0);normal=(rot*vec4(v_normal, 1.0)).xyz;uv=v_uv;}\n";
				const char *fragment = "#version 330 core\nlayout(location=0) out vec4 col;in vec3 normal;in vec2 uv;uniform sampler2D diff;void main(){float d=(dot(normal, normalize(vec3(1.0, 1.5, 0.2))) + 1.0) / 2.0;col=vec4(texture(diff, uv).rgb * d, 1.0);}\n";
			}
			namespace diffuse2d
			{
				const char *vertex = "#version 330 core\nlayout(location=0) in vec2 vpos;layout(location=1) in vec2 vuv;uniform float aspect;uniform vec2 pos;uniform mat2 transform;out vec2 uv; void main() {uv = vuv;vec4 tx=vec4(((vpos * transform) + pos), 0.0, 1.0);tx.x *= aspect;gl_Position=tx;}\n";
				const char *fragment = "#version 330 core\nlayout(location=0) out vec4 col; in vec2 uv; uniform sampler2D diff;void main() {col=texture(diff, uv).rgba;}\n";
			}
		}
	}
}

#endif