//
// Created by zhh on 2023/12/27.
//

#ifndef ZST_DME_ELMOREDELAY_H
#define ZST_DME_ELMOREDELAY_H
#include "util.h"
#include "reader.h"

class ElmoreDelay {
private:
    double calNodeCap(std::shared_ptr<TreeNode> node);
    double calNodeDelay(std::shared_ptr<TreeNode> node);
public:
    double calTrrRadius(std::shared_ptr<TreeNode> node_a, std::shared_ptr<TreeNode> node_b, double dis);
    double calTrrRadiusBeyondKa(std::shared_ptr<TreeNode> node_a, std::shared_ptr<TreeNode> node_b);
    double calTrrRadiusBeyondKb(std::shared_ptr<TreeNode> node_a, std::shared_ptr<TreeNode> node_b);
};


#endif //ZST_DME_ELMOREDELAY_H
