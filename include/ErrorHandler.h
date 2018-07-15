#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <iostream>

#if defined(__GNUC__)
#define DEBUG_BREAK() __builtin_trap()
#elif defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#endif

#define M_DEBUG
#if defined(M_DEBUG)
#define LOG_ERROR(x) std::cerr << "Error: "<< __FILE__ << ":" << __LINE__ << " " << __func__ << " [" << x << "]" << std::endl;
#define LOG_WARNING(x) std::cerr << "Warning: "<< __FILE__ << ":" << __LINE__ << " " << __func__ << " [" << x << "]" << std::endl;
#define DBG_V(x) std::cout << "Debug: " << __FILE__ << ":" << __LINE__ << " " << __func__ << " [" << x << "]" << std::endl;
#define DBG(x) std::cout << "Debug: " << "[" << x << "]" << std::endl;
#else
#define LOG_ERROR(x)
#define LOG_WARNING(x)
#define DBG_V(x)
#define DBG(x)
#endif

#endif
