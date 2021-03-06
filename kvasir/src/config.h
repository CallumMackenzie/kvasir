#ifndef KVASIR_CONFIG_H
#define KVASIR_CONFIG_H 1

#ifdef _WIN32

#ifdef BUILD_SHARED
#define KV_EXPORT __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define KV_EXPORT
#elif defined(KVASIR_DLL)
#define KV_EXPORT __declspec(dllimport)
#else
#define KV_EXPORT
#endif

#else
#define KV_EXPORT
#endif

#endif