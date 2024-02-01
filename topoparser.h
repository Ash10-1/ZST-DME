#ifndef ZST_DME_TOPOPARSER_H
#define ZST_DME_TOPOPARSER_H

#include <memory>
#include "util.h"
#include "vector"

std::shared_ptr<TreeNode> tree_build(std::string bench_path, const std::string topo_path);

#endif //ZST_DME_TOPOPARSER_H
