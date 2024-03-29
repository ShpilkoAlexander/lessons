
#include "ppm_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>

using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

byte SobelValue(const img_lib::Image& image, int x, int y) {
    int tl = Sum(image.GetLine(y - 1)[x - 1]);
    int tc = Sum(image.GetLine(y - 1)[x]);
    int tr = Sum(image.GetLine(y - 1)[x + 1]);
    int cl = Sum(image.GetLine(y)[x - 1]);
    int cr = Sum(image.GetLine(y)[x + 1]);
    int bl = Sum(image.GetLine(y + 1)[x - 1]);
    int bc = Sum(image.GetLine(y + 1)[x]);
    int br = Sum(image.GetLine(y + 1)[x + 1]);

    int gx = - tl - 2*tc - tr + bl + 2*bc + br;
    int gy= - tl - 2*cl - bl + tr + 2*cr + br;

    return static_cast<byte>(clamp<double>(sqrt(gx*gx + gy*gy), 0, 255));
}

// реализуйте оператор Собеля
img_lib::Image Sobel(const img_lib::Image& image) {
    img_lib::Image result_img(image.GetWidth(), image.GetHeight(), img_lib::Color::Black());
    for (int y = 1; y < image.GetHeight() - 1; ++y) {
        for (int x = 1; x < image.GetWidth() - 1; ++x) {
            byte sobel_value = SobelValue(image, x, y);

            result_img.GetLine(y)[x].r = sobel_value;
            result_img.GetLine(y)[x].g = sobel_value;
            result_img.GetLine(y)[x].b = sobel_value;
        }
    }
    return result_img;
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    image = Sobel(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
