#ifndef SYNC_EVENT_H_
#define SYNC_EVENT_H_

#include "pevents.h"

using namespace neosmart;

class SyncEvent
{
public:
    SyncEvent(bool set);
    ~SyncEvent();

    void Set();
    void Reset();
    bool IsSet() const;
    bool Wait(int milliseconds) const;
    void WaitInfinite() const;

private:
    DISALLOW_COPY_AND_ASSIGN(SyncEvent);

    neosmart_event_t event_handle_;
};

#endif // SYNC_EVENT_H_
