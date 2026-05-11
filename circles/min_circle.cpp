#include <vector>
#include <cmath>
#include <random>
#include "min_circle.h"

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment>& segments){
    if (segments.empty()){
        return Circle(Point2D(0, 0), -1);
    }
    
    std::vector<Point2D> points = extractPointsFromSegments(segments);
    return minCircle(points);  
}

double distanceSquared(const Point2D& p1, const Point2D& p2){
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

double distance(const Point2D& p1, const Point2D& p2){
    return sqrt(distanceSquared(p1, p2));
}

Circle circleFromDiameter(const Point2D& p1, const Point2D& p2){
    return Circle(findMidPoint(LineSegment(p1, p2)), distance(p1, p2)/2.0);
}

Circle circleFrom3Points(const Point2D& p1, const Point2D& p2, const Point2D& p3){
    double side1 = distance(p1, p2);   
    double side2 = distance(p1, p3);   
    double side3 = distance(p2, p3);  

    double half_per = (side1 + side2 + side3)/2;
    double s = sqrt(half_per * (half_per - side1) * (half_per - side2) * (half_per - side3));
    double r = (side1 * side2 * side3) / (4 * s);
    
    double des = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
    if (std::abs(des) < 1e-8){
        Circle c12 = circleFromDiameter(p1, p2);
        Circle c13 = circleFromDiameter(p1, p3);
        Circle c23 = circleFromDiameter(p2, p3);

        Circle best = c12;
        if (c13.radius > best.radius) best = c13;
        if (c23.radius > best.radius) best = c23;
        return best;
    }

    Point2D center;
    center.x = ((pow(p1.x, 2) + pow(p1.y, 2)) * (p2.y - p3.y) + (pow(p2.x, 2) + pow(p2.y, 2)) * (p3.y - p1.y) + (pow(p3.x, 2) + pow(p3.y, 2)) * (p1.y - p2.y))/des;
    center.y = ((pow(p1.x, 2) + pow(p1.y, 2)) * (p3.x - p2.x) + (pow(p2.x, 2) + pow(p2.y, 2)) * (p1.x - p3.x) + (pow(p3.x, 2) + pow(p3.y, 2)) * (p2.x - p1.x))/des;

    return Circle(center, r);
}

static std::mt19937 gen(std::random_device{}());

Circle minCircleHelper(std::vector<Point2D>& points, std::vector<Point2D> boundary, size_t n){
    if (n == 0 || boundary.size() == 3){
        switch (boundary.size()){
	    case 0:
	        return Circle();
	    case 1:
	        return Circle(boundary[0], 0);
	    case 2:
	        return circleFromDiameter(boundary[0], boundary[1]);
	    case 3:
	        return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
	}
    }
    std::uniform_int_distribution<> dist(0, n - 1);
    int rand_ind = dist(gen);

    Point2D rand_p = points[rand_ind];
    std::swap(points[rand_ind], points[n - 1]);

    Circle min_circle = minCircleHelper(points, boundary, n - 1);

    if (min_circle.isValid() && min_circle.contains(rand_p)){
        return min_circle;
    }
    
    boundary.push_back(rand_p);
    return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points){
    return minCircleHelper(points, {}, points.size());
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment>& segments){
    std::vector<Point2D> points;
    for (auto segment : segments){
        points.push_back(segment.start);
	points.push_back(segment.end);
    }
    return points;
}

Point2D findMidPoint(const LineSegment& segment){
    Point2D p1 = segment.start;
    Point2D p2 = segment.end;

    return Point2D((p1.x + p2.x)/2.0, (p1.y + p2.y)/2.0);
}
