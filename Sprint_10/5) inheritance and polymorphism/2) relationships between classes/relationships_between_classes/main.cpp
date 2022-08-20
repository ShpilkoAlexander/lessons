#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

class House {
public:
    House(int length, int width, int heigth) :
            length_(length), width_(width), heigth_(heigth) {
    }
    int GetLength() const {
        return length_;
    }
    int GetWidth() const {
        return width_;
    }
    int GetHeight() const {
        return heigth_;
    }

private:
    int length_ = 0;
    int width_ = 0;
    int heigth_ = 0;
};

class Resources {
public:
    Resources(int brick_count) :
        brick_count_(brick_count) {
    }

    void TakeBricks(int count) {
        if (count < 0 || brick_count_ < count) {
            throw out_of_range("");
        }

        else {
            brick_count_ -= count;
        }
    }

    int GetBrickCount() const {
        return brick_count_;
    }


private:
    int brick_count_ = 0;


};

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class Builder {
public:
    Builder(Resources& resources) :
        resources_(resources) {
    }

    House BuildHouse(HouseSpecification spec) {
        int bricks_needed = 4 * (((spec.length + spec.width) * 2) * 8 * spec.height);

        if (bricks_needed > resources_.GetBrickCount()) {
            throw runtime_error("");
        }

        resources_.TakeBricks(bricks_needed);
        return House{spec.length, spec.width, spec.height};
    }

private:
    Resources& resources_;

};

int main() {
    Resources resources{10000};
    Builder builder1{resources};
    Builder builder2{resources};

    House house1 = builder1.BuildHouse(HouseSpecification{12, 9, 3});
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House house2 = builder2.BuildHouse(HouseSpecification{8, 6, 3});
    assert(house2.GetLength() == 8);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House banya = builder1.BuildHouse(HouseSpecification{4, 3, 2});
    assert(banya.GetHeight() == 2);
    cout << resources.GetBrickCount() << " bricks left"s << endl;
}
