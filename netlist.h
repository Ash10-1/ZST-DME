//
// Created by 13504 on 2023/11/22.
//

#ifndef ZST_DME_NETLIST_H
#define ZST_DME_NETLIST_H

#include <cstdio>
#include "util.h"

void generate_netlist(const char* filename, std::shared_ptr<TreeNode> root);
void generate_netlist_op(FILE* file, std::shared_ptr<TreeNode> node);


#endif //ZST_DME_NETLIST_H
