//
// Created by zhh on 2023/12/27.
//

#include <iostream>
#include "ElmoreDelay.h"
#include "cmath"
/**
*计算以该节点为根的子树的电容
*@param node 左节点指针
*@return 节点的电容
*/
double ElmoreDelay::calNodeCap(std::shared_ptr<TreeNode> node) {
    double node_cap = node->capacitance;
    //如果改节点为一个sink node，直接返回node的电容
    if (!node->type && (node->capacitance != 0) ){
        return node_cap;
    }
    double left_cap = 0;
    double right_cap = 0;
    if (node->left_child != nullptr){
        //计算与子节点之间的曼哈顿距离
        double dis = manhattanDistance(node->p, node->left_child->p);
        //计算导线的电容
        double edge_cap = per_unit_capacitance * dis;
        //计算当前节点的电容
        left_cap = edge_cap  + node->left_child->capacitance;
    }
    if (node->right_child != nullptr){
        //计算与子节点之间的曼哈顿距离
        double dis = manhattanDistance(node->p, node->right_child->p);
        //计算导线的电容
        double edge_cap = per_unit_capacitance * dis;
        //计算当前节点的电容
        right_cap = edge_cap  + node->right_child->capacitance;
    }
    node->capacitance = left_cap + right_cap + node_cap;

    return node->capacitance;
}
/**
*计算以该节点为根的子树的时延
*@param node 左节点指针
*@return 节点的时延
*/
double ElmoreDelay::calNodeDelay(std::shared_ptr<TreeNode> node) {
    if (!node->type){
        return 0;
    }
    double delay = 0;
    if (node->left_child != nullptr){
        delay = node->left_child->delay;
        double dis = manhattanDistance(node->p, node->left_child->p);
        double edge_resistance = per_unit_resistance * dis;
        double edge_cap = per_unit_capacitance * dis;
        //由于这个式子，因此需要先计算节点的cap，才能对节点的delay进行计算
        delay += edge_resistance * (0.5*edge_cap+node->capacitance);
        node->delay = delay;
        return delay;
    }
    return -1;

}
/**
*计算两个即将合并节点的Trr半径，且合并代价小于k。
*@param node_a 左节点指针
*@param node_b 右节点指针
*@return 节点a的Trr半径
*/
double ElmoreDelay::calTrrRadius(std::shared_ptr<TreeNode> node_a, std::shared_ptr<TreeNode> node_b, double dis) {

    //计算两个节点的电容,应当先计算电容，在计算时延
    double c1 = calNodeCap(node_a);
    double c2 = calNodeCap(node_b);

    //计算两个节点的时延
    double t1 = calNodeDelay(node_a);
    double t2 = calNodeDelay(node_b);
    node_a->delay = t1;
    node_b->delay = t2;
//    std::cout << per_unit_resistance << "   ";
//    std::cout << per_unit_capacitance;
    double trra = (t2 - t1) + per_unit_resistance * dis * (c2 + 0.5 * per_unit_capacitance * dis);
    trra /= (per_unit_resistance * (c1 + c2 + per_unit_capacitance * dis));
    return trra;
}
/**
*计算合并代价大于k时，两个合并节点的Trr半径（下列情况是节点a的半径为0的情况）
*@param node_a 左节点指针
*@param node_b 右节点指针
*@return 节点a的Trr半径
*/
double ElmoreDelay::calTrrRadiusBeyondKa(std::shared_ptr<TreeNode> node_a, std::shared_ptr<TreeNode> node_b) {
    //节点b的时延太大的情况
    double c2 = node_b->capacitance;
    double t1 = node_a->delay;
    double t2 = node_b->delay;
//    assert(t1 > t2);
    double trrb = sqrt(pow(per_unit_resistance*c2,2) + 2 * per_unit_capacitance *
                                                       per_unit_resistance * (t1 - t2)) - per_unit_resistance * c2;
    trrb = trrb / per_unit_resistance / per_unit_capacitance;
    return trrb;
}
/**
*计算合并代价大于k时，两个合并节点的Trr半径（下列情况是节点b的半径为0的情况）
*@param node_a 左节点指针
*@param node_b 右节点指针
*@return 节点a的Trr半径
*/
double ElmoreDelay::calTrrRadiusBeyondKb(std::shared_ptr<TreeNode> node_a, std::shared_ptr<TreeNode> node_b) {
    //节点a的时延太大的情况
    double c2 = node_a->capacitance;
    double t1 = node_a->delay;
    double t2 = node_b->delay;
//    assert(t1 > t2);
    double trra = sqrt(pow(per_unit_resistance*c2,2) + 2 * per_unit_capacitance *
                                                       per_unit_resistance * (t2 - t1)) - per_unit_resistance * c2;
    trra = trra / per_unit_resistance / per_unit_capacitance;

    return trra;
}