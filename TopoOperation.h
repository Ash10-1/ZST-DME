//
// Created by zhh on 2024/1/3.
//

#ifndef ZST_DME_TOPOOPERATION_H
#define ZST_DME_TOPOOPERATION_H
#include "string"
#include "vector"
#include "util.h"
//拓扑文件的生成
class TopoOperation {
public:
        void write(const std::string file_path,std::vector<std::shared_ptr<Triple>> &path);
        void read(const std::string file_path, std::vector<std::shared_ptr<Triple>> &path);
private:
        bool readCoordinates(const std::string& line, double& x, double& y);
        bool readCapacitance(const std::string& line, double& cap);
        int readNodeId(const std::string& line);
        void readNode(std::ifstream& file,
                      std::unordered_map<int , std::shared_ptr<TreeNode>>& node_map,
                      std::shared_ptr<TreeNode>&,
                      std::string line);
};


#endif //ZST_DME_TOPOOPERATION_H
