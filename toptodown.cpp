//
// Created by 13504 on 2023/12/12.
//
#include <cmath>
#include "toptodown.h"
#include "algorithm"
#include "trr.h"
using namespace std;
const double EPS = 1e-6;


bool isOnLineSegment(const Point &point, const Point &end1, const Point &end2) {
    double minX = std::min(end1.x, end2.x);
    double maxX = std::max(end1.x, end2.x);
    double minY = std::min(end1.y, end2.y);
    double maxY = std::max(end1.y, end2.y);

    return (point.x > minX || doubleEquals(point.x, minX)) &&
           (point.x < maxX || doubleEquals(point.x, maxX)) &&
           (point.y > minY || doubleEquals(point.y, minY)) &&
           (point.y < maxY || doubleEquals(point.y, maxY));
}


// 计算线段与trr边的交点
vector<Point> calculateIntersections(const Point &lineStart, const Point &lineEnd,
                                     const Point &center, double d) {
    vector<Point> intersections;
    double m = (lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x); // 斜率

    // trr四个顶点
    Point top(center.x, center.y + d), right(center.x + d, center.y),
            bottom(center.x, center.y - d), left(center.x - d, center.y);

    // trr四条边
    Point diamondEdges[4][2] = {{top, right}, {right, bottom}, {bottom, left}, {left, top}};

    for (int i = 0; i < 4; ++i) {
        Point p1 = diamondEdges[i][0];
        Point p2 = diamondEdges[i][1];

        // trr边的斜率
        double mDiamond = (p2.y - p1.y) / (p2.x - p1.x);

        // 计算交点
        double x = (m * lineStart.x - lineStart.y - mDiamond * p1.x + p1.y) / (m - mDiamond);
        double y = m * (x - lineStart.x) + lineStart.y;

        Point intersection(x, y);

        // 检查交点是否在线段和trr边界上
        if (isOnLineSegment(intersection, p1, p2) && isOnLineSegment(intersection, lineStart, lineEnd)) {
            intersections.push_back(intersection);
        }
    }
    //如果有一个端点在trr的内部,或者两个端点都在trr的内部
    if (intersections.size() != 2){
        Point p1 = lineStart;
        Point p2 = lineEnd;
        double d1 = manhattanDistance(p1,center);
        if (d1 < d || fabs(d1 - d) < EPS){
            intersections.push_back(p1);
        }
        double d2 = manhattanDistance(p2,center);
        if (d2 < d || fabs(d2 - d) < EPS){
            intersections.push_back(p2);
        }
    }

    return intersections;
}
Point find_exact_point(TreeNode &segment, Point p){
    double radius = segment.radius;
    if (!segment.type){
        return segment.p;
    }
    if (fabs(radius-0)<EPS){
        //如果半径为0
        return p;
    }
    vector<Point> interPoints;
    //计算线段和trr边界上相交的点
    interPoints = calculateIntersections(segment.p, segment.ms_end,p, radius);
    if (interPoints.size()==1){
        return interPoints[0];
    }
    Point p1 = interPoints[0];
    Point p2 = interPoints[1];
    double x = (p1.x+p2.x)/2;
    double y=(p1.y+p2.y)/2;
    return Point (x,y);


}