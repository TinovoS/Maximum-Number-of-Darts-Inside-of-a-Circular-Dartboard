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

struct SweepEvent {
    int pointIndex;
    double angle;
    int type;
    int dartIndex;
};

std::vector<Point> findCircleCenters(Point a, Point b, double r);
std::vector<std::vector<SweepEvent>> getSweepEvents(const std::vector<Point>& darts, double r);
int countInside(const std::vector<Point>& darts, double cx, double cy, double r);
Result angularSweep(const std::vector<Point>& darts, double r);
Result bruteForce(const std::vector<Point>& darts, double r);

#endif // ALGORITHM_H
