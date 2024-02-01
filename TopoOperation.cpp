//
// Created by zhh on 2024/1/3.
//

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "TopoOperation.h"
/**
*计算两点间曼哈顿距离
*@param left 左节点指针
*@param right 右节点指针
*@return 两节点间距离
*/
double TopoOperation::calc_dist(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right){
    double xl = left -> p.x;
    double yl = left -> p.y;
    double xr = right -> p.x;
    double yr = right -> p.y;
    return (double)fabs(xl-xr)+fabs(yl-yr);
}
/**
*计算一组node中最小间距，recurset是递归数组,n是数组长度。我们将结果存进result，输出最小距离
* @param recur_set 一组node
* @param n recur_set的长度
* @param result 这一组中间距最小的两个点
 * 需要指明的是，我们在函数外部就定义了result数组，而且result数组是存储指针(地址)的，
 * 因此当我们计算出最小间距点对并将其保存在result中时
 * 即便跳出这个函数，那个函数外部的result数组仍然保存着最小间距点对
* @return 最小的两点间距离
*/
double TopoOperation::get_nearest(std::vector<std::shared_ptr<TreeNode>>& recur_set , std::vector<std::shared_ptr<TreeNode>>& result,std::vector<int>& iters){
    int n = recur_set.size();
    if (n < 2) return 0; // 如果节点数少于2，直接返回

    // 初始化最近节点对和最小距离
    result[0] = recur_set[0];
    result[1] = recur_set[1];
    iters[0] = 0;
    iters[1] = 1;
    double min_dist = calc_dist(result[0], result[1]);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = calc_dist(recur_set[i], recur_set[j]);
            if (dist < min_dist) {
                min_dist = dist;
                result[0] = recur_set[i];
                result[1] = recur_set[j];
                iters[0]=i;
                iters[1]=j;
            }
        }
    }
    return min_dist;
}


/**
*计算中点也就是 merging point
* @param left 左节点的指针
* @param right 右节点的指针
* @return 动态内存中开辟的mergepoint的指针
*/
std::shared_ptr<TreeNode> TopoOperation::merge_point(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right) {
    Point p((left->p.x + right->p.x) / 2, (left->p.y + right->p.y) / 2);
    TreeNode node(p);
    return std::make_shared<TreeNode>(node);
}
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
void TopoOperation::topoGenerate(std::vector<std::shared_ptr<Triple>> &path,
                                 std::string topo_path,
                                 std::string bench_path) {
    FileOperation fileOperation;
    std::vector<double> caps;
    std::vector<Point> sink_set = fileOperation.read(bench_path,caps);
    printf("The total number of pins: %d\n", numpins);
    for(int i = 0; i< numpins; i++){
        std::cout << "(" <<  sink_set[i].x << ",";
        std::cout << sink_set[i].y << ")  ";
        std::cout << "Capacitive Load :" << caps[i] << "\n";
    }// 打印点集
    std::vector<std::shared_ptr<TreeNode>> recur_array;
    auto caps_iter = caps.begin();
    int id = 0;
    for (const auto &sink: sink_set){
        //初始化每个sink node节点
        auto ptr = std::make_shared<TreeNode>();
        ptr->p = Point (sink.x,sink.y);
        ptr->capacitance = *caps_iter;
        ptr->node_id = id++;
        //防止内存溢出
        assert(caps_iter != caps.end());
        ++caps_iter;
        recur_array.push_back(ptr);
    }
    //生成时钟树的拓扑结构
    while(recur_array.size() > 1){
        std::vector<std::shared_ptr<TreeNode>> result(2); // 用于存储最小间距点对
        std::vector<int> iters = {-1, -1};
        get_nearest(recur_array, result, iters); // 最小间距点对的间距
        // 然后在使用时：
        std::shared_ptr<TreeNode> mergeNode = merge_point(result[0], result[1]);

        mergeNode->type= true;
        mergeNode->delay=0;
        mergeNode->radius=-1;
        mergeNode->capacitance = 0;
        mergeNode->left_child=result[0];
        mergeNode->right_child=result[1];
        mergeNode->left_child->father = mergeNode;
        mergeNode->right_child->father = mergeNode;
        mergeNode->node_id = id++;
        //建立Triple并填入路径数组
        std::shared_ptr<Triple> triple = std::make_shared<Triple>();
        triple -> leftnode = result[0];
        triple -> rightnode = result[1];
        triple ->mergenode = mergeNode;

        // 从递归数组中删除最小间距点对 result[0] 和 result[1]
        if ((iters[0] != -1)&&((iters[1] != -1))){
            //先删除索引比较大的元素
            int first = std::max(iters[0], iters[1]);
            int sec = std::min(iters[0], iters[1]);
            recur_array.erase(recur_array.begin()+first);
            recur_array.erase(recur_array.begin()+sec);
        }

        recur_array.push_back(mergeNode); // 新增merging_point
        path.push_back(triple);
    }
    this->write(topo_path, path);

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