#include <iostream>
#include "util.h"
#include "FileOperation.h"
#include "topoparser.h"
#include "vector"

const char* benchmark_file_path = "F:\\C++Project\\ZST_DME-master\\benchmark\\r1.txt"; //todo benchmark.txt文件的绝对路径
const char* netlist_file_path = "F:\\C++Project\\ZST_DME-master\\embedding\\netlist.txt"; //todo netlist.txt文件的绝对路径
const std::string topo_path = "F:\\C++Project\\ZST_DME-master\\topo\\r1.txt";

//const char* netlist_file_path = "F:\\C++Project\\ZST_DME-master\\embedding\\netlist.txt"; //todo netlist.txt文件的绝对路径

int main() {

  FileOperation fileOperation;

     /**
     * 生成二叉树结构
     */
  std::shared_ptr<TreeNode> tree = tree_build(benchmark_file_path, topo_path); //时钟树(二叉树)

    /**
     * 生成网表文件
     */
  fileOperation.generate_netlist(netlist_file_path, tree);



    return 0;
}