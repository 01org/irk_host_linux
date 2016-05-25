#ifndef SIMPLE_NETWORK_BUFFER_H_
#define SIMPLE_NETWORK_BUFFER_H_

#include "NetworkBuffer.h"

class SimpleNetworkBuffer : public NetworkBuffer
{
public:
    SimpleNetworkBuffer(const BYTE *buffer, unsigned int buffer_size);
    virtual ~SimpleNetworkBuffer();

    virtual bool ReadData(BYTE *data_buffer, unsigned int data_size, int timeoutMilliseconds);

private:
    DISALLOW_COPY_AND_ASSIGN(SimpleNetworkBuffer);

    const BYTE * const buffer_;
    const unsigned int buffer_size_;
    unsigned int buffer_offset_;
};

#endif // SIMPLE_NETWORK_BUFFER_H_
