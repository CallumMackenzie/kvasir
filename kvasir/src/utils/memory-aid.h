#ifndef MEMORY_AID_H_
#define MEMORY_AID_H_ 1

#ifndef __APPLE__

#define COUNT_ALLOCS                \
	size_t allocs = 0;              \
	void *operator new(size_t size) \
	{                               \
		++allocs;                   \
		return malloc(size);        \
	}

// Counts frees into a variable called 'deletes'.
#define COUNT_DELETES               \
	size_t deletes = 0;             \
	void operator delete(void *ptr) \
	{                               \
		++deletes;                  \
		free(ptr);                  \
	}

#else
#define COUNT_DELETES
#define COUNT_ALLOCS
#endif

// Counts allocs and frees into variables called 'allocs' and 'deletes' respectively.
#define COUNT_MEMORY \
	COUNT_ALLOCS     \
	COUNT_DELETES

#ifndef __APPLE__
// Prints the number of allocations.
#define PRINT_ALLOCS \
	std::cout        \
		<< "Allocations: " << allocs << std::endl;

// Prints the number of frees.
#define PRINT_DELETES \
	std::cout         \
		<< "Deletes: " << deletes << std::endl;

// Prints the number of allocations and frees.
#define PRINT_MEMORY_SUMMARY                 \
	std::cout                                \
		<< "Allocs: " << allocs << std::endl \
		<< "Deletes: " << deletes << std::endl;

#else
#define PRINT_ALLOCS
#define PRINT_DELETES
#define PRINT_MEMORY_SUMMARY
#endif

#define DEL_PTR(ptr)   \
	if (ptr)           \
	{                  \
		delete ptr;    \
		ptr = nullptr; \
	}

#define DEL_ARR_PTR(ptr) \
	if (ptr)             \
	{                    \
		delete[] ptr;    \
		ptr = nullptr;   \
	}

#endif
