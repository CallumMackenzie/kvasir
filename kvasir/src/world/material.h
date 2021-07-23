#ifndef KVASIR_MATERIAL_H_
#define KVASIR_MATERIAL_H_ 1

#ifdef _WIN32
typedef unsigned int uint;
#endif

#ifndef NO_USE_INCLUDES
#include "texture.h"
#include <vector>
#endif

namespace kvasir
{
	struct material_base
	{
		std::vector<texture_base *> texs{nullptr, nullptr, nullptr, nullptr};
		texture_base *get_diffuse();
		texture_base *get_specular();
		texture_base *get_normal();
		texture_base *get_parallax();
		void bind();
	};
}

#endif