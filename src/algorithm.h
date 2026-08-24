#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>

struct Point {
    double x, y;
};

struct Result {
    int maxDarts;
    double cx, cy;
    std::vector<bool> inside;
};

std::vector<Point> findCircleCenters(Point a, Point b, double r);
int countInside(const std::vector<Point>& darts, double cx, double cy, double r);
Result angularSweep(const std::vector<Point>& darts, double r);
Result bruteForce(const std::vector<Point>& darts, double r);

#endif // ALGORITHM_H
