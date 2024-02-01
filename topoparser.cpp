//
// Created by 13504 on 2023/11/22.
//
#include "topoparser.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include "util.h" //注意文件位置
#include "trr.h"
#include "toptodown.h"
#include "TopoOperation.h"
#include "FileOperation.h"
 /**
  *
  * @param bench_path bench的路径
  * @param topo_path  拓扑路径
  * @return
  */
std::shared_ptr<TreeNode> tree_build(std::string bench_path, const std::string topo_path){

    std::vector<std::shared_ptr<Triple>> path;

    TopoOperation op;
//    op.topoGenerate(path, topo_path, bench_path);
    op.read(topo_path, path);
    for (int i = 0; i < path.size(); ++i){
        //计算合并线段
        std::shared_ptr<Triple> pair = path[i];
        cal_merge_segment(pair);
        assert((path[i]->mergenode->p.y) > path[i]->mergenode->ms_end.y);
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

