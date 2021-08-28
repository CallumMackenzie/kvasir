
#include "krc-packer.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>

using namespace kvasir;

int main(int, char **)
{
	packer::obj_data_to_krc(data::objects3d::get_cube_obj()).save("D:/test.krc");
	// packer::krc_file::deserialize("D:/test.krc");
}