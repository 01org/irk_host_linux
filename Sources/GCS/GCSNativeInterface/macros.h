#ifndef MACROS_H_
#define MACROS_H_

#include <stddef.h>
#include <string.h>

#ifdef _DEBUG
#include <cassert>
#define ASSERT(expression) assert(expression)
#else
#define ASSERT(expression)
#endif

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  TypeName& operator=(const TypeName&)

#define SAFE_DELETE(object) \
    if ((object) != NULL) \
    { \
        delete (object); \
        (object) = NULL; \
    }

#define SAFE_DELETE_ARRAY(object) \
    if ((object) != NULL) \
    { \
        delete [] (object); \
        (object) = NULL; \
    }

#define SAFE_RELEASE(object) \
    if ((object) != NULL) \
    { \
        (object)->Release(); \
        (object) = NULL; \
    }

template <typename T>
inline void SAFE_ASSIGN_REF(T *&dst, T *src)
{
    if (src != NULL)
    {
        src->AddRef();
    }
    if (dst != NULL)
    {
        dst->Release();
    }
    dst = src;
}

inline void MemCopy(void *dst, size_t dst_size, const void *src, size_t count)
{
    dst_size;
    memcpy(dst, src, count);
}

#endif // MACROS_H_
