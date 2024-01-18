#ifndef ZST_DME_TOPOPARSER_H
#define ZST_DME_TOPOPARSER_H

#include <memory>
#include "util.h"
#include "vector"

double calc_dist(std::shared_ptr<TreeNode> , std::shared_ptr<TreeNode> );
double get_nearest(std::vector<std::shared_ptr<TreeNode>>& recur_set , std::vector<std::shared_ptr<TreeNode>>& result,std::vector<int>& iters);
std::shared_ptr<TreeNode> merge_point(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right);
std::shared_ptr<TreeNode> tree_build(std::vector<Point> &sink_set, std::vector<double> caps, const std::string topo_path);

#endif //ZST_DME_TOPOPARSER_H
