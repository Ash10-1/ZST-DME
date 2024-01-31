//
// Created by zhh on 2024/1/3.
//

#include <fstream>
#include <sstream>
#include <iomanip>

#include "TopoOperation.h"

// Function to read coordinates from a line
bool TopoOperation ::readCoordinates(const std::string& line, double& x, double& y) {
    if (line.find("Coordinate :") != std::string::npos) {
        std::istringstream iss(line);
        std::string temp;
        iss >> temp >> temp >> x >> y;
        return true;
    }
    return false;
}

// Function to read capacitance from a line
bool TopoOperation :: readCapacitance(const std::string& line, double& cap) {
    if (line.find("Capacitive Load :") != std::string::npos) {
        std::istringstream iss(line);
        std::string temp;
        iss >> temp >> temp >> temp >> cap;
        return true;
    }
    return false;
}
// 从文件中读取node_id
int TopoOperation :: readNodeId(const std::string& line) {
    int node_id;
    if (line.find("node_id :") != std::string::npos) {
        std::istringstream iss(line);
        std::string temp;
        iss >> temp >> temp >> node_id;
        return node_id;
    }
    return -1;
}
void TopoOperation::readNode(std::ifstream &file,
                             std::unordered_map<int, std::shared_ptr<TreeNode>> &node_map,
                             std::shared_ptr<TreeNode>& node,
                             std::string line) {
    std::getline(file, line);
    int node_id = readNodeId(line);
    if (node_id != -1) {
        if (node_map.find(node_id) != node_map.end()) {
            node = node_map[node_id];
            // Skip the next two lines
            std::getline(file, line);
            std::getline(file, line);
        }
        else {
            node->node_id = node_id;
            std::getline(file, line);
            double x, y;
            readCoordinates(line, x, y);
            node->p = {x, y};
            std::getline(file, line);
            double cap;
            readCapacitance(line, cap);
            node->capacitance = cap;
            node_map[node_id] = node;
        }
    }
    else {
        throw std::runtime_error("Unable to read node id");
    }

}
void TopoOperation::read(const std::string file_path,
                         std::vector<std::shared_ptr<Triple>> &path) {
    std::unordered_map<int , std::shared_ptr<TreeNode>> node_map;
    // 创建一个ifstream对象用于读取文件
    std::ifstream file(file_path);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::string line;
    int size = 0;
    //读取path的数量
    while (std::getline(file, line)) {
        if (line.find("size :") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> size;
            break;
        }
    }
    path.reserve(size);//为path分配足够多的空间

    for (int i = 0; i < size; ++i) {
        auto triple = std::make_shared<Triple>();
        auto left_node = std::make_shared<TreeNode>();
        auto right_node = std::make_shared<TreeNode>();
        auto merge_node = std::make_shared<TreeNode>();
        // 读取左右节点
        readNode(file, node_map, left_node, line);
        readNode(file, node_map, right_node, line);
        readNode(file, node_map, merge_node, line);
        // Set triple
        triple->leftnode = left_node;
        triple->rightnode = right_node;
        triple->mergenode = merge_node;
        // Set child and parent relationships
        merge_node->left_child = left_node;
        merge_node->right_child = right_node;
        left_node->father = merge_node;
        right_node->father = merge_node;
        path.push_back(triple);
    }
    file.close();

}
void TopoOperation::write(const std::string file_path, std::vector<std::shared_ptr<Triple>> &path) {
    // 创建一个ofstream对象用于写入文件
    std::ofstream outfile(file_path);

    // 检查文件是否成功打开
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }
    outfile << std::fixed << std::setprecision(18);  // 设置精度为10位小数
    int size = path.size();
    outfile << "size : " << size << std::endl;
    for(const auto& triple : path){
        std::shared_ptr<TreeNode> n1 = triple->leftnode;
        outfile << "node_id : " << n1->node_id << std::endl;
        outfile << "Coordinate : " << n1->p.x << " " << n1->p.y << std::endl;
        outfile << "Capacitive Load : " << n1->capacitance << std::endl;

        n1 = triple->rightnode;
        outfile << "node_id : " << n1->node_id << std::endl;
        outfile << "Coordinate : " << n1->p.x << " " << n1->p.y << std::endl;
        outfile << "Capacitive Load : " << n1->capacitance << std::endl;

        n1 = triple->mergenode;
        outfile << "node_id : " << n1->node_id << std::endl;
        outfile << "Coordinate : " << n1->p.x << " " << n1->p.y << std::endl;
        outfile << "Capacitive Load : " << n1->capacitance << std::endl;

    }
    // 写入文件


    // 关闭文件流
    outfile.close();
}