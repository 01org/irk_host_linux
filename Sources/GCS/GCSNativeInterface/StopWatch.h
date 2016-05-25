#ifndef STOP_WATCH_H_
#define STOP_WATCH_H_

#include <sys/time.h>

class StopWatch
{
public:
    StopWatch();
    virtual ~StopWatch();

    void Start();
    void Stop();
    float GetElapsedSeconds() const;

private:
    LONG64 GetCurrentCounterValue() const;

    float inverse_frequency_;
    struct timeval start_time_;
    struct timezone time_zone;
    float elapsed_seconds_;
};

#endif // STOP_WATCH_H_
