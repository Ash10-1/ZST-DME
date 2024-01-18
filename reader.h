//
// Created by 13504 on 2023/11/22.
//

#ifndef ZST_DME_READER_H
#define ZST_DME_READER_H
#include "vector"
extern int numpins;
extern double per_unit_resistance;
extern double per_unit_capacitance;
std::vector<Point> read(const std::string& filename, std::vector<double>& caps);
#endif //ZST_DME_READER_H
