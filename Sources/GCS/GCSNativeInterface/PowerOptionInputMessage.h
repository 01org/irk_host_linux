#ifndef POWER_OPTION_INPUT_MESSAGE_H_
#define POWER_OPTION_INPUT_MESSAGE_H_
#include "InputMessage.h"

enum PowerOptionType
{
    SWITCH_USER = 0x00,
    LOG_OFF = 0x01,
    SLEEP = 0x02,
    RESTART = 0x03,
    SHUT_DOWN = 0x04
};

class PowerOptionInputMessage : public InputMessage
{
public:
    PowerOptionInputMessage();
    virtual ~PowerOptionInputMessage();

    BYTE GetOption() const;
    void HandlePowerOption(bool isAtLogon) const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(PowerOptionInputMessage);

    BYTE power_option_;
};

#endif // POWER_OPTION_INPUT_MESSAGE_H_
