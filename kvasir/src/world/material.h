#ifndef KVASIR_MATERIAL_H_
#define KVASIR_MATERIAL_H_ 1

#ifdef _WIN32
typedef unsigned int uint;
#endif

#include <vector>
#ifndef NO_USE_INCLUDES
#include "config.h"
#include "galg-dllexport.h"
#include "texture.h"
#include "memory-aid.h"
#endif

namespace kvasir
{
	struct KV_EXPORT material_base
	{
		std::vector<texture_base *> texs{nullptr, nullptr, nullptr, nullptr};

		~material_base();
		texture_base *&diffuse();
		texture_base *&specular();
		texture_base *&normal();
		texture_base *&parallax();
		texture_base *&operator[](size_t index);
		void bind();
		std::vector<texture_base *> &get_texs();
	};
}

#endif