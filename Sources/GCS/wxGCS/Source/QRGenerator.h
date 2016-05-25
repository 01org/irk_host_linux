#ifndef QR_GENERATOR_H_
#define QR_GENERATOR_H_

#include<stdlib.h>
#include <wx/wx.h>

class QRGenerator
{
public:
    static QRGenerator* GetInstance();
    wxBitmap GenerateQRCode(char const *code);
    static void Destroy();

private:
    QRGenerator();
    static QRGenerator* instance_;
};

#endif //QR_GENERATOR_H_