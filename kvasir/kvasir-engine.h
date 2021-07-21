#ifndef KVASIR_ENGINE_INCLUDE_H
#define KVASIR_ENGINE_INCLUDE_H 1

#include "./lib/galg/galg.h"
#include "./lib/galg/galg_ops.h"
#include "./src/utils/memory-aid.h"

using namespace galg;

namespace kvasir
{
	void say_hello();
	void test_window_prog();
	mat4f get_mat();
};

#endif