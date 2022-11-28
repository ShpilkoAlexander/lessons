#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_map>

namespace svg {

struct Point {
    Point() = default;
    Point(double x, double y)
        : x(x)
        , y(y) {
    }
    double x = 0.0;
    double y = 0.0;
};

/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out)
        : out(out) {
    }

    RenderContext(std::ostream& out, int indent_step, int indent = 0)
        : out(out)
        , indent_step(indent_step)
        , indent(indent) {
    }

    RenderContext Indented() const {
        return {out, indent_step, indent + indent_step};
    }

    void RenderIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
public:
    void Render(const RenderContext& context) const;

    virtual ~Object() = default;

private:
    virtual void RenderObject(const RenderContext& context) const = 0;
};

enum class StrokeLineCap {
    BUTT,
    ROUND,
    SQUARE
};

inline std::ostream& operator<<(std::ostream& out, const StrokeLineCap linecap) {
    if (linecap == StrokeLineCap::BUTT) {
        out << "butt";
    }
    else if (linecap == StrokeLineCap::ROUND) {
        out << "round";
    }
    else if (linecap == StrokeLineCap::SQUARE) {
        out << "square";
    }
    return out;
}

enum class StrokeLineJoin {
    ARCS,
    BEVEL,
    MITER,
    MITER_CLIP,
    ROUND
};

inline std::ostream& operator<<(std::ostream& out, const StrokeLineJoin linejoin) {
    if (linejoin == StrokeLineJoin::ARCS) {
        out << "arcs";
    }
    else if (linejoin == StrokeLineJoin::BEVEL) {
        out << "bevel";
    }
    else if (linejoin == StrokeLineJoin::MITER) {
        out << "miter";
    }
    else if (linejoin == StrokeLineJoin::MITER_CLIP) {
        out << "miter-clip";
    }
    else if (linejoin == StrokeLineJoin::ROUND) {
        out << "round";
    }
    return out;
}



using Color = std::string;

// Объявив в заголовочном файле константу со спецификатором inline,
// мы сделаем так, что она будет одной на все единицы трансляции,
// которые подключают этот заголовок.
// В противном случае каждая единица трансляции будет использовать свою копию этой константы
inline const Color NoneColor{"none"};

/*
 * Базовый класс PathProps для отображения свойств фигур
 */
template <typename Owner>
class PathProps {
public:

    Owner& SetFillColor(Color color) {
        fill_color_ = std::move(color);
        return AsOwner();
    }
    Owner& SetStrokeColor(Color color) {
        stroke_color_ = std::move(color);
        return AsOwner();
    }
    Owner& SetStrokeWidth(double width) {
        stroke_width_ = std::move(width);
        return AsOwner();
    }
    Owner& SetStrokeLineCap(StrokeLineCap linecap) {
        stroke_linecap_ = std::move(linecap);
        return AsOwner();
    }
    Owner& SetStrokeLineJoin(StrokeLineJoin linejoin) {
        stroke_linejoin_ = std::move(linejoin);
        return AsOwner();
    }

protected:
    ~PathProps() = default;

    void RenderAttrs(std::ostream& out) const {
        using namespace std::literals;

        if (fill_color_) {
            out << " fill=\""sv << *fill_color_ << "\""sv;
            //out << " fill="sv << *fill_color_;
        }
        if (stroke_color_) {
            out << " stroke=\""sv << *stroke_color_ << "\""sv;
            //out << " stroke="sv << *stroke_color_;
        }
        if (stroke_width_) {
            out << " stroke-width=\""sv << *stroke_width_ << "\""sv;
        }
        if (stroke_linecap_) {
            out << " stroke-linecap=\""sv << *stroke_linecap_ << "\""sv;
        }
        if (stroke_linejoin_) {
            out << " stroke-linejoin=\""sv << *stroke_linejoin_ << "\""sv;
        }

    }

private:
    Owner& AsOwner() {
        // static_cast безопасно преобразует *this к Owner&,
        // если класс Owner — наследник PathProps
        return static_cast<Owner&>(*this);
    }

    std::optional<Color> fill_color_;
    std::optional<Color> stroke_color_;
    std::optional<double> stroke_width_;
    std::optional<StrokeLineCap> stroke_linecap_;
    std::optional<StrokeLineJoin> stroke_linejoin_;
};


/*
 * Класс Circle моделирует элемент <circle> для отображения круга
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
 */
class Circle final : public Object, public PathProps<Circle> {
public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);

private:
    void RenderObject(const RenderContext& context) const override;

    Point center_;
    double radius_ = 1.0;
};

/*
 * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
 */
class Polyline : public Object, public PathProps<Polyline> {
public:
    // Добавляет очередную вершину к ломаной линии
    Polyline& AddPoint(Point point);

private:
    void RenderObject(const RenderContext& context) const override;

    std::list<Point> points_;
};

/*
 * Класс Text моделирует элемент <text> для отображения текста
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
 */
class Text : public Object, public PathProps<Text> {
public:
    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(Point pos);

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(Point offset);

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontSize(uint32_t size);

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family);

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight);

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data);

private:
    void RenderObject(const RenderContext& context) const override;

    Point pos_ = {0.0, 0.0};
    Point offset_ = {0.0, 0.0};
    uint32_t font_size_ = 1;
    std::string font_family_;
    std::string font_weight_;
    std::string data_ = "";
    std::unordered_map<char, std::string> spec_symbols_ = {
        {'\"', "&quot;"},
        {'<', "&lt;"},
        {'>', "&gt;"},
        {'\'', "&apos;"},
        {'&', "&amp;"}
    };

};

class ObjectContainer {
public :
    virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

    template <typename Obj>
    void Add(Obj obj) {
        AddPtr(std::make_unique<Obj>(std::move(obj)));
    }

protected:
    ~ObjectContainer() = default;
};

class Document : public ObjectContainer {
public:

    // Добавляет в svg-документ объект-наследник svg::Object
    void AddPtr(std::unique_ptr<Object>&& obj) override;

    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;

private:
    std::list<std::unique_ptr<Object>> objects_;
};


class Drawable {
public:
    virtual void Draw(ObjectContainer& container) const = 0;

    virtual ~Drawable() = default;
};

}  // namespace svg
