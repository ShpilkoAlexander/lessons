#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    Size GetSize() const {
        return GetImageSize(image_);
    }

    char GetPixelColor(Point p) const {
        (void) p;
        return image_.at(p.y).at(p.x);
    }

private:
    Image image_;
};
