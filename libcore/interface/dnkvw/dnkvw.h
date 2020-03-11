#ifndef DNKVW_HEADER
#define DNKVW_HEADER

#ifdef _WIN32
# ifdef DNKVW_WIN_EXPORT
#   define DNKVW_API  __declspec( dllexport )
# else
#   define DNKVW_API  __declspec( dllimport )
# endif
#else
# define DNKVW_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

DNKVW_API int addNums(int a, int b);

#ifdef __cplusplus
}
#endif

#endif
