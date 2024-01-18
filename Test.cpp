//
// Created by zhh on 2024/1/7.
//

#include "Test.h"
using namespace std;
void Test::testFile(std::vector<std::shared_ptr<Triple>> p1,std::vector<std::shared_ptr<Triple>> p2) {
    for(int i=0; i < p1.size(); ++i){
        printf("path[%d] \n", i);
        shared_ptr<TreeNode> left1 = p1[i]->leftnode;
        shared_ptr<TreeNode> left2 = p2[i]->leftnode;
        assert(left1->p.x == left2->p.x);
        assert(left1->p.y == left2->p.y);
        assert(left1->capacitance == left2->capacitance);

        shared_ptr<TreeNode> r1 = p1[i]->rightnode;
        shared_ptr<TreeNode> r2 = p2[i]->rightnode;
        assert(r1->p.x == r2->p.x);
        assert(r1->p.y == r2->p.y);
        assert(r1->capacitance == r2->capacitance);

        shared_ptr<TreeNode> m1 = p1[i]->mergenode;
        shared_ptr<TreeNode> m2 = p2[i]->mergenode;
        assert(m1->p.x == m2->p.x);
        assert(m1->p.y == m2->p.y);
        assert(m1->capacitance == m2->capacitance);
    }

}