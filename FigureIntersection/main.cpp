#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

class Point {
public:
    Point(int64_t x, int64_t y) : x_(x), y_(y) {}

    bool operator<(const Point& poinTest) const {
        return (x_ < point.x_) || (x_ == point.x_ && y_ < point.y_);
    }

    bool operator==(const Point& point) const {
        return (x_ == point.x_) && (y_ == point.y_);
    }

    Point operator-(const Point& point) const {
        return {x_ - point.x_, y_ - point.y_};
    }

    int64_t SquareLength() const {
        return x_ * x_ + y_ * y_;
    }

    int64_t Cross(const Point& point) const {
        return x_ * point.y_ - y_ * point.x_;
    }

    bool IsOnLine(const Point& point) const {
        return Cross(point) == 0;
    }

    bool IsOnSegment(const Point& point) const {
        return IsOnLine(point) && SquareLength() >= std::max(point.SquareLength(), (*this - point).SquareLength());
    }

private:
    int64_t x_;
    int64_t y_;
};

class Segment {
public:
    Segment(const Point& start_point, const Point& end_point) : start_point_(start_point),
                                                                end_point_(end_point) {}

    bool IsOnSegment(const Point& point) const {
        return (end_point_ - start_point_).IsOnSegment(point - start_point_);
    }

    bool IsOnBothSides(const Segment& segment) const {
        return (end_point_ - start_point_).Cross(segment.start_point_ - start_point_) *
               (end_point_ - start_point_).Cross(segment.end_point_ - start_point_) < 0;
    }

    bool IsIntersect(const Segment& segment) {
        if (IsOnBothSides(segment) && segment.IsOnBothSides(*this)) {
            return true;
        }

        if (IsOnSegment(segment.start_point_) || IsOnSegment(segment.end_point_) ||
            segment.IsOnSegment(start_point_) || segment.IsOnSegment(end_point_)) {
            return true;
        }

        return false;
    }

private:
    Point start_point_;
    Point end_point_;
};

class Figure {
public:
    static Figure ReadFigureFromFile(const std::string& file_name) {
        Figure figure;
        std::ifstream fin(file_name);
        fin >> figure.points_count_;
        int64_t x = 0, y = 0;
        for (int i = 0; i < figure.points_count_; ++i) {
            fin >> x >> y;
            figure.points_.emplace_back(x, y);
        }
        return figure;
    }

    bool IsIntersect(const Figure& other_figure) {
        for (int i = 0; i < points_count_; ++i) {
            for (int j = 0; j < other_figure.points_count_; ++j) {
                bool is_intersect =
                        Segment(points_[i],
                                points_[(i + 1) % points_count_]).IsIntersect(
                                Segment(other_figure.points_[j],
                                        other_figure.points_[(j + other_figure.points_count_) %
                                                             other_figure.points_count_]));
                if (is_intersect) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    std::vector<Point> points_;
    size_t points_count_;

    Figure() : points_({}), points_count_(0) {}

};

int main() {
    Figure first_figure = Figure::ReadFigureFromFile("first_figure.txt");
    Figure second_figure = Figure::ReadFigureFromFile("second_figure.txt");

    bool is_intersect = first_figure.IsIntersect(second_figure);

    std::cout << is_intersect;
    return 0;
}