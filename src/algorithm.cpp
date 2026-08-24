#include "algorithm.h"
#include <cmath>
#include <algorithm>

std::vector<Point> findCircleCenters(Point a, Point b, double r)
{
    std::vector<Point> centers;
    double dx = b.x - a.x;
    double dy = b.y - a.y;
double d = std::sqrt(dx*dx + dy*dy);

    if (d > 2.0 * r || d < 1e-18)
        return centers;

    double mx = (a.x + b.x) / 2.0;
    double my = (a.y + b.y) / 2.0;
    double h = std::sqrt(r * r - (d/2.0) * (d/2.0));

    double ux = dx/d;
    double uy = dy/d;

    centers.push_back({mx+h*uy, my-h*ux});
    centers.push_back({mx-h*uy, my+h*ux});

    return centers;
}

int countInside(const std::vector<Point>& darts, double cx, double cy, double r)
{
    int count = 0;
    for (const auto& p : darts) {
        double dx = p.x - cx;
        double dy = p.y - cy;
        if (dx*dx + dy*dy <= r*r + 1e-9)
            ++count;
    }

    return count;
}

Result bruteForce(const std::vector<Point>& darts, double r)
{
    Result result;
    int n = static_cast<int>(darts.size());
    result.maxDarts = 1;
    result.cx = darts[0].x;
    result.cy = darts[0].y;
    result.inside.assign(n, false);
    result.inside[0] = true;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            auto centers = findCircleCenters(darts[i], darts[j], r);
            for (const auto& c : centers) {
                int count = countInside(darts, c.x, c.y, r);
                if (count > result.maxDarts) {
                    result.maxDarts = count;
                    result.cx = c.x;
                    result.cy = c.y;
                }
            }
        }
    }
    result.inside.assign(n, false);
    for (int i = 0; i<n; ++i) {
        double dx = darts[i].x - result.cx;
        double dy = darts[i].y - result.cy;
        if (dx*dx + dy*dy <= r*r + 1e-9)
            result.inside[i] = true;
    }   
    return result;
}

Result angularSweep(const std::vector<Point>& darts, double r)
{
    int n = static_cast<int>(darts.size());
    Result best;
    best.maxDarts = 1;
    best.cx = darts[0].x;
    best.cy = darts[0].y;

    for(int i = 0; i < n; ++i) {
        std::vector<std::pair<double, int>> events; 

        for (int j = 0; j < n; ++j) {
            if (i==j) continue;
            double dx = darts[j].x - darts[i].x;
            double dy = darts[j].y - darts[i].y;
            double d = std::sqrt(dx*dx + dy*dy);

            if (d > 2.0 * r) continue;

            double angle = std::atan2(dy, dx);
            double delta = std::acos(d / (2.0 * r));

            events.push_back({angle - delta, 1});
            events.push_back({angle + delta, -1});
        }

        std::sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
        if (std::abs(a.first - b.first) < 1e-9)
            return a.second > b.second;
        return a.first < b.first;
        });

        int count = 1;
        int maxCount = 1;
        double bestAngle = 0;

        for (const auto& e : events) {
            count += e.second;
            if (count > maxCount) {
                maxCount = count;
                bestAngle = e.first;
            }
        }

        if (maxCount > best.maxDarts) {
            best.maxDarts = maxCount;
            best.cx = darts[i].x + r * std::cos(bestAngle);
            best.cy = darts[i].y + r * std::sin(bestAngle);
        }
    }

    int nn = n;
    best.inside.assign(nn, false);
    for (int i = 0; i< nn; ++i) {
        double dx = darts[i].x - best.cx;
        double dy = darts[i].y - best.cy;
        if (dx*dx + dy*dy <= r*r + 1e-9)
            best.inside[i] = true;
    }
    return best;
}

std::vector<std::vector<SweepEvent>> getSweepEvents(const std::vector<Point>& darts, double r)
{
    int n = static_cast<int>(darts.size());
    std::vector<std::vector<SweepEvent>> allEvents(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            double dx = darts[j].x - darts[i].x;
            double dy = darts[j].y - darts[i].y;
            double d = std::sqrt(dx * dx + dy * dy);

            if (d > 2.0 * r) continue;

            double angle = std::atan2(dy, dx);
            double delta = std::acos(d / (2.0 * r));

            allEvents[i].push_back({i, angle - delta, 1, j});
            allEvents[i].push_back({i, angle + delta, -1, j});
        }

        std::sort(allEvents[i].begin(), allEvents[i].end(),
            [](const SweepEvent& a, const SweepEvent& b) {
                if (std::abs(a.angle - b.angle) < 1e-9)
                    return a.type > b.type;
                return a.angle < b.angle;
            });
    }

    return allEvents;
}
