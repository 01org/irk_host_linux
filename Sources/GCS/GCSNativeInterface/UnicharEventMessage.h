#ifndef UNICHAR_EVENT_MESSAGE_H_
#define UNICHAR_EVENT_MESSAGE_H_
#include "InputMessage.h"

class UnicharEventMessage : public InputMessage
{
public:
    UnicharEventMessage();
    virtual ~UnicharEventMessage();

	const std::wstring & GetCharacters() const;

	void SendUnicharEvent(int input_handler_fd = -1) const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(UnicharEventMessage);

    std::wstring characters_;
};

#endif // UNICHAR_EVENT_MESSAGE_H_
