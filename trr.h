//
// Created by 13504 on 2023/11/22.
//

#ifndef ZST_DME_TRR_H
#define ZST_DME_TRR_H

#include "util.h"
#include "ElmoreDelay.h"

bool cal_linear_delays(std::shared_ptr<Triple> pair);
bool cal_elmore_delays(std::shared_ptr<Triple> pair);
double cal_radius(double delaya,double delayb,double distance);
Trr cal_trr(std::shared_ptr<TreeNode> node, double radius);
bool isOverlap(LineSegment ls1, LineSegment ls2, LineSegment &overlap);
std::shared_ptr<TreeNode> cal_merge_segment(std::shared_ptr<Triple> pair);
LineSegment cal_ms_cost_beyond_k(double radius, std::shared_ptr<TreeNode> n1, std::shared_ptr<TreeNode> n2);
#endif //ZST_DME_TRR_H
