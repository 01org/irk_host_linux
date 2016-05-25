#include "StdAfx.h"
#include "StreamNetworkBuffer.h"
#include "IPAddress.h"
#include "NetworkSocket.h"
#include "pevents.h"
#include <string.h>

namespace
{
    static const int kMaxBufferSize = 1024;
    static const unsigned int THREAD_EXIT_FAILED = 0xFFFFFFFF;
    static const unsigned int THREAD_EXIT_SUCCESS = 0;
}

StreamNetworkBuffer::StreamNetworkBuffer(NetworkSocket *socket)
    : NetworkBuffer()
    , socket_(socket)
    , thread_handle_()
    , thread_exit_signal_(false)
    , receive_buffer_signal_(true)
    , buffer_received_signal_(false)
    , receive_buffer_(NULL)
    , current_buffer_(NULL)
    , current_buffer_size_(0)
    , current_buffer_index_(0)
{
    receive_buffer_ = new BYTE[kMaxBufferSize];
    current_buffer_ = new BYTE[kMaxBufferSize];
    pthread_create(&thread_handle_, NULL, &StreamNetworkBuffer::ReceiveBufferThreadEntry, this);
}

// virtual
StreamNetworkBuffer::~StreamNetworkBuffer()
{
    thread_exit_signal_.Set();      // Let the thread know we want to exit
    receive_buffer_signal_.Set();   // Wake up the thread in case it is waiting for us
    void *retval;
    pthread_join(thread_handle_, &retval);
    unsigned int *status = static_cast<unsigned int *>(retval);
    SAFE_DELETE(status);

    SAFE_DELETE_ARRAY(receive_buffer_);
    SAFE_DELETE_ARRAY(current_buffer_);
}

bool StreamNetworkBuffer::ReadData(BYTE *data_buffer, unsigned int data_size, int timeoutMilliseconds)
{
    unsigned int remaining = current_buffer_size_ - current_buffer_index_;

    // If the buffer contains enough data, just set it and succeed
    if (current_buffer_ != NULL && data_buffer != NULL && remaining >= data_size)
    {
        MemCopy(data_buffer, data_size, &(current_buffer_[current_buffer_index_]), data_size);
        current_buffer_index_ += data_size;
        return true;
    }

    // If we have partial data, set the data that we have and advance the buffer pointer appropriately
    if (current_buffer_ != NULL && data_buffer != NULL && remaining > 0)
    {
        MemCopy(data_buffer, data_size, &current_buffer_[current_buffer_index_], remaining);
        current_buffer_index_ += remaining;
        data_buffer += remaining;
        data_size -= remaining;
    }

    // Wait for a new buffer from the network stream
    receive_buffer_signal_.Set();
    if (!buffer_received_signal_.Wait(timeoutMilliseconds))
    {
        return false;
    }
    buffer_received_signal_.Reset();

    // Fill in any remaining data
    return ReadData(data_buffer, data_size, timeoutMilliseconds);
}

// static
void *StreamNetworkBuffer::ReceiveBufferThreadEntry(void *context)
{
    StreamNetworkBuffer *network_buffer = reinterpret_cast<StreamNetworkBuffer *>(context);
    unsigned int *exit_code = new unsigned int;
    if (network_buffer == NULL)
    { 
        GCSOutputDebugString(L"SocketNetworkBuffer::threadEntry bad network buffer passed\n");
        
        *exit_code = THREAD_EXIT_FAILED;
        return exit_code;
    }

    *exit_code = THREAD_EXIT_SUCCESS;

    while (!network_buffer->thread_exit_signal_.IsSet())
    {  

        if (!network_buffer->ReceiveNextBuffer())
        {
            *exit_code = THREAD_EXIT_FAILED;
            break;
        }
    }

    pthread_exit(exit_code);
}

bool StreamNetworkBuffer::ReceiveNextBuffer()
{
    receive_buffer_signal_.WaitInfinite();
    receive_buffer_signal_.Reset();

    if (!thread_exit_signal_.IsSet() && receive_buffer_ != NULL)
    {
        int count = Receive(receive_buffer_, kMaxBufferSize);
        if (count < 0)
        {
            return false;
        }

        BYTE *swap_buffer = receive_buffer_;
        receive_buffer_ = current_buffer_;
        current_buffer_ = swap_buffer;

        current_buffer_index_ = 0;
        current_buffer_size_ = count;

        buffer_received_signal_.Set();
    }
    else
    {
        return false;
    }

    return true;
}

int StreamNetworkBuffer::Receive(BYTE *buffer, int buffer_size)
{
    if (socket_ == NULL)
    {
        GCSOutputDebugString(L"Invalid socket");
        return -1;
    }

    return socket_->Receive(NULL, buffer, buffer_size);
}
