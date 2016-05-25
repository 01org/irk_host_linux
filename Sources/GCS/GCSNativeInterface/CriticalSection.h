#ifndef CRITICAL_SECTION_H_
#define CRITICAL_SECTION_H_

#include "macros.h"
#include "pthread.h"
typedef pthread_mutex_t CRITICAL_SECTION;

class CriticalSection
{
public:
    CriticalSection();
    ~CriticalSection();

    void Enter() const;
    bool TryEnter() const;
    void Leave() const;

private:
    DISALLOW_COPY_AND_ASSIGN(CriticalSection);

    CRITICAL_SECTION *critical_section_;
};

#endif // CRITICAL_SECTION_H_
