//
// Created by zhh on 2024/1/30.
//

#ifndef ZST_DME_FILEOPERATION_H
#define ZST_DME_FILEOPERATION_H
#include "vector"
#include "util.h"
extern int numpins;
extern double per_unit_resistance;
extern double per_unit_capacitance;


class FileOperation {
public:
    std::vector<Point> read(const std::string& filename, std::vector<double>& caps);
    void generate_netlist(const char* filename, std::shared_ptr<TreeNode> root);
    void generate_netlist_op(FILE* file, std::shared_ptr<TreeNode> node);
    void init();
};


#endif //ZST_DME_FILEOPERATION_H
