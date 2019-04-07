#pragma once
#include "PrimitiveGraph.h"

class Operator : public Vertex
{
public:
	Operator() :Vertex() {};
	Operator(int top, int bot, int tpos) :Vertex(top,bot,tpos) {
		erank = 0;
	}

	void setup(int erank, int t_pos, char c);
private:
	int erank;
	char name;
};
class OperatorPool : public VertexPool
{
    
};

//base diagram class
class Diagram
{
public:
	Diagram(const std::shared_ptr<VertexPool>& pool, const std::vector<Edge>& edges);
	Diagram(const Diagram& diagram);
	const size_t countEdges() const;
	Edge* getEdge(int pos){return &edges[pos];};
	std::vector<Edge> getEdges() { return edges; };
	void printDiagram()const;
	void printSimple()const;
	bool isEqual(const Diagram& rhs)const;
	bool isEqual(const std::unique_ptr<Diagram>& rhs)const;
	virtual bool isEqual(const std::vector<Edge>& tmp) const;
	std::vector<std::unique_ptr<Diagram>> createHugenholtzDiagrams();    
	~Diagram();
protected:
	const std::shared_ptr<VertexPool> vertices;
	std::vector<Edge> edges;
private:
    std::vector<std::vector<int>> adjacency_matrix;
    void generatePermutations(std::vector<std::vector<int>>& time_ordered_vertices, std::vector<int>& permutation, size_t idx,std::vector<std::vector<int>>& permutation_set)const ;
	bool isIsomorphic(const Diagram& rhs)const;
	void equipartition();
    bool isEqualVertexPermutation(const std::vector<Edge>& tmp)const;
	void enumerateEdges(std::vector<Edge>& edges, std::vector<Edge>& tmp,std::vector<std::unique_ptr<Diagram>> &diagram_vec, size_t numOfHoles, size_t numOfPart);
};

//Diagram container class
template<typename T>
class DiagramContainer
{
public:
	DiagramContainer() { data.reserve(5); };
	DiagramContainer(const DiagramContainer & rhs) :data(rhs.data) {};
	DiagramContainer& operator=(const DiagramContainer& rhs) 
	{
		this->data = data;
		return *this;
	};

	void insert(T&& in);	
	void remove(int i);

	~DiagramContainer() { data.clear(); };
private:
	std::vector<T> data;

};

class HugenholtzDiagram : public Diagram
{
public:
	HugenholtzDiagram(const std::shared_ptr<VertexPool>& vertices,const std::vector<Edge>& edges);
	HugenholtzDiagram(const HugenholtzDiagram & rhs);
	HugenholtzDiagram(const Diagram& HugenholtzDiagram);
	void getAlgebraicForm();
	bool isEqual(const std::vector<Edge>& tmp) const override ;

private:
	
};



//Tree classes
class Node
{
public:
	Node();
	Node(Node* parent);
	Node(Node* parent, Edge edge_in);
	void branch(Node* next);
	std::string print();
	Edge getEdge();
	~Node();
	std::vector<Node *> child;
private:
	Node* parent;
	Edge data;
};
class Tree
{
public:
	Tree(std::shared_ptr<VertexPool>& pool);
	Tree(Node* root, std::shared_ptr<VertexPool>& pool);
	std::vector< std::unique_ptr<Diagram>>& getDiagrams(){return diagrams;};
	void printTree(Node *root, bool last, std::string indent);
	~Tree();
private:
	Node* root;
	std::shared_ptr<VertexPool> pool;
	std::vector<std::unique_ptr<Diagram>> diagrams;
	std::vector<Edge> edge_data; 
	void makeTree(Node* parent, int d);
	void reduceTree(Node* parent, std::vector<Edge>& edge_in, size_t  edge_count);
};

template<typename T>
inline void DiagramContainer<T>::insert(T&& in)
{
	data.push_back(std::forward<T>(in));
}

template<typename T>
inline void DiagramContainer<T>::remove(int i)
{
	data.erase(i);
}
