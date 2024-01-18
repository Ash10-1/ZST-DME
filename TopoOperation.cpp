//
// Created by zhh on 2024/1/3.
//

#include <fstream>
#include <sstream>
#include <iomanip>
#include "TopoOperation.h"
// Function to read coordinates from a line
bool readCoordinates(const std::string& line, double& x, double& y) {
    if (line.find("Coordinate :") != std::string::npos) {
        std::istringstream iss(line);
        std::string temp;
        iss >> temp >> temp >> x >> y;
        return true;
    }
    return false;
}

// Function to read capacitance from a line
bool readCapacitance(const std::string& line, double& cap) {
    if (line.find("Capacitive Load :") != std::string::npos) {
        std::istringstream iss(line);
        std::string temp;
        iss >> temp >> temp >> temp >> cap;
        return true;
    }
    return false;
}
void TopoOperation::read(const std::string file_path, std::vector<std::shared_ptr<Triple>> &path) {
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

    while (std::getline(file, line)) {
        auto triple = std::make_shared<Triple>();
        auto left_node = std::make_shared<TreeNode>();
        auto right_node = std::make_shared<TreeNode>();
        auto merge_node = std::make_shared<TreeNode>();

        triple->leftnode = left_node;
        triple->rightnode = right_node;
        triple->mergenode = merge_node;

        double x, y, cap;

        // Left node
        readCoordinates(line, x, y);
        std::getline(file, line);
        readCapacitance(line, cap);
        left_node->p = {x, y};
        left_node->capacitance = cap;

        // Right node
        std::getline(file, line);
        readCoordinates(line, x, y);
        std::getline(file, line);
        readCapacitance(line, cap);
        right_node->p = {x, y};
        right_node->capacitance = cap;

        // Merge node
        std::getline(file, line);
        readCoordinates(line, x, y);
        std::getline(file, line);
        readCapacitance(line, cap);
        merge_node->p = {x, y};
        merge_node->ms_end = {0, 0};
        merge_node->type = true;
        merge_node->delay = 0;
        merge_node->radius = -1;
        merge_node->capacitance = 0; // No need to read from file as it's set to 0

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
        outfile << "Coordinate : " << n1->p.x << " " << n1->p.y << std::endl;
        outfile << "Capacitive Load : " << n1->capacitance << std::endl;
        n1 = triple->rightnode;
        outfile << "Coordinate : " << n1->p.x << " " << n1->p.y << std::endl;
        outfile << "Capacitive Load : " << n1->capacitance << std::endl;
        n1 = triple->mergenode;
        outfile << "Coordinate : " << n1->p.x << " " << n1->p.y << std::endl;
        outfile << "Capacitive Load : " << n1->capacitance << std::endl;
    }
    // 写入文件


    // 关闭文件流
    outfile.close();
}