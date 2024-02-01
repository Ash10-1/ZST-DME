//
// Created by zhh on 2024/1/30.
//

#ifndef ZST_DME_CLOCK_H
#define ZST_DME_CLOCK_H


#include <vector>
#include <unordered_set>
#include "util.h"

class Clock {
public:
    std::shared_ptr<TreeNode> tree_build(std::string bench_path, const std::string topo_path);

};


#endif //ZST_DME_CLOCK_H
