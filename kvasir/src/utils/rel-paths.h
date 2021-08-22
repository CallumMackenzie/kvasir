#ifndef REL_PATHS_H_
#define REL_PATHS_H_ 1

#ifndef NO_USE_INCLUDES
#include "config.h"
#endif

#include <string>
#ifdef _WIN32
#define R_PATH(str) \
	std::string("../").append(str)
#else
#define R_PATH(str) \
	return std::string(str)
#endif

#define RESOURCE(X) \
	R_PATH(X).c_str()

#endif