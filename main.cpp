#include <iostream>
#include "util.h"
#include "reader.h"
#include "topoparser.h"
#include "netlist.h"
#include "vector"

const char* benchmark_file_path = "F:\\C++Project\\ZST_DME-master\\benchmark\\r1.txt"; //todo benchmark.txt文件的绝对路径
const char* netlist_file_path = "F:\\C++Project\\ZST_DME-master\\embedding\\netlist.txt"; //todo netlist.txt文件的绝对路径
const std::string topo_path = "F:\\C++Project\\ZST_DME-master\\topo\\r1.txt";

//const char* netlist_file_path = "F:\\C++Project\\ZST_DME-master\\embedding\\netlist.txt"; //todo netlist.txt文件的绝对路径

int main() {

    /**
     * benchmark 文件读取解析，获取 2 * numpins 的二维数组，存储所有点的坐标
     */
    std::vector<double> caps;
    std::vector<Point> sink_set = read(benchmark_file_path,caps);
    printf("The total number of pins: %d\n", numpins);

    for(int i = 0; i< numpins; i++){
        std::cout << "(" <<  sink_set[i].x << ",";
        std::cout << sink_set[i].y << ")  ";
        std::cout << "Capacitive Load :" << caps[i] << "\n";
    }// 打印点集

     /**
     * 生成二叉树结构
     */
    std::shared_ptr<TreeNode> tree = tree_build(sink_set, caps, topo_path); //时钟树(二叉树)

    /**
     * 生成网表文件
     */
    generate_netlist(netlist_file_path, tree);



    return 0;
}