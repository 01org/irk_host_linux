#include "StdAfx.h"
#include "SimpleNetworkBuffer.h"
#include "IPAddress.h"

#include <string.h>

SimpleNetworkBuffer::SimpleNetworkBuffer(const BYTE *buffer, unsigned int buffer_size)
    : NetworkBuffer()
    , buffer_(buffer)
    , buffer_size_(buffer_size)
    , buffer_offset_(0)
{
}

// virtual
SimpleNetworkBuffer::~SimpleNetworkBuffer()
{
    // We do not delete the buffer because we do not have ownership of it
}

bool SimpleNetworkBuffer::ReadData(BYTE *data_buffer, unsigned int data_size, int timeoutMilliseconds)
{
    UNREFERENCED_PARAMETER(timeoutMilliseconds);
    unsigned int remaining = buffer_size_ - buffer_offset_;

    // If the buffer contains enough data, just set it and succeed
    if (buffer_ != NULL && data_buffer != NULL && remaining >= data_size)
    {
        MemCopy(data_buffer, data_size, &buffer_[buffer_offset_], data_size);
	    buffer_offset_ += data_size;
        return true;
    }

    // Otherwise, we cannot get any additional data
    return false;
}
