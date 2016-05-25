#include "StdAfx.h"
#include "BasicClientDevice.h"

namespace
{
    const std::wstring kDeviceIdentityKey(L"DeviceID");
    const std::wstring kDeviceNameKey(L"DeviceName");
    const std::wstring kDeviceMakeKey(L"DeviceMake");
    const std::wstring kDeviceTypeKey(L"DeviceType");
};

BasicClientDevice::BasicClientDevice(ClientDeviceType type, const std::wstring &name, const std::wstring &device_id, const std::wstring &device_type, const std::wstring &device_make)
    : type_(type)
    , name_(name)
    , device_id_(device_id)
    , device_type_(device_type)
    , device_make_(device_make)
{
}

// virtual
BasicClientDevice::~BasicClientDevice()
{
}

ClientDeviceType BasicClientDevice::GetType() const
{
    return type_;
}

const std::wstring &BasicClientDevice::GetName() const
{
    return name_;
}

const std::wstring &BasicClientDevice::GetDeviceType() const
{
    return device_type_;
}

const std::wstring &BasicClientDevice::GetDeviceMake() const
{
    return device_make_;
}

const std::wstring &BasicClientDevice::GetDeviceID()
{
    return device_id_;
}
