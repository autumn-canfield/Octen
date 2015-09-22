#ifdef _WIN32
#define OS_WINDOWS
#endif

// OS_LINUX is defined on both linux and mac.
#ifdef __unix__
#define OS_UNIX
#endif

#ifdef __linux__
#define OS_LINUX
#endif

#ifdef __APPLE__
#define OS_MAC
#endif

