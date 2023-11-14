#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

class ImageFormatInterface {
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
};

namespace FormatInterfaces {

class PPM : public ImageFormatInterface {
    img_lib::Image LoadImage(const img_lib::Path& file) const {
        return img_lib::LoadPPM(file);
    }

    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const {
        return img_lib::SaveJPEG(file, image);
    }
};

class JPEG : public ImageFormatInterface {
    img_lib::Image LoadImage(const img_lib::Path& file) const {
        return img_lib::LoadJPEG(file);
    }

    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const {
        return img_lib::SavePPM(file, image);
    }
};


} // FormatInterfaces

enum Format {
    JPEG,
    PPM,
    UNKNOWN
};

static /*const*/ FormatInterfaces::PPM ppmInterface;
static /*const*/ FormatInterfaces::JPEG jpegInterface;

Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    return Format::UNKNOWN;
}

ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
    Format image_format = GetFormatByExtension(path);

    switch (image_format) {
    case Format::JPEG:
        return &jpegInterface;
        break;
    case Format::PPM:
        return &ppmInterface;
        break;
    case Format::UNKNOWN:
        return nullptr;
        break;
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }
    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];
    ImageFormatInterface* in_file = GetFormatInterface(in_path);
    ImageFormatInterface* out_file = GetFormatInterface(out_path);

    if (!in_file) {
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }
    if (!out_file) {
        cerr << "Unknown format of the output file"sv << endl;
        return 3;
    }

    img_lib::Image image =  in_file->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!out_file->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
}
