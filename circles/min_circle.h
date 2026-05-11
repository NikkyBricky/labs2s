#pragma once

#include <vector>
#include <cmath>

struct Point2D {
    double x, y;
    Point2D(double x1 = 0, double y1 = 0): x(x1), y(y1) {}

    bool operator==(const Point2D& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point2D& other) const {
        return !(*this == other);
    }
};

double distance(const Point2D& p1, const Point2D& p2);

struct LineSegment {
    Point2D start; 
    Point2D end;  

    LineSegment(const Point2D p1, const Point2D p2): start(p1), end(p2) {}
};

struct Circle {
    Point2D center; 
    double radius;  
    
    Circle(const Point2D c = Point2D(0, 0), double r  = 0): center(c), radius(r) {}

    bool isValid() const {
        return radius >= 0;
    }

    bool contains(const Point2D& point) const {
        return distance(center, point) <= radius + 1e-8;
    }

    bool contains(const LineSegment& segment) const {
        return contains(segment.start) && contains(segment.end);
    }

    bool containsEntireSegment(const LineSegment& segment) const {
        return contains(segment);
    }
};

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments);

double distanceSquared(const Point2D& p1, const Point2D& p2);

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2);

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3);

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n);

Circle minCircle(std::vector<Point2D> points);

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments);

Point2D findMidPoint(const LineSegment& segment);
