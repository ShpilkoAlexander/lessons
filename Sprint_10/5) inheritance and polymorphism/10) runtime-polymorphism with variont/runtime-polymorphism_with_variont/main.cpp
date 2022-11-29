#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

namespace shapes {

class Triangle : public svg::Drawable {
public:
    Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
        : p1_(p1)
        , p2_(p2)
        , p3_(p3) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
    }

private:
    svg::Point p1_, p2_, p3_;
};

class Star : public Drawable {

public:

    Star(svg::Point center, double ray_len, double radius, size_t ray_count)
        : center_(center)
        , ray_len_(ray_len)
        , radius_(radius)
        , ray_count_(ray_count) {
    }

    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) const {
        using namespace svg;

        Polyline polyline;

        for (int i = 0; i <= num_rays; ++i) {
            double angle = 2 * M_PI * (i % num_rays) / num_rays;
            polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
            if (i == num_rays) {
                break;
            }
            angle += M_PI / num_rays;
            polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
        }

        return polyline;
    }

    void Draw(svg::ObjectContainer& container) const override {
        Color fill_color = "red";
        Color stroke_color = "black";

        container.Add(CreateStar(center_, ray_len_, radius_, ray_count_).SetFillColor(fill_color).SetStrokeColor(stroke_color));
    }

private:
    svg::Point center_;
    double ray_len_ = 0.0;
    double radius_ = 0.0;
    int ray_count_ = 0;
};

class Snowman : public Drawable {

public:
    Snowman(svg::Point center, double radius)
        : center_(center)
        , radius_(radius) {
    }

    void Draw(svg::ObjectContainer& container) const override {
        Color fill_color = "rgb(240,240,240)";
        Color stroke_color = "black";
        svg::Point center = center_;
        center.y += (5 * radius_);
        container.Add(svg::Circle().SetCenter(center).SetRadius(radius_ * 2).SetFillColor(fill_color).SetStrokeColor(stroke_color));

        center.y -= (3 * radius_);
        container.Add(svg::Circle().SetCenter(center).SetRadius(radius_ * 1.5).SetFillColor(fill_color).SetStrokeColor(stroke_color));

        center.y -= (2 * radius_);
        container.Add(svg::Circle().SetCenter(center).SetRadius(radius_).SetFillColor(fill_color).SetStrokeColor(stroke_color));
    }



private:
    svg::Point center_;
    double radius_;
};

} // namespace shapes

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}


int main() {
    using namespace svg;
    using namespace std;

    Color none_color;
    cout << none_color << endl; // none

    Color purple{"purple"s};
    cout << purple << endl; // purple

    Color rgb = Rgb{100, 200, 255};
    cout << rgb << endl; // rgb(100,200,255)

    Color rgba = Rgba{100, 200, 255, 0.5};
    cout << rgba << endl; // rgba(100,200,255,0.5)

    Circle c;
    c.SetRadius(3.5).SetCenter({1.0, 2.0});
    c.SetFillColor(rgba);
    c.SetStrokeColor(purple);

    Document doc;
    doc.Add(std::move(c));
    doc.Render(cout);
}
