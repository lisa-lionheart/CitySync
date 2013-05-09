#ifndef SC4HOOK_GLOBAL_H
#define SC4HOOK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SC4HOOK_LIBRARY)
#  define SC4HOOKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SC4HOOKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SC4HOOK_GLOBAL_H
