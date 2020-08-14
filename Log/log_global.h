#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(LOG_LIB)
#  define LOG_EXPORT Q_DECL_EXPORT
# else
#  define LOG_EXPORT Q_DECL_IMPORT
# endif
#else
# define LOG_EXPORT
#endif
