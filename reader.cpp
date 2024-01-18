#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "util.h"
#include "reader.h"

int numpins;
double per_unit_resistance;
double per_unit_capacitance;

// 使用 std::vector<Point> 来替代二维数组
std::vector<Point> read(const std::string& filename, std::vector<double>& caps) {
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