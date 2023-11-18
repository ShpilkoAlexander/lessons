#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

#include <iostream>

using namespace std;

namespace img_lib {
PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    char b = 'B';
    char m = 'M';             //    Подпись — 2 байта. Символы BM.
    uint16_t total_size = 0;  //    Суммарный размер заголовка и данных — 4 байта, беззнаковое целое. Размер данных определяется как отступ, умноженный на высоту изображения.
    uint32_t reserve = 0;     //    Зарезервированное пространство — 4 байта, заполненные нулями.
    uint32_t indent = 54;     //    Отступ данных от начала файла — 4 байта, беззнаковое целое. Он равен размеру двух частей заголовка
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    // поля заголовка Bitmap Info Header
    uint32_t size = 40;                     //    Размер заголовка — 4 байта, беззнаковое целое. Учитывается только размер второй части заголовка.
    int32_t width = 0;                      //    Ширина изображения в пикселях — 4 байта, знаковое целое.
    int32_t height = 0;                     //    Высота изображения в пикселях — 4 байта, знаковое целое.
    uint16_t planes_count = 1;              //    Количество плоскостей — 2 байта, беззнаковое целое. В нашем случае всегда 1 — одна RGB плоскость.
    uint16_t pixel_bit_count = 24;          //    Количество бит на пиксель — 2 байта, беззнаковое целое. В нашем случае всегда 24.
    uint32_t compression_type = 0;          //    Тип сжатия — 4 байта, беззнаковое целое. В нашем случае всегда 0 — отсутствие сжатия.
    uint32_t data_byte_count = 0;           //    Количество байт в данных — 4 байта, беззнаковое целое. Произведение отступа на высоту.
    int32_t horizontal_resolution = 11811;  //    Горизонтальное разрешение, пикселей на метр — 4 байта, знаковое целое. Нужно записать 11811, что примерно соответствует 300 DPI.
    int32_t vertical_resolution = 11811;    //    Вертикальное разрешение, пикселей на метр — 4 байта, знаковое целое. Нужно записать 11811, что примерно соответствует 300 DPI.
    int32_t color_count = 0;                //    Количество использованных цветов — 4 байта, знаковое целое. Нужно записать 0 — значение не определено.
    int32_t mean_color_count = 0x1000000;   //    Количество значимых цветов — 4 байта, знаковое целое. Нужно записать 0x1000000.
}
PACKED_STRUCT_END

    // функция вычисления отступа по ширине
    static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

void Serialize(const BitmapFileHeader& fh, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
}

void Serialize(const BitmapInfoHeader& ih, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&ih), sizeof(ih));
}

void Deserialize(std::istream& in, BitmapFileHeader& fh) {
    in.read(reinterpret_cast<char*>(&fh), sizeof(fh));
}


void Deserialize(std::istream& in, BitmapInfoHeader& ih) {
    in.read(reinterpret_cast<char*>(&ih), sizeof(ih));
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);

    BitmapFileHeader file_header;
    file_header.total_size = file_header.indent + (GetBMPStride(image.GetWidth()) * image.GetHeight());
    out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));

    BitmapInfoHeader info_header;
    info_header.width = image.GetWidth();
    info_header.height = image.GetHeight();
    info_header.data_byte_count = GetBMPStride(image.GetWidth()) * image.GetHeight();
    out.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    const int w = image.GetWidth();
    const int h = image.GetHeight();
    std::vector<char> buff(GetBMPStride(w));

    for (int y = h - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        out.write(buff.data(), GetBMPStride(w));
    }

    return out.good();
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    // открываем поток с флагом ios::binary
    // поскольку будем читать данные в двоичном формате
    ifstream ifs(file, ios::binary);
    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;
    ifs.read(reinterpret_cast<char*>(&file_header), 14);
    ifs.read(reinterpret_cast<char*>(&info_header), 40);

    int step = GetBMPStride(info_header.width);
    Image result(info_header.width, info_header.height, Color::Black());
    std::vector<char> buff(step);
    for (int y = info_header.height - 1; y >= 0; --y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), step);
        for (int x = 0; x < info_header.width; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib
