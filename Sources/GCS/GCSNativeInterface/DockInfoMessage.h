#ifndef DOCKINFO_MESSAGE_H_
#define DOCKINFO_MESSAGE_H_
#include "InputMessage.h"
#include "ClientDockInfo.h"

class DockInfoMessage : public InputMessage
{    
public:
    DockInfoMessage();
    virtual ~DockInfoMessage();

    const ClientDockInfo &GetDockInfo() const;

protected:
    virtual void ReadMessage();

private:
    DISALLOW_COPY_AND_ASSIGN(DockInfoMessage);

    ClientDockInfo dock_info_;
};

#endif // DOCKINFO_MESSAGE_H_
