#pragma once
#include <string>
#include <vector>
#include <memory>
#include "PrimitiveGraph.h"

class algebraic_node
{
public:
	algebraic_node(std::string& term);
	~algebraic_node();
	void toLatex();
private:
	std::vector<std::string> m_term;
};

class abstract_syntax_tree
{
	abstract_syntax_tree();
};
