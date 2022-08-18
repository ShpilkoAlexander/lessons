#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE};

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) :
        type_(type) {
    }

    void SetPosition(Point pos) {
        pos_ = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = texture;
    }

    // Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        if (type_ == ShapeType::ELLIPSE) {
            for (int height = 0; height < size_.height; ++height) {
                for (int width = 0; width < size_.width; ++width) {
                    if (GetImageSize(image).height <= height + pos_.y || GetImageSize(image).width <= width + pos_.x) {
                        break;
                    }
                    if (!IsPointInEllipse({width, height}, size_)) {
                        continue;
                    }
                    if (texture_ == nullptr || texture_->GetSize().height <= height || texture_->GetSize().width <= width) {
                        image.at(height + pos_.y).at(width + pos_.x) = '.';
                    }
                    else {
                        image.at(height + pos_.y).at(width + pos_.x) = texture_->GetPixelColor({width, height});
                    }
                }
            }
        }

        else {
            for (int height = 0; height < size_.height; ++height) {
                for (int width = 0; width < size_.width; ++width) {
                    if (GetImageSize(image).height <= height + pos_.y || GetImageSize(image).width <= width + pos_.x) {
                        break;
                    }
                    if (!IsPointInRectangle({width, height}, size_)) {
                        continue;
                    }
                    if (texture_ == nullptr || texture_->GetSize().height <= height || texture_->GetSize().width <= width) {
                        image.at(height + pos_.y).at(width + pos_.x) = '.';
                    }
                    else {
                        image.at(height + pos_.y).at(width + pos_.x) = texture_->GetPixelColor({width, height});
                    }
                }
            }
        }
    }

private:
    Point pos_;
    Size size_;
    ShapeType type_;
    std::shared_ptr<Texture> texture_ = std::make_unique<Texture>(move(Image{}));
};
