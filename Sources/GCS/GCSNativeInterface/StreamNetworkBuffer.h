#ifndef STREAM_NETWORK_BUFFER_H_
#define STREAM_NETWORK_BUFFER_H_

#include "NetworkBuffer.h"
#include "SyncEvent.h"

typedef pthread_t HANDLE;

class NetworkSocket;

class StreamNetworkBuffer : public NetworkBuffer
{
public:
    StreamNetworkBuffer(NetworkSocket *socket);
    virtual ~StreamNetworkBuffer();

    virtual bool ReadData(BYTE *data_buffer, unsigned int data_size, int timeoutMilliseconds);

private:
    DISALLOW_COPY_AND_ASSIGN(StreamNetworkBuffer);
    
    static void *ReceiveBufferThreadEntry(void *context);
    
    bool ReceiveNextBuffer();
    int Receive(BYTE *buffer, int buffer_size);
    
    NetworkSocket *socket_;
    HANDLE thread_handle_;
    SyncEvent thread_exit_signal_;
    SyncEvent receive_buffer_signal_;
    SyncEvent buffer_received_signal_;

    BYTE *receive_buffer_;
    BYTE *current_buffer_;
    unsigned int current_buffer_size_;
    unsigned int current_buffer_index_;
};

#endif // STREAM_NETWORK_BUFFER_H_
