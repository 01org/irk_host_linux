#ifndef CLIENT_DOCK_INFO_H_
#define CLIENT_DOCK_INFO_H_

enum ClientDockType
{
    ClientDock_NoDock,
    ClientDock_iOS,
    ClientDock_Android
};

typedef struct _ClientDockInfo
{
public:
    _ClientDockInfo();
    void Clear();

    ClientDockType type_;
    int screen_width_;
    int screen_height_;
} ClientDockInfo;

#endif // CLIENT_DOCK_INFO_H_