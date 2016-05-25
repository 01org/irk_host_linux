#include "QRGenerator.h"
#include <qrencode.h>
#include "../../GCSNativeInterface/macros.h"

#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

QRGenerator* QRGenerator::instance_ = NULL;

QRGenerator* QRGenerator::GetInstance()
{
    if(!instance_)
    {
        instance_ = new QRGenerator;
    }
    return instance_;
}

wxBitmap QRGenerator::GenerateQRCode(char const *code)
{
    const int kScale = 16;
    const int kBytesPerPixel = 3;

    unsigned int width;
    unsigned int adjusted_width;
    unsigned int data_bytes;
    unsigned char *rgb_data = NULL;
    unsigned char *source_data;
    unsigned char *destination_data;
    QRcode *qr_code = NULL;
    wxBitmap generated_bitmap;

    // Compute QRCode
    qr_code = QRcode_encodeString(code, 0, QR_ECLEVEL_H, QR_MODE_8, 1);

    if (qr_code)
    {
        width = qr_code->width;
        adjusted_width = width * kScale * kBytesPerPixel;

        if (adjusted_width % 4)
        {
            adjusted_width = (adjusted_width / 4 + 1) * 4;
        }
        data_bytes = adjusted_width * width * kScale;

        // Allocate pixels buffer
        rgb_data = (unsigned char *)malloc(sizeof(unsigned char) * data_bytes);

        if (!rgb_data)
        {
            wxLogDebug("Out of memory");
        }
        else
        {
            // Preset to white
            memset(rgb_data, 0xff, data_bytes);

            // Convert QrCode bits to bmp pixels
            unsigned int x, y, l, n;
            source_data = qr_code->data;
            for(y = 0; y < width; y++)
            {
                destination_data = rgb_data + adjusted_width * y * kScale;
                for(x = 0; x < width; x++)
                {
                    if (*source_data & 1)
                    {
                        for(l = 0; l < kScale; l++)
                        {
                            for(n = 0; n < kScale; n++)
                            {
                                *(destination_data +     n * kBytesPerPixel + adjusted_width * l) = 0;
                                *(destination_data + 1 + n * kBytesPerPixel + adjusted_width * l) = 0;
                                *(destination_data + 2 + n * kBytesPerPixel + adjusted_width * l) = 0;
                            }
                        }
                    }
                    destination_data += kBytesPerPixel * kScale;
                    source_data++;
                }
            }

            //Create the bitmap from the color data
            wxImage qr_image(width * kScale, width * kScale, rgb_data);
            generated_bitmap = wxBitmap(qr_image);
        }

        QRcode_free(qr_code);
        qr_code = NULL;
    }
     
    QRcode_clearCache();
    return generated_bitmap;
}

void QRGenerator::Destroy()
{
    QRcode_clearCache();
    SAFE_DELETE(instance_);
}

QRGenerator::QRGenerator()
{
}
