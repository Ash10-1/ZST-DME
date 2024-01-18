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
};


#endif //ZST_DME_TOPOOPERATION_H
