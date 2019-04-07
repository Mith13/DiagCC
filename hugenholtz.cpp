#include "hugenholtz.h"
#include "auxiliary.h"

using VertexPool_sPtr = std::shared_ptr<VertexPool>;
using Hugenholtz_uPtr = std::unique_ptr<HugenholtzDiagram>;
using Diagram_uPtr = std::unique_ptr<Diagram>;
 
//class Node
Node::Node()
{
    parent = NULL;
}
 
Node::Node(Node * parent)
{
    this->parent = parent;
}
 
Node::Node(Node * parent, Edge edge_in)
{
    this->parent = parent;
    data = edge_in;
}
 
void Node::branch(Node * next)
{
    child.push_back(next);
}
std::string Node::print()
{
    std::string tmp = "(" + std::to_string(data.getFirst()+1) + "," + std::to_string(data.getSecond()+1) + ")";
    return tmp;
}
Edge Node::getEdge()
{
    return data;
}
 
Node::~Node()
{   
 
    std::cout << "Deleting Node" << std::endl;
    this->parent = NULL;
    if (child.size() == 0) return;
    for (Node* next : child) {
        next->~Node();
    }
}
//class Tree
Tree::Tree(VertexPool_sPtr& pool):pool(pool)
{
    //this->pool=pool;
    this->root = new Node();
    edge_data.reserve(10.0*pool->countVertices());
    makeTree(root,0);   
    printTree(root,false,"");
    pool->reset();
    int sum=pool->allConnected()/2;
    reduceTree(root, edge_data,sum);
    std::cout << "Printing diagram" << std::endl;
    for (auto& x : diagrams)
    {
        x->printSimple();
    }
}
 
Tree::Tree(Node * root, VertexPool_sPtr& pool):pool(pool)
{
    //this->pool=pool;
    this->root = root;
	edge_data.reserve(10*pool->countVertices());
    makeTree(root,0);
    printTree(root,false,"");
    reduceTree(root, edge_data,pool->allConnected()/2);
    std::cout << "Printing diagram" << std::endl;
    for (auto& x : diagrams) 
    {
       //x->printDiagram();
    }
}
 // Prepare tree for diagram generation
void Tree::makeTree(Node* parent, int d)
{
    int sum = pool->allConnected();
 
    if (sum == 0)
    {
        return;
    }
    for (int u = d + 1; u < pool->countVertices(); u++)
    {
        if (pool->getVertex(d).getUpLines() == 0 && d < pool->countVertices() - 2)
        {
            d++;
            u = d + 1;
        }
        if (pool->getVertex(u).getDownLines() > 0 && pool->getVertex(u).getTimePosition() != pool->getVertex(d).getTimePosition())
        {
            //if (pool[d].top_h == 0 && d<pool.size()-2) {
            //  d++;
            //  u=d+1;
            //}
            //Edge data(pool.work_tmp[d].getTimePosition(),pool.work_tmp[u].getTimePosition());
            Edge data(d,u);
            pool->getVertex(d).contractUp();
            pool->getVertex(u).contractDown();
            Node *child = new Node(parent, data);
            parent->branch(child);
            makeTree(child, d);
            pool->getVertex(d).decontractUp();
            pool->getVertex(u).decontractDown();
        }
    }
    return;
}
// Reduce diagram tree to non isomorphic diagrams
void Tree::reduceTree(Node* parent, std::vector<Edge>& edges, size_t  edge_count)
{
    if (parent->child.size() == 0)
    {
        //printf("End of branch\n");
        if (edges.size() == edge_count)
        {
			std::vector<Edge> edge_tmp(edges);
			std::sort(edge_tmp.begin(),edge_tmp.end());
            //std::unique_ptr <Diagram> tmp = std::make_unique<Diagram>(pool,stack);
            //tmp.Print();
            if (diagrams.size() > 0)
            {
                bool same = false;
                for (size_t i = 0; i < diagrams.size(); i++)
                {
                    if (diagrams[i]->isEqual(edge_tmp)) same = true;
                }
                if (!same)
                {
                    diagrams.push_back(std::make_unique<Diagram>(pool,edge_tmp));
                }
            }
            else
            {
                diagrams.push_back(std::make_unique<Diagram>(pool,edge_tmp));
            }
        }
        //stack.pop_back();
        return;
    }
    for (size_t i = 0; i < parent->child.size(); i++)
    {
        Node *next = parent->child[i];
        //std::cout << "next_node ";
        //next->GetData().Print();
        Edge tmp = next->getEdge();
        edges.push_back(tmp);
        reduceTree(next, edges, edge_count);
        edges.pop_back();
    }
    return;
}
 
void Tree::printTree(Node *parent, bool last, std::string indent)
{
    std::cout << indent + "+-" + parent->print() << std::endl;
    indent += last ? "  " : "| ";
    if (parent->child.size() == 0)
    {
        return;
    }
    for (int i = 0; i < static_cast<int>(parent->child.size()); i++)
    {
        Node *next = parent->child[i];
        printTree(next, i == (static_cast<int>(parent->child.size()) - 1), indent);
    }
}

 
Tree::~Tree(){
    std::cout << "Deleting Tree" << std::endl;
    root->~Node();
 
}
 
//class Diagram
Diagram::Diagram(const VertexPool_sPtr& pool,const std::vector<Edge>& edges):vertices(pool),adjacency_matrix(pool->countVertices(),std::vector<int>(pool->countVertices()))
{
    for (Edge e : edges)
    {
        this->edges.push_back(e);
        ++adjacency_matrix[e.getFirst()][e.getSecond()];
        ++adjacency_matrix[e.getSecond()][e.getFirst()];
    }
    //std::sort(this->edges.begin(),this->edges.end());
	std::cout << "Creating Diagram\n";
//  this->edges=edges;
}
Diagram::Diagram(const Diagram& diagram):vertices(diagram.vertices)
{
    this->edges=diagram.edges;
    this->adjacency_matrix=diagram.adjacency_matrix;
}
 
const size_t Diagram::countEdges()const
{
    return edges.size();
}
 
void Diagram::printDiagram() const
{
    int num_lines = 0;
 
    for (int i = 0; i < vertices->countVertices()-1;i++) 
    {
        for(const Edge& e : edges)
        {           
            if(e.getFirst()==i){
                num_lines++;
            }
        }
        std::cout<<"=";
        if(vertices->getVertex(i+1).getTimePosition()> vertices->getVertex(i).getTimePosition()){
            std::cout<<std::endl;
            for (int j=0;j<num_lines;j++){
                std::cout<<"|";
            }
            std::cout<<std::endl;
            num_lines = 0;
        }
    }
}
 
void Diagram::printSimple() const
{
    std::cout << "Printing diagram\n";
    for (const Edge& e : edges)
    {
        std::cout << "(" << e.getFirst() << "," << e.getSecond() << ")";
        if(e.getLineType()!=N_) std::cout<<" : "<<e.getLineType();
        std::cout<<std::endl;
    }
    //printDiagram();
    std::cout << std::endl;
}
 
bool Diagram::isEqual(const Diagram& rhs) const
{
        if (countEdges() != rhs.countEdges())
            return false;
        for (size_t i = 0; i < edges.size(); i++)
        {
            if (!(edges[i] == rhs.edges[i]))
            {
                return false;
            }
        }
        return true;
}
bool Diagram::isEqual(const Diagram_uPtr& rhs) const
{
    if (countEdges() != rhs->countEdges())
        return false;
    for (size_t i = 0; i < edges.size(); i++)
    {
        if (!(edges[i] == rhs->edges[i]))
        {
            return false;
        }
    }
    return true;
}
bool Diagram::isEqual(const std::vector<Edge>& tmp)const
{
	if (countEdges() != tmp.size())
		return false;
	for (size_t i = 0; i < edges.size(); i++)
	{
		if (!(edges[i] == tmp[i]))
		{
			//return false;
		}
	}
	return isEqualVertexPermutation(tmp);
}
bool Diagram::isIsomorphic(const Diagram& rhs)const 
{
	return true;
}
void Diagram::equipartition()
{

}
bool Diagram::isEqualVertexPermutation(const std::vector<Edge>& tmp) const
{
    
    std::vector<std::vector<int>> rhs_adjacency_matrix(vertices->countVertices(),std::vector<int>(vertices->countVertices(),0));
	for(auto& e:tmp)
    {
	    ++rhs_adjacency_matrix[e.getFirst()][e.getSecond()];
        ++rhs_adjacency_matrix[e.getSecond()][e.getFirst()];
    }
    std::vector<std::vector<int>> equivalent_vertices;
    for(int t=0;t<=vertices->t_last;t++)
    {            
		for (int down_lines = 0; down_lines <= vertices->down_max; down_lines++) {
			for (int up_lines = 0; up_lines <= vertices->up_max; up_lines++) {
				vertices->getTimedEquivalentVertices(equivalent_vertices,t, up_lines, down_lines);
			}
		}
    }
    
    std::vector<int> permutation_stack;
    std::vector<std::vector<int>> permutation_set;
    generatePermutations(equivalent_vertices,permutation_stack,0,permutation_set);
    
    for(auto& permutation:permutation_set)
    {
		if (permutation.size() < 2)continue;
		bool equal = true;
		for (int i = 0; i < vertices->countVertices(); ++i) {
			for (int j = 0; j <= i; ++j)
			{
				const int adjacency = adjacency_matrix[permutation[i]][permutation[j]];
				const int rhs_adjacency = rhs_adjacency_matrix[i][j];
				if (adjacency != rhs_adjacency)
					equal = false;
			}
		}
		if (equal)return true;
    }
    return false;
}
void Diagram::generatePermutations(std::vector<std::vector<int>>& time_ordered_vertices, std::vector<int>& permutation, size_t idx,std::vector<std::vector<int>>& permutation_set) const{
    if (idx == time_ordered_vertices.size())
	{
        permutation_set.push_back(permutation);
		return;
	}
	do{
		permutation.insert(permutation.end(),time_ordered_vertices[idx].begin(),time_ordered_vertices[idx].end());
		generatePermutations(time_ordered_vertices, permutation,idx+1,permutation_set);
		permutation.resize(permutation.size()-time_ordered_vertices[idx].size());
	} while (std::next_permutation(time_ordered_vertices[idx].begin(), time_ordered_vertices[idx].end()));
}
std::vector<Diagram_uPtr> Diagram::createHugenholtzDiagrams()
{
	std::vector<Edge> tmp_vec;
	std::vector<Diagram_uPtr> final_diagrams;
	enumerateEdges(edges,tmp_vec,final_diagrams,0,0); 
	std::cout<<"Enumerated diagrams"<<std::endl;
	return final_diagrams;
}
void Diagram::enumerateEdges(std::vector<Edge>& edges, std::vector<Edge>& tmp,std::vector<Diagram_uPtr> &diagram_vec, size_t numOfHoles, size_t numOfPart)
{
	size_t size = tmp.size();
	if(size==edges.size())
	{
		std::sort(tmp.begin(),tmp.end());
		bool correct_diagram=true;
		for (auto it = vertices->begin(); it < vertices->end();it++)
		{	
			int sum=0;		
			for (auto v: tmp)
			{
				if(v.getFirst()==(it-vertices->begin()))
				{
					int line=static_cast<int>(v.getLineType());
					sum+=line;
				}
				if(v.getSecond()==(it-vertices->begin()))
				{
					int line=static_cast<int>(v.getLineType());
					sum-=line;
				}
			}
			if(sum!=0)correct_diagram=false;	
		}
		if(correct_diagram){
			for(auto& diag_tmp : diagram_vec){
				if(diag_tmp->isEqual(tmp))correct_diagram=false;	
			}
		}
        //if(numOfHoles==num)
		//Diagram_uPtr tmp_diag =;
		if(correct_diagram)diagram_vec.push_back(std::make_unique<HugenholtzDiagram>(vertices, tmp));
		return;
	}    
    {
        Edge edge_tmp(edges[size]);
        edge_tmp.assignLineType(H_);
        tmp.push_back(edge_tmp);
        enumerateEdges(edges,tmp,diagram_vec,numOfHoles+1,numOfPart);
        tmp.pop_back();
    }    
    {
        Edge edge_tmp(edges[size]);
        edge_tmp.assignLineType(P_);
        tmp.push_back(edge_tmp);
        enumerateEdges(edges,tmp,diagram_vec,numOfHoles,numOfPart+1);
        tmp.pop_back();
    }
}
Diagram::~Diagram()
{
    this->edges.clear();
    std::cout << "Deleting Diagram"<<std::endl;
}
HugenholtzDiagram::HugenholtzDiagram(const VertexPool_sPtr& vertices, const std::vector<Edge>& edges) :Diagram(vertices, edges)
{
	
	std::sort(this->edges.begin(),this->edges.end());
	std::cout << "Creating Hugenholtz\n";
}

HugenholtzDiagram::HugenholtzDiagram(const HugenholtzDiagram & rhs) : Diagram(rhs.vertices, rhs.edges)
{
	std::cout << "Copying Hugenholtz\n";
}

HugenholtzDiagram::HugenholtzDiagram(const Diagram & rhs) : Diagram(rhs)
{
	std::cout << "Copying Diagram into Hugenholtz\n";
}
bool HugenholtzDiagram::isEqual(const std::vector<Edge>& tmp)const
{
	if (countEdges() != tmp.size())
		return false;
	for (size_t i = 0; i < edges.size(); i++)
	{
		if (!(edges[i] == tmp[i]))
		{
			return false;
		}
	}
	return true;
}
//class Operator
void Operator::setup(int erank, int t_pos, char c)
{
    this->t_pos = t_pos;
    this->name = c;
    switch (name)
    {
    case 'T':
        if (erank > 0) {
            std::cout << "Creating an excitation operator " << this->name << erank << std::endl;
            while (erank > 0) {
                up = erank * 2;
            }
        }
        else if (erank < 0) {
            name = 'L';
            std::cout << "Creating a deexcitation operator " << this->name << -erank << std::endl;
            while (erank < 0) {
                down = erank * 2;
            }
        }
        else {
            std::cout << "Wrong excitation rank for operator T or L" << std::endl;
        }
        break;
    case 'V':
        if (erank >= 0) {
            up = erank;
            down = 4 - erank;
        }
        else {
            aux::abortMessage("Wrong type of two electron operator");
        }
        break;
    case 'F':
        break;
    default:
        aux::abortMessage("Uknown operator type");
    }
}

