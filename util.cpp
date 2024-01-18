//
// Created by 13504 on 2023/11/22.
//

#include "util.h"
#include "cmath"
const double EPS = 1e-6;
using namespace std;


/**
 * 判断两个树节点是否代表同一个点(坐标相等)
 * @param a
 * @param b
 * @return bool
 */
bool equals(std::shared_ptr<TreeNode> a, std::shared_ptr<TreeNode> b){
    if(a->p.x == b->p.x && a->p.y == b->p.y){
        return true;
    }else{
        return false;
    }

}
//判断两个浮点数是否相等
bool doubleEquals(double a, double b) {
    return std::abs(a - b) < EPS;
}
std::shared_ptr<TreeNode> get_brother(std::shared_ptr<TreeNode> root){
    if(root->father == nullptr){ //说明这个 root 是整个时钟树的 root 而不单单只是子树的 root, 因而其父节点为空
        return nullptr;
    } else{
        if(root == root->father->left_child){
            return root->father->right_child;
        }else{
            return root->father->left_child;
        }
    }
}

/**
*判断点是否在线段上（基于斜率为1或-1）
*@param Point 一个点
*@param ls 一条线段
*@return 点是否在线段上
**/
bool onSegment(const Point &p, const LineSegment &ls) {
    double min_x = std::min(ls.p1.x, ls.p2.x);
    double max_x = std::max(ls.p1.x, ls.p2.x);
    double min_y = std::min(ls.p1.y, ls.p2.y);
    double max_y = std::max(ls.p1.y, ls.p2.y);

    return (p.x > min_x || doubleEquals(p.x, min_x)) && (p.x < max_x || doubleEquals(p.x, max_x)) &&
           (p.y > min_y || doubleEquals(p.y, min_y)) && (p.y < max_y || doubleEquals(p.y, max_y));
}
//计算两个点之间的曼哈顿距离
double manhattanDistance(Point p1, Point p2) {
    return abs(p1.x - p2.x) + abs(p1.y-p2.y);
}
/**
*计算点到线段的垂直投影，如果投影点不在线段上，返回false
*@param Point 一个点
*@param ls 一条线段
*@return 点到线段的投影点
**/
bool project_point_on_line_segment(Point p, LineSegment ls, Point &projection) {
    Point AP = Point(p.x - ls.p1.x, p.y - ls.p1.y); // 从p1到点p的向量
    Point AB = Point(ls.p2.x - ls.p1.x, ls.p2.y - ls.p1.y); // 线段的向量

    double dot_product = AP.x * AB.x + AP.y * AB.y; // 点积
    double length_AB_squared = AB.x * AB.x + AB.y * AB.y; // AB的长度平方

    double t = dot_product / length_AB_squared; // 归一化距离

    // 检查投影点是否在线段上
    if (t < 0.0 || t > 1.0) {
        return false; // 投影点不在线段上
    }

    // 计算投影点
    projection.x = ls.p1.x + t * AB.x;
    projection.y = ls.p1.y + t * AB.y;

    return true; // 投影点在线段上
}
/**
*计算点到直线的曼哈顿距离
*@param Point 一个点
*@param ls 一条线段
*@return 点到线段的最短曼哈顿距离
**/
double minDistanceToSegment(const Point &p, const LineSegment &ls){
//    double slope = (ls.p2.y - ls.p1.y) / (ls.p2.x - ls.p1.x);
    Point projection = {0, 0};

//    // 计算点到线段的垂直投影
//    if (fabs(slope - 1) < EPS) {
//        projection = Point((p.x + p.y - ls.p1.y + ls.p1.x) / 2, (p.y + p.x - ls.p1.x + ls.p1.y) / 2);
//    } else { // slope == -1
//        projection = Point((p.x - p.y + ls.p1.y + ls.p1.x) / 2, (ls.p1.y - ls.p1.x + p.x + p.y) / 2);
//    }
//
//    // 检查投影是否在线段上
//    if (onSegment(projection, ls)) {
//        return manhattanDistance(p, projection);
//    }
    // 计算点到线段的垂直投影
    bool flag = project_point_on_line_segment(p, ls, projection);
    // 检查投影是否在线段上
    if (flag){
        return manhattanDistance(p, projection);
    }

    // 如果投影不在线段上，计算到两个端点的曼哈顿距离，并取最小值
    return min(manhattanDistance(p, ls.p1), manhattanDistance(p, ls.p2));
}

/**
* 计算两条线段之间的最短曼哈顿距离
*@param ls1 第一条线段
*@param ls2 第二条线段
*@return 点到线段的最短曼哈顿距离
**/
double minDistanceBetweenSegments(const LineSegment &ls1, const LineSegment &ls2) {
    // 计算ls1的端点到ls2的距离
    double distance1 = std::min(minDistanceToSegment(ls1.p1, ls2),
                                minDistanceToSegment(ls1.p2, ls2));
    double distance2 = std::min(minDistanceToSegment(ls2.p1, ls1),
                                minDistanceToSegment(ls2.p2, ls1));

    // 返回这四个距离中的最小值
    return std::min(distance1, distance2);
}
/**
*计算一对sink或者segment之间的最短距离
*@param pair 存储二叉树生成的路径的数组，即路径数组(见readme文件中算法步骤部分)
*@return 最短距离
**/
double cal_dis(std::shared_ptr<Triple> pair){
    std::shared_ptr<TreeNode> n1 = pair->leftnode;
    std::shared_ptr<TreeNode> n2 = pair->rightnode;
    //两个节点都是sink
    if(!n1->type && !n2->type ){
        Point p1 = n1->p;
        Point p2 = n2->p;
        return manhattanDistance(p1, p2);
    }
        //两个节点都是segment
    else if(n1->type && n2->type){
        LineSegment ls1(n1->p,n1->ms_end);
        LineSegment ls2(n2->p,n2->ms_end);
        return minDistanceBetweenSegments(ls1, ls2);
    }
        //其中一个节点是segment
    else{
        if(n1->type){
            LineSegment ls(n1->p,n1->ms_end);
            return minDistanceToSegment(n2->p,ls);
        } else{
            LineSegment ls(n2->p,n2->ms_end);
            return minDistanceToSegment(n1->p,ls);
        }
    }
}