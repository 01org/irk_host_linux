#include "StdAfx.h"
#include "SyncEvent.h"

SyncEvent::SyncEvent(bool set)
    : event_handle_(NULL)
{
    event_handle_ = CreateEvent(true, set ? true : false);
}

SyncEvent::~SyncEvent()
{
    CloseHandle(event_handle_);
}

void SyncEvent::Set()
{
    BOOL result = SetEvent(event_handle_);
    UNREFERENCED_PARAMETER(result);
}

void SyncEvent::Reset()
{
    BOOL result = ResetEvent(event_handle_);
    UNREFERENCED_PARAMETER(result);
}

bool SyncEvent::IsSet() const
{
    return Wait(0);
}

bool SyncEvent::Wait(int milliseconds) const
{
    int result = WaitForEvent(event_handle_, milliseconds);
    return result == 0;
}

void SyncEvent::WaitInfinite() const
{
    int result = WaitForEvent(event_handle_);
    UNREFERENCED_PARAMETER(result);
}
