#include "kvasir-eng-include.h"

#include <iostream>

using namespace kvasir;

int main(int, char **)
{
	kvasir_init();
	render_base *base = kvasir_engine::get_base(render_base::OPENGL);
	base->init("KVS", 720, 480);

	material_base *mb = make_material(base, RESOURCE("../res/img/h.png"));
	kvasir_destroy();
	printf("Fin");
}