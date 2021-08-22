
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define A(s) \
	.append(s)

namespace kvasir
{
	namespace resource_bundler
	{
		std::string bundle_string_resource_from_stream(std::string name, std::basic_istream<char, std::char_traits<char>> *f)
		{
			static const size_t BUFF_SZ = 256;
			const char ops[5]{'\'', '\"', '\\', '\n', '\t'};
			const char dic[5]{'\'', '\"', '\\', 'n', 't'};
			using namespace std;
			string header_define = string(name) A("__RESOURCE_HEADER_");
			string head = string("#ifndef ") A(header_define) A("\n#define ") A(header_define) A("\nnamespace kvasir{namespace data{");
			string foot = string("}}\n#endif\n");

			string body = string("namespace ") A(name) A("{");
			string body_foot = string("}");
			string datastr = string("static const char *data=\"");
			while (!f->eof())
			{
				char buff[BUFF_SZ] = {'\0'};
				f->get(buff, BUFF_SZ - 1, '\0');
				buff[BUFF_SZ - 1] = '\0';
				std::vector<char> dat;
				for (size_t i = 0; i < BUFF_SZ; ++i)
				{
					if (buff[i] == '\0')
						break;
					bool foundchar = false;
					for (size_t j = 0; j < 5; ++j)
						if (buff[i] == ops[j])
						{
							dat.push_back('\\');
							dat.push_back(dic[j]);
							foundchar = true;
							break;
						}
					if (!foundchar)
						dat.push_back(buff[i]);
				}
				if (dat.size() == 0)
					break;
				dat.push_back('\0');
				datastr A(std::string((char *)(&dat[0])));
			}
			datastr A("\";");
			body A(datastr) A(body_foot);

			return head.append(body).append(foot);
		}
		std::string bundle_resource(const char *file_path, std::string name)
		{
			return bundle_string_resource_from_stream(name, &std::ifstream(file_path));
		}
	}
}

void main(int argc, char **argv)
{
	// kvasir::resource_bundler::bundle_resource("D:\\3D Models\\isont.obj", "ico_sphere");
	std::ofstream of("D:\\Coding\\Cpp\\kvasir\\tests\\MANDO.h");
	of << kvasir::resource_bundler::bundle_resource("D:\\3D Models\\uvsmoothnt.obj", "mando_ship");
	of.close();
}
