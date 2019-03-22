/*#include "diagram.h"	
Diagram::Diagram(const Diagram& diag):operators(4),h_name{'a','b','c','d','e','f','g','h'},p_name{'i','j','k','l','m','n','o','p'}
{
    internal_lines=diag.internal_lines;
    operators=diag.operators;
}
Diagram::Diagram(ContractionPattern& internals, std::vector<std::vector<Operator>>& Op_pool_ordered):operators(4),h_name{'a','b','c','d','e','f','g','h'},p_name{'i','j','k','l','m','n','o','p'}
{
    internal_lines=internals;
    operators=Op_pool_ordered;
    t_last=operators.size();
    for(unsigned int i=0;i<t_last;i++){
        if(operators[i].size()==internal_lines.count_operators()){
            for(unsigned int j=0;j<operators[i].size();j++){
            //    operators[i][j].set_internal_lines(internals.get_operator(j));
            }
        }
    }
}
void Diagram::enumerate()
{
    
    for(unsigned int i=0;i<t_last;i++){
        for(unsigned int j=0;j<operators[i].size();j++){
            if(operators[i][j].get_name()=='V'){
                std::cout<<operators[i][j].get_name()<<"<";
               // for(x:)
            }
        }
    }
}
std::vector<int> Diagram::sub_internal(std::vector<int>& pool, unsigned int Op_pos)
{
    std::vector<int> pool_tmp(pool);
    std::vector<int> internal_Op(internal_lines.get_operator(Op_pos));
    std::sort(pool_tmp.begin(),pool_tmp.end());
    std::sort(internal_Op.begin(),internal_Op.end());
    std::vector<int> result;
    if(pool_tmp.size()>=internal_Op.size()){
        for(unsigned int i=0;i<pool_tmp.size();i++){
            if(i<internal_Op.size() && internal_Op[i]!=pool_tmp[i]){
                result.push_back(pool_tmp[i]);
            } else if(i>=internal_Op.size()) {
                result.push_back(pool_tmp[i]);
            }
        }
    }
    return result;
}
Diagram::~Diagram()
{    
}


*/