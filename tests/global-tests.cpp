
#include <iostream>
#include "kvasir-engine.h"

COUNT_MEMORY

int main(int, char **)
{
	{
		kvasir::say_hello();
		kvasir::test_window_prog();
		mat4f id = kvasir::get_mat();
		std::cout << to_string(id) << std::endl;
	}

	PRINT_MEMORY_SUMMARY
}