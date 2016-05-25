#include "StdAfx.h"
#include "CriticalSection.h"

CriticalSection::CriticalSection()
    : critical_section_(NULL)
{
    critical_section_ = new CRITICAL_SECTION;
    pthread_mutex_init(critical_section_, 0);
}

CriticalSection::~CriticalSection()
{
    pthread_mutex_destroy(critical_section_);
    SAFE_DELETE(critical_section_);
}

void CriticalSection::Enter() const
{
    pthread_mutex_lock(critical_section_);
}

bool CriticalSection::TryEnter() const
{
    return (pthread_mutex_trylock(critical_section_) != 0);
}

void CriticalSection::Leave() const
{
    pthread_mutex_unlock(critical_section_);
}
