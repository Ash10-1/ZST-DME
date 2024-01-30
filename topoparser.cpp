//
// Created by 13504 on 2023/11/22.
//
#include "topoparser.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "util.h" //注意文件位置
#include "trr.h"
#include "toptodown.h"
#include "TopoOperation.h"
#include "Test.h"

/**
*计算两点间曼哈顿距离
*@param left 左节点指针
*@param right 右节点指针
*@return 两节点间距离
*/
double calc_dist(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right){
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
double get_nearest(std::vector<std::shared_ptr<TreeNode>>& recur_set , std::vector<std::shared_ptr<TreeNode>>& result,std::vector<int>& iters){
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
std::shared_ptr<TreeNode> merge_point(std::shared_ptr<TreeNode> left, std::shared_ptr<TreeNode> right) {
    Point p((left->p.x + right->p.x) / 2, (left->p.y + right->p.y) / 2);
    TreeNode node(p);
    return std::make_shared<TreeNode>(node);
}

/**
* 生成所有 merging point，并且存储到路径数组里面
*@param sink_set double格式二维数组指针，代表从benchmark文件中读取的坐标点构成的数组
*@param n sink_set数组的行数，也就是点的个数
*@param path 路径数组
*@return 完成二叉树构建之后二叉树的根的指针 root
**/
std::shared_ptr<TreeNode> tree_build(std::vector<Point> &sink_set, std::vector<double> caps, const std::string topo_path){
    std::vector<std::shared_ptr<TreeNode>> recur_array;
    std::vector<std::shared_ptr<Triple>> path;
    auto caps_iter = caps.begin();

    for (const auto &sink: sink_set){
        //初始化每个sink node节点
        auto ptr = std::make_shared<TreeNode>();
        ptr->p = Point (sink.x,sink.y);
        ptr->capacitance = *caps_iter;
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
    std::vector<std::shared_ptr<Triple>> path1;
    TopoOperation op;
    op.write(topo_path, path);
    op.read(topo_path, path1);
    Test test;
    test.testFile(path, path1);
    for (int i = 0; i < path.size(); ++i){
        //计算合并线段
        std::shared_ptr<Triple> pair = path[i];
        cal_merge_segment(pair);
//        assert((path[i]->mergenode->p.y) > path[i]->mergenode->ms_end.y);
        assert(path[i]->mergenode->ms_end.y != 0);
        assert(path[i]->mergenode->ms_end.y > 0);
        printf("path[%d] radiusa = %lf   ", i,path[i]->leftnode->radius);
        printf("radiusb = %lf  ", path[i]->rightnode->radius);
        std::cout  << "merging segment: " <<  "("<< pair->mergenode->p.x << ","<< pair->mergenode->p.y << ")";
        std::cout <<  "(" << pair->mergenode->ms_end.x << ","<< pair->mergenode->ms_end.y << ")" << "\n";
    }
    //从上到下计算精确点
    for(int i = path.size()-1; i >=0; i--){

        Point par = path[i]->mergenode->p;
        std::shared_ptr<TreeNode> left = path[i]->leftnode;
        std::shared_ptr<TreeNode> right  =path[i]->rightnode;
        Point v1 = find_exact_point(*left,par);
        std::cout << "path[" << i << "]  ";
        v1.print();
        std::cout << "  ";
        left->p = v1;
        Point v2 = find_exact_point(*right,par);
        v2.print();
        std::cout << "\n";
        right->p = v2;
    }


    Triple * triple1 = path[(path.size()-1)].get();
    return triple1->mergenode;
}

