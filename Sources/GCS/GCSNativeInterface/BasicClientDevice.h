#ifndef BASIC_CLIENT_DEVICE_H_
#define BASIC_CLIENT_DEVICE_H_

#include "ClientDeviceType.h"

class BasicClientDevice
{
public:
    BasicClientDevice(ClientDeviceType type, const std::wstring &name, const std::wstring &device_id, const std::wstring &device_type, const std::wstring &device_make);
    virtual ~BasicClientDevice();

    ClientDeviceType GetType() const;
    const std::wstring &GetName() const;
    const std::wstring &GetDeviceID();
    const std::wstring &GetDeviceType() const;
    const std::wstring &GetDeviceMake() const;

private:
    DISALLOW_COPY_AND_ASSIGN(BasicClientDevice);

    ClientDeviceType type_;
    std::wstring name_;
    std::wstring device_id_;
    std::wstring device_type_;
    std::wstring device_make_;
};

#endif // BASIC_CLIENT_DEVICE_H_
