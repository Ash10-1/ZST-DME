//
// Created by zhh on 2024/1/30.
//

#include <fstream>
#include <sstream>
#include "FileOperation.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
// Use the nlohmann json namespace
using json = nlohmann::json;
int numpins;
double per_unit_resistance;
double per_unit_capacitance;

// 使用 std::vector<Point> 来替代二维数组
std::vector<Point> FileOperation:: read(const std::string& filename, std::vector<double>& caps) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::vector<Point> sink_set;
    std::string line;
    int flag = 0;
    // 读取 NumPins
    while (std::getline(file, line)) {
        if (line.find("NumPins :") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> numpins;
            ++flag;
        }
        if (line.find("PerUnitResistance") != std::string::npos){
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> per_unit_resistance;
            ++flag;
        }
        if (line.find("PerUnitCapacitance") != std::string::npos){
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> per_unit_capacitance;
            ++flag;
        }
        if (flag >= 3){
            break;
        }
    }

    sink_set.reserve(numpins); // 为 sink_set 分配足够的内存

    // 读取坐标
    while (std::getline(file, line)) {
        if (line.find("Coordinate :") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            double x, y;
            iss >> temp >> temp >> x >> y;
            sink_set.push_back({x, y});
        }
        if (line.find("Capacitive Load :") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            double cap;
            iss >> temp >> temp >> temp >> cap;
            caps.push_back(cap);
        }
    }

    return sink_set;
}
/**
 *  生成网表文件
 *  步骤：
 *  1. 因为在网表文件中，根节点以及其左右子节点的信息必须在开头，因此首先进行处理
 *  2. 之后进行递归，递归的详细介绍见递归函数 (_op)
 * @param filename 网表的输出文件名
 * @param root  整颗二叉树的根节点
 */

void FileOperation::generate_netlist(const char* filename, std::shared_ptr<TreeNode> root){
    FILE* file = fopen(filename, "w");
    fprintf(file, "%lf, %lf\n", root->left_child->p.x, root->left_child->p.y);
    fprintf(file, "%lf, %lf\n", root->p.x, root->p.y);
    fprintf(file, "%lf, %lf\n", root->right_child->p.x, root->right_child->p.y);
    generate_netlist_op(file, root);
    fclose(file);
}
/**
 * 1. 首先对左子节点进行递归，按照顺序输出“本节点-父节点-兄弟节点”
 * 2. 之后对右子节点进行递归，最终输出除了根节点之外所有节点的 Triple (Triple 表示某节点及其左右子节点)
 =* 注意：
 * 1. node->father == NULL 表示node是根节点，在generate_netlist函数中已经对根节点的 Triple 进行处理了，因此这里不再处理
 * 2. node->father->father == NULL 表示 node 是从上往下第三层的节点
 *    因为输出的是“本节点-父节点-兄弟节点”而不是“本节点-左子节点-右子节点”
 *    所以 if(node->father->father != NULL) 分支实际上是处理了从上往下第二层节点的 Triple
 * @param file 文件指针
 * @param node 本节点
 */
void FileOperation::generate_netlist_op(FILE* file, std::shared_ptr<TreeNode> node){
    if (node == nullptr || file == nullptr) {
        return;
    }

    if(node->left_child != nullptr){
        generate_netlist_op(file, node->left_child);
    }
    if(node->right_child != nullptr){
        generate_netlist_op(file, node->right_child);
    }

    if(node->father == nullptr){
        return;
    }
    std::shared_ptr<TreeNode> brother = get_brother(node);
    if (brother != nullptr) {
        fprintf(file, "%f, %f\n", node->p.x, node->p.y);
        fprintf(file, "%f, %f\n", node->father->p.x, node->father->p.y);
        fprintf(file, "%f, %f\n", brother->p.x, brother->p.y);
    }
}
