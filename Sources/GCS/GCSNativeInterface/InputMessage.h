#ifndef INPUT_MESSAGE_H_
#define INPUT_MESSAGE_H_

class IPAddress;
class NetworkBuffer;

class InputMessage
{
public:
    enum InputMessageType
    {
        kHeartbeatMessage = 'H',
        kKeyboardEventMessage = 'K',
        kMouseMoveEventMessage = 'M',
        kMouseButtonEventMessage = 'B',
        kMouseScrollEventMessage = 'S',
        kDisconnectMessage = 'X',
        kDockInfoMessage = 'I',
        kUnicharEventMessage = 'U',
        kSuspendGCCMessage = '0',
        kWindows8CommandMessage = 'W',
        kControlPCMessage = 'P',
        kClientInfoMessage = '*',
        kPasscodeMessage = 'p',
        kVolumeChangedMessage = 'V',
        kPowerOptionMessage = 'O',
    };

    InputMessage(InputMessageType type);
    virtual ~InputMessage();

    void Init(NetworkBuffer *network_buffer);

    InputMessageType GetType() const;
    bool IsValid() const;

protected:
    virtual void ReadMessage() = 0;
    
    int ReadInt(int defaultValue = 0);
    unsigned int ReadUnsignedInt(unsigned int defaultValue = 0);
    short ReadShort(short defaultValue = 0);
    float ReadFloat(float defaultValue = 0.0f);
    char ReadChar(char defaultValue = '\0');
    wchar_t ReadWChar(wchar_t defaultValue = L'\0');
    BYTE ReadByte(BYTE defaultValue = 0);
    bool ReadBool(bool defaultValue = false);

    void ReadWString(std::wstring &value);
    void ReadIPAddress(IPAddress &address);
    void ReadData(BYTE *data_buffer, unsigned int data_size);

private:
    DISALLOW_COPY_AND_ASSIGN(InputMessage);

    InputMessageType message_type_;
    bool is_valid_;
    NetworkBuffer *network_buffer_;
};

#endif //INPUT_MESSAGE_H_
