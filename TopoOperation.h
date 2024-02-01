//
// Created by zhh on 2024/1/3.
//

#ifndef ZST_DME_TOPOOPERATION_H
#define ZST_DME_TOPOOPERATION_H
#include "string"
#include "vector"
#include "util.h"
#include "FileOperation.h"
//拓扑文件的生成
class TopoOperation {
public:
        void topoGenerate(std::vector<std::shared_ptr<Triple>> &path, std::string topo_path, std::string bench_path);
        void write(const std::string file_path,std::vector<std::shared_ptr<Triple>> &path);
        void read(const std::string file_path, std::vector<std::shared_ptr<Triple>> &path);
private:
        double calc_dist(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right);
        double get_nearest(std::vector<std::shared_ptr<TreeNode>>& recur_set , std::vector<std::shared_ptr<TreeNode>>& result,std::vector<int>& iters);
        std::shared_ptr<TreeNode> merge_point(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right);
        bool readCoordinates(const std::string& line, double& x, double& y);
        bool readCapacitance(const std::string& line, double& cap);
        int readNodeId(const std::string& line);
        void readNode(std::ifstream& file,
                      std::unordered_map<int , std::shared_ptr<TreeNode>>& node_map,
                      std::shared_ptr<TreeNode>&,
                      std::string line);
};


#endif //ZST_DME_TOPOOPERATION_H
