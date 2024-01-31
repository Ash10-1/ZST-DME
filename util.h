//
// Created by 13504 on 2023/11/22.
//
#ifndef ZST_DME_UTIL_H
#define ZST_DME_UTIL_H
#include <cstdbool>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <unordered_map>

typedef struct Point {
    double x, y;
    Point(){
        x = 0.0;
        y = 0.0;
    }
    Point(double x1, double y1){
        x=x1;
        y=y1;
    }
    void print() const {
        std::cout << "Point(" << x << ", " << y << ")" ;
    }
}Point;
typedef struct LineSegment{
    Point p1,p2;
    LineSegment(Point a, Point b){
        p1=a;
        p2=b;
    }
};
typedef struct Trr{
    Point top;
    Point right;
    Point bottom;
    Point left;
}Trr;
typedef struct TreeNode{
    int node_id;
    Point p;//如果该节点为ms的话，p则为ms的起始节点
    Point ms_end;
    double radius;
    std::shared_ptr<TreeNode>  father;
    std::shared_ptr<TreeNode> left_child;
    std::shared_ptr<TreeNode> right_child;
    bool type; //1表示当前节点为ms，0表示当前节点为sink
    double delay;//表示该几点到引脚的时延
    double capacitance;
    TreeNode(const TreeNode& a){
        p=a.p;
        ms_end=a.ms_end;
        radius=a.radius;
        father=a.father;
        left_child=a.left_child;
        right_child=a.right_child;
        delay=a.delay;
        capacitance=a.capacitance;
    }
    TreeNode(){
        right_child= nullptr;
        left_child= nullptr;
        father= nullptr;
        type= false;
        delay=0;
        radius=-1;
        capacitance=0;
    }
    TreeNode(Point point){
        p = point;
        right_child= nullptr;
        left_child= nullptr;
        father= nullptr;
        type= false;
        delay=0;
        radius=-1;
        capacitance=0;
    }
}TreeNode;
typedef struct Triple{
    std::shared_ptr<TreeNode> mergenode;
    std::shared_ptr<TreeNode> leftnode;
    std::shared_ptr<TreeNode> rightnode;
    Triple(std::shared_ptr<TreeNode> node1, std::shared_ptr<TreeNode> node2){
        leftnode = node1;
        rightnode = node2;
    }
    Triple(){

    }
}Triple;

bool equals(std::shared_ptr<TreeNode> a, std::shared_ptr<TreeNode> b);
std::shared_ptr<TreeNode> get_brother(std::shared_ptr<TreeNode> root);
bool onSegment(const Point &p, const LineSegment &ls);
double manhattanDistance(Point p1, Point p2);
bool doubleEquals(double a, double b);
bool project_point_on_line_segment(Point p, LineSegment ls, Point &projection);
double cal_dis(std::shared_ptr<Triple> pair);
double minDistanceToSegment(const Point &p, const LineSegment &ls);
double minDistanceBetweenSegments(const LineSegment &ls1, const LineSegment &ls2);
#endif //ZST_DME_UTIL_H
