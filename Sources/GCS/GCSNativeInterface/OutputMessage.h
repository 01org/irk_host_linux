#ifndef OUTPUT_MESSAGE_H_
#define OUTPUT_MESSAGE_H_

class CommunicationBuffer;
class OutputMessage
{
public:
    enum OutputMessageType
    {
        kServerBroadcast = 'B',
        kHeartbeat = 'H',
        kServerInfo = 'S',
        kServerDisconnect = 'D',
        kSetLog = 'L',
        kAuthenticationRequest = 'R',
        kAuthenticationWrongPassword = 'W',
        kAuthenticationSuccess = 'A',
        kAuthenticationFailed = 'F',
        kMediaVolumeChanged = 'V',
        kMediaMuteChanged = 'Z',
        kElevatedPrivilegesChanged = 'E',
        kPowerOptionMessage = 'O',
        kLogonInfo = 'l',
    };

    OutputMessage(OutputMessageType type);
    virtual ~OutputMessage();       

    const CommunicationBuffer *GetCommunicationBuffer();
    const OutputMessageType &GetType() const;

protected:
    virtual int GetDataSize() const = 0 ;
    virtual void SetData(CommunicationBuffer *buffer, int data_size) const = 0 ;

private:
    DISALLOW_COPY_AND_ASSIGN(OutputMessage);

    OutputMessageType message_type_;
    CommunicationBuffer *buffer_; 
};

#endif // OUTPUT_MESSAGE_H_
