#include <valarray>
#include "vector"
#include "trr.h"
#include "toptodown.h"

using namespace std;
//double比较的精度，用于double之间的比较
const double EPS = 1e-6;


/**
*线性模型时延
*@param pair 传入一个合并节点
*@return 合并代价是否超过k
**/
bool cal_linear_delays(std::shared_ptr<Triple> pair){
    std::shared_ptr<TreeNode> n1 = pair->leftnode;
    std::shared_ptr<TreeNode> n2 = pair->rightnode;

    double delay_n1 = 0;
    if (n1->type){
        delay_n1 = manhattanDistance(n1->p, n1->left_child->p);
        delay_n1+=n1->left_child->delay;
    }
    n1->delay=delay_n1;

    double delay_n2 = 0;
    if (n2->type){
        delay_n2 = manhattanDistance(n2->p, n2->left_child->p);
        delay_n2+=n2->left_child->delay;
    }
    n2->delay=delay_n2;

    //计算节点n1和n2之间的距离
    double dis = cal_dis(pair);
    //计算radius
    double radius_a = cal_radius(delay_n1,delay_n2,dis);
    double radius_b = dis-radius_a;
    if (radius_a < 0){
        //说明n1的merging cost超过了k
        n1->radius=0;
        n2->radius= delay_n1 - delay_n2;
        //如果n1是一个节点，则把n1作为merging segment直接返回
        if (!n1->type){
            TreeNode node(*n1);
            auto ptr = std::make_shared<TreeNode>(node);
            ptr->ms_end = n1->p;
            pair->mergenode = ptr;
            return true;
        }
        //如果n1是一条线段，计算merging segment
        LineSegment mergeSegment = cal_ms_cost_beyond_k(dis,n2,n1);
        pair->mergenode->p=mergeSegment.p1;
        pair->mergenode->ms_end=mergeSegment.p2;
        return true;
    }
    if (radius_b < 0){
        //说明n2的merging cost超过了k
        n2->radius=0;
        n1->radius = delay_n2 - delay_n1;
        //如果n2是一个节点，则把n2作为merging segment直接返回
        if (!n2->type){
            TreeNode node(*n2);
            auto ptr = std::make_shared<TreeNode>(node);
            ptr->ms_end = n2->p;
            pair->mergenode = ptr;
            return true;
        }
        //如果n2是一条线段，计算merging segment
        LineSegment mergeSegment = cal_ms_cost_beyond_k(dis,n1,n2);
        pair->mergenode->p=mergeSegment.p1;
        pair->mergenode->ms_end=mergeSegment.p2;
        return true;
    }
    //保存n1和n2的半径，用于后续从上到下的计算
    n1->radius=radius_a;
    n2->radius=radius_b;
    return false;

}
/**
*Elmore模型时延，同时计算trr的半径
*@param pair 传入一个合并节点
*@return 合并代价是否超过k
**/
bool cal_elmore_delays(std::shared_ptr<Triple> pair){
    ElmoreDelay elmoreDelay;
    std::shared_ptr<TreeNode> n1 = pair->leftnode;
    std::shared_ptr<TreeNode> n2 = pair->rightnode;
     double dis = cal_dis(pair);
    //计算两个节点各自的trr半径
    double radius_a = elmoreDelay.calTrrRadius(n1, n2,  dis);
    double radius_b = dis - radius_a;
    //如果节点的合并代价大于k
    if (radius_a > dis){
        radius_b = 0;
        radius_a = elmoreDelay.calTrrRadiusBeyondKb(n1, n2);
        //保存n1和n2的半径
        n1->radius = radius_a;
        n2->radius = radius_b;
        //如果n2是一个节点，直接把n2作为ms返回
        if (!n2->type || (fabs(n2->p.x - n2->ms_end.x) < EPS && fabs(n2->p.y - n2->ms_end.y) < EPS)){
            TreeNode node(*n2);
            auto ptr = std::make_shared<TreeNode>(node);
            ptr->ms_end = n2->p;
            ptr->type = false;
            pair->mergenode = ptr;
            return true;
        }
        LineSegment merge_segment = cal_ms_cost_beyond_k(radius_a, n1, n2);
        Point p1 = merge_segment.p1;
        Point p2 = merge_segment.p2;
        if ((fabs(p1.y - p2.y) < EPS ) && (fabs(p1.x - p2.x) < EPS )){
            pair->mergenode->type = false;
        }
        pair->mergenode->p = merge_segment.p1;
        pair->mergenode->ms_end = merge_segment.p2;
        return true;
    }
    if (radius_b > dis){
        radius_a = 0;
        radius_b = elmoreDelay.calTrrRadiusBeyondKa(n1, n2);
        //保存n1和n2的半径
        n1->radius = radius_a;
        n2->radius = radius_b;
        //如果n1是一个节点，直接把n1作为ms返回
        if (!n1->type || (fabs(n1->p.x - n1->ms_end.x) < EPS && fabs(n1->p.y - n1->ms_end.y) < EPS)){
            TreeNode node(*n1);
            auto ptr = std::make_shared<TreeNode>(node);
            ptr->ms_end = n1->p;
            ptr->type = false;
            pair->mergenode = ptr;
            return true;
        }
        LineSegment merge_segment = cal_ms_cost_beyond_k(radius_b, n2, n1);
        Point p1 = merge_segment.p1;
        Point p2 = merge_segment.p2;
        if ((fabs(p1.y - p2.y) < EPS ) && (fabs(p1.x - p2.x) < EPS )){
            pair->mergenode->type = false;
        }
        pair->mergenode->p = merge_segment.p1;
        pair->mergenode->ms_end = merge_segment.p2;
        return true;
    }
    n1->radius = radius_a;
    n2->radius = radius_b;
    return false;
}
/**
*计算线性模型TRR半径
*@param delaya 节点a的时延
*@param delayb 节点b的时延
*@param distance 节点ab之间的距离
*@return TRR的半径
**/
double cal_radius(double delaya,double delayb,double distance){
    double trr_a = (distance + delayb - delaya) / 2;
    return trr_a;
}
/**
*TRR计算
*@param node 传入一个节点或者segment
*@return 一个节点或线段的TRR
**/
Trr cal_trr(std::shared_ptr<TreeNode> node, double radius){
    Trr trr ;

    double x = node->p.x;
    double y = node->p.y;
    //sink的TRR计算
    if(!node->type){
        trr.top = Point(x,y+radius);
        trr.right = Point(x+radius,y);
        trr.bottom = Point(x , y-radius);
        trr.left = Point(x-radius,y);
        return trr;
    }else{
        double x1 = node->ms_end.x;
        double y1 = node->ms_end.y;
        //计算线段的斜率
        double slope = (y1 -y) / (x1 -x);
        if (fabs(slope-1) < EPS){
            trr.top = Point(x,y+radius);
            trr.right = Point(x+radius,y);
            trr.bottom =Point(x1 , y1-radius);
            trr.left = Point(x1-radius,y1);
        } else{
            trr.top = Point(x,y+radius);
            trr.left = Point(x-radius,y);
            trr.bottom =Point(x1 , y1-radius);
            trr.right = Point(x1+radius,y1);
        }

        return trr;
    }
}
/**
*求两个线段之间的重叠部分
*@param seg1 线段1
*@param seg2 线段2
*@param &overlap 重叠线段
*@return 是否重叠
**/
bool isOverlap(LineSegment ls1, LineSegment ls2, LineSegment &overlap) {
    // Sort points of the line segments based on x-coordinate
    if (ls1.p1.x > ls1.p2.x) std::swap(ls1.p1, ls1.p2);
    if (ls2.p1.x > ls2.p2.x) std::swap(ls2.p1, ls2.p2);

    // Check for overlap
    if (ls1.p2.x < ls2.p1.x || ls2.p2.x < ls1.p1.x) {
        return false; // No overlap
    }

    // Calculate the overlap segment
    Point start = ls1.p1.x > ls2.p1.x ? ls1.p1 : ls2.p1;
    Point end = ls1.p2.x < ls2.p2.x ? ls1.p2 : ls2.p2;

    overlap = LineSegment(start, end);
    return true;
}
/**
* 当合并代价超过k（两个节点之间的距离）时计算merging segment
*@param radius 两个节点之间距离，作为trr的半径
*@param n1 作为计算Trr的节点
*@param n2 Trr半径为0的节点
*@return 返回merging segment
**/
LineSegment cal_ms_cost_beyond_k(double radius, std::shared_ptr<TreeNode> n1, std::shared_ptr<TreeNode> n2){
    Trr trr = cal_trr(n1, radius);
    vector<Point> intersections;
    Point lineStart = n2->p;
    Point lineEnd = n2->ms_end;
    double m = (lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x); // 斜率

    // trr四条边
    LineSegment diamondEdges[4] = {{trr.top, trr.right}, {trr.right, trr.bottom}, {trr.bottom, trr.left}, {trr.left, trr.top}};

    for (int i = 0; i < 4; ++i) {
        Point p1 = diamondEdges[i].p1;
        Point p2 = diamondEdges[i].p2;

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
        if (n1->type){//如果n1是一条线段，计算点到线段的距离
            LineSegment line(n1->p, n1->ms_end);
            double d1 = minDistanceToSegment(p1, line);
            if ((d1 < radius)||fabs(d1-radius) < EPS){
                intersections.push_back(p1);
            }
            double d2 = minDistanceToSegment(p2, line);
            if (d2 < radius || fabs(d2 - radius) < EPS)
                intersections.push_back(p2);
        }else{//如果n1是一个点，计算点之间的距离
            Point center = n1->p;
            double d1 = manhattanDistance(p1,center);
            if (d1 < radius||fabs(d1-radius) < EPS){
                intersections.push_back(p1);
            }
            double d2 = manhattanDistance(p2,center);
            if (d2 < radius||fabs(d2-radius) < EPS){
                intersections.push_back(p2);
            }
        }
    }
    assert(intersections.size() == 2);
    //如果计算出的合并线段是一个点
    Point p1 = intersections[0];
    Point p2 = intersections[1];

    if (p1.y > p2.y){
        return LineSegment(p1,p2);
    } else{
        return LineSegment(p2,p1);
    }
}


/**
*合并线段计算(只适用于节点数量为偶数的情况下）
*@param node 传入一对节点或者segment
*@return 节点或者segment之间的合并线段
**/
std::shared_ptr<TreeNode> cal_merge_segment(std::shared_ptr<Triple> pair){

    std::shared_ptr<TreeNode> n1 = pair->leftnode;
    std::shared_ptr<TreeNode> n2 = pair->rightnode;
    //使用elmore模型计算时延
    bool flag = cal_elmore_delays(pair);

//    //使用线性模型计算时延
//    bool flag = cal_linear_delays(pair);
    if (flag){
        return nullptr;
    }
    double radius_a = pair->leftnode->radius;
    double radius_b = pair->rightnode->radius;
    //计算n1和n2的trr
    Trr trr1 = cal_trr(n1, radius_a);
    Trr trr2 = cal_trr(n2, radius_b);
    LineSegment mergeSeg{{0.0, 0.0}, {0.0, 0.0}};//初始化merge Segment
    std::vector<LineSegment> rect1 = {{trr1.top,trr1.left},{trr1.left,trr1.bottom},{trr1.bottom,trr1.right},{trr1.right,trr1.top}};
    std::vector<LineSegment> rect2 = {{trr2.top,trr2.left},{trr2.left,trr2.bottom},{trr2.bottom,trr2.right},{trr2.right,trr2.top}};

    for (const LineSegment &seg1: rect1){
        for (const LineSegment &seg2: rect2){
            // 计算第一条线段的斜率
             double slope1 = (seg1.p2.y - seg1.p1.y) / (seg1.p2.x - seg1.p1.x);
            // 计算第二条线段的斜率
            double slope2 = (seg2.p2.y - seg2.p1.y) / (seg2.p2.x - seg2.p1.x);

            if (fabs(slope1 - slope2) > EPS){
                continue;//如果两条直线不平行，这两条直线肯定没有两个交点
            }else{
                //判断截距
                double a = seg1.p1.y - slope1*(seg1.p1.x);
                double b = seg2.p1.y - slope2*(seg2.p1.x);

                if (fabs(a-b) < EPS){//表示两条线段平行且重叠
                    //判断交点
                    isOverlap(seg1,seg2,mergeSeg);
                    //一种边界条件(当计算出的mergesegment是一个点的时候）
                    if ((fabs(mergeSeg.p1.x - mergeSeg.p2.x) < EPS)&&(fabs(mergeSeg.p1.y - mergeSeg.p2.y)) < EPS){
                        pair->mergenode->p = mergeSeg.p1;
                        pair->mergenode->ms_end = mergeSeg.p2;
                        pair->mergenode->type = false;//说明计算出的mergenode是一个点
                        continue;
                    }
                    //保证计算出的ms p顶点在上方，方便后续的trr计算
                    if (mergeSeg.p1.y > mergeSeg.p2.y){
                        pair->mergenode->p = mergeSeg.p1;
                        pair->mergenode->ms_end = mergeSeg.p2;
                    }else{
                        pair->mergenode->p = mergeSeg.p2;
                        pair->mergenode->ms_end = mergeSeg.p1;
                    }
                    pair->mergenode->type = true;

                    return  nullptr;
                }
            }
        }
    }
    return nullptr ;

    //测试代码
//    cout << "Top Point1: (" << trr1.top.x << ", " << trr1.top.y << ")" << "\n";
//    cout << "Right Point1: (" << trr1.right.x << ", " << trr1.right.y << ")" << "\n";
//    cout << "Bottom Point1: (" << trr1.bottom.x << ", " << trr1.bottom.y << ")" << "\n";
//    cout << "Left Point1: (" << trr1.left.x << ", " << trr1.left.y << ")" << "\n";
//
//    cout << "Top Point: (" << trr2.top.x << ", " << trr2.top.y << ")" << "\n";
//    cout << "Right Point: (" << trr2.right.x << ", " << trr2.right.y << ")" << "\n";
//    cout << "Bottom Point: (" << trr2.bottom.x << ", " << trr2.bottom.y << ")" << "\n";
//    cout << "Left Point: (" << trr2.left.x << ", " << trr2.left.y << ")" << "\n";
    //计算n1和n2的合并线段

}


