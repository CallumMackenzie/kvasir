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
		~material_base();
		std::vector<texture_base *> texs{nullptr, nullptr, nullptr, nullptr};
		texture_base *&diffuse();
		texture_base *&specular();
		texture_base *&normal();
		texture_base *&parallax();
		texture_base *&operator[](size_t index);
		void bind();
	};
}

#endif