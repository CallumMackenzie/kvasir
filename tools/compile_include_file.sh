rm -rf ./kv-ex-include.h
gcc -E ../kvasir/kvasir-eng-include.h -DEXPANDING_INCLUDE=1 >> kv-ex-include.h
sed -i \
-e '1c#ifndef KVASIR_INCLUDE_H__ ' \
-e '2c#define KVASIR_INCLUDE_H__ 1 ' \
-e '$c#endif' \
-e '/#/c\ ' \
kv-ex-include.h
sed -i \
-e '3s/^/#include <stdio.h> \n/' \
-e '3s/^/#include <stddef.h> \n/' \
-e '3s/^/#include <time.h> \n/' \
-e '3s/^/#include <string.h> \n/' \
-e '3s/^/#include <unordered_map> \n/' \
-e '3s/^/#include <vector> \n/' \
-e '3s/^/#include <string> \n/' \
-e '3s/^/#include <iostream> \n/' \
-e '3s/^/#include <memory> \n/' \
-e '3s/^/#include <fstream> \n/' \
-e '3s/^/#include <sstream> \n/' \
-e '3s/^/#include <initializer_list> \n/' \
-e '3s/^/#include <cmath> \n/' \
kv-ex-include.h
sed -i \
	-e '3s/^/#define DEL_PTR(x) if (x) {delete x; x = nullptr;} \n/' \
	-e '3s/^/#define DEL_ARR_PTR(x) if (x) {delete[] x; x = nullptr;} \n/' \
kv-ex-include.h
