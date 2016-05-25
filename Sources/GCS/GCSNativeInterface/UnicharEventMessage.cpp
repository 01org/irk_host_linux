#include "StdAfx.h"
#include "UnicharEventMessage.h"
#include <linux/input.h>
#include <unistd.h>

UnicharEventMessage::UnicharEventMessage() 
    : InputMessage(InputMessage::kUnicharEventMessage)
    , characters_()
{
}

// virtual 
UnicharEventMessage::~UnicharEventMessage()
{
}

// virtual 
void UnicharEventMessage::ReadMessage()
{
    ReadWString(characters_);
}

const std::wstring & UnicharEventMessage::GetCharacters() const
{
	return characters_;
}

void UnicharEventMessage::SendUnicharEvent(int input_handler_fd) const
{
    if(input_handler_fd == -1)
    {
        return;
    }

    char type = 'U';
    unsigned int num_characters = static_cast<unsigned int>(characters_.length());

    for (unsigned int i = 0; i < num_characters; ++i)
    {
        wchar_t character = characters_[i];
        write(input_handler_fd, &type, sizeof(type));
        write(input_handler_fd, &character, sizeof(character));
    }
}
