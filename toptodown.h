//
// Created by 13504 on 2023/12/12.
//
#include "util.h"
#include "vector"
#ifndef ZST_DME_TOPTODOWN_H
#define ZST_DME_TOPTODOWN_H
bool doubleEquals(double a, double b);
Point find_exact_point(TreeNode &segment, Point p);
bool isOnLineSegment(const Point &point, const Point &end1, const Point &end2);
std::vector<Point> calculateIntersections(const Point &lineStart, const Point &lineEnd,const Point &center, double d);
#endif //ZST_DME_TOPTODOWN_H
