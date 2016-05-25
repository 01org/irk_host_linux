#include "StdAfx.h"
#include "StopWatch.h"
#include <float.h>

StopWatch::StopWatch()
    : inverse_frequency_(FLT_MAX)
    , start_time_()
    , elapsed_seconds_(0.0f)
{
}

// virtual
StopWatch::~StopWatch()
{
}

void StopWatch::Start()
{
    gettimeofday(&start_time_, &time_zone);
}

void StopWatch::Stop()
{
    struct timeval stop_time;
    gettimeofday(&stop_time, &time_zone);
    
    elapsed_seconds_ = stop_time.tv_sec - start_time_.tv_sec; //seconds
    elapsed_seconds_ += (stop_time.tv_usec - start_time_.tv_usec) * 0.000001; //(microseconds)/1,000,000 -> seconds
}

float StopWatch::GetElapsedSeconds() const
{
    return elapsed_seconds_;
}

LONG64 StopWatch::GetCurrentCounterValue() const
{
    return 0;
}
