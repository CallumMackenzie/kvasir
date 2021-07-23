#ifndef KVASIR_BUFFER_H_
#define KVASIR_BUFFER_H_ 1

namespace kvasir
{

	struct buffer_base
	{
		virtual void gen_buffer() = 0;
		virtual void set_data(void *data, size_t len, bool change_often = false) = 0;
		virtual void sub_data(void *data, size_t offset, size_t len) = 0;
		virtual void get_data(void *recv, size_t offset, size_t len) = 0;
		virtual void attrib_ptr(size_t ptr_num, size_t size, size_t step = 0, size_t offset = 0) = 0;
		virtual void bind_buffer() = 0;
		virtual void bind_vao() = 0;
	};

}

#endif