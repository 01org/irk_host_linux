#ifndef COMMUNICATION_BUFFER_H_
#define COMMUNICATION_BUFFER_H_
#include "OutputMessage.h"
#include "GUID.h"

class IPAddress;

class CommunicationBuffer 
{
public:
    CommunicationBuffer(unsigned int size);
    ~CommunicationBuffer();

    const BYTE *GetBuffer() const;
    unsigned int GetSize() const;
    
    void PushInt(int value);
    void PushUnsignedInt(unsigned int value);
    void PushShort(short value);
    void PushFloat(float value);
    void PushChar(char value);
    void PushByte(BYTE value);
    void PushBool(bool value);
    void PushString(const std::wstring &value);
    void PushData(const BYTE *data, unsigned int data_size);
    void PushGUID(const GUID &guid);

private:
    DISALLOW_COPY_AND_ASSIGN(CommunicationBuffer);

    BYTE *buffer_;
    BYTE *current_data_pointer_;
    unsigned int size_;
};

#endif // COMMUNICATION_BUFFER_H_
