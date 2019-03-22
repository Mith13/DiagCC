#ifndef DIAGRAM_H
#define DIAGRAM_H
#include <vector>
#include "operator.h"
#include "contraction_pattern.h"
#include <iostream>
#include "stdafx.h"
#include <algorithm>


class Diagram
{
public:
    Diagram (ContractionPattern& internals, std::vector<std::vector<Operator>>& Op_pool_ordered);
    Diagram(const Diagram& diag);
    void enumerate();
    void toLatex();
    ~Diagram();
private:
    unsigned int t_last;
    std::vector<std::vector<Operator>> operators; 
    std::vector<char> h_name;
    std::vector<char> p_name;
    ContractionPattern internal_lines;
    std::vector<int> sub_internal(std::vector<int>& pool, unsigned int Op_pos);
};

#endif // DIAGRAM_H
