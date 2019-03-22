#pragma once
#include <vector>
#include "auxiliary.h"



enum lineType {N_ = 0, H_ = 1, P_ = -1};
class Vertex 
{
public:
	Vertex() :up(0), down(0),t_pos(-1),index(-1) {};
    Vertex(int top, int bot, int tpos):up(top),down(bot),t_pos(tpos),index(-1){};
	Vertex(const Vertex& rhs);
    
	virtual void setup(int up, int down, int t_pos);
    virtual void setIndex(int i);
    
	void contractUp();
	void decontractUp();
	void contractDown();
	void decontractDown();
    
	const int getTimePosition() const;
	const int getUpLines() const;
	const int getDownLines() const;
    const int getIndex() const;
	bool operator<(const Vertex & rhs) const;	
    
protected:
	int up, down;
	int t_pos;
    int index;
};
class VertexPool 
{
public:
	VertexPool();
	VertexPool(std::vector<Vertex>& pool):vertices(pool),vertices_wrk(pool){};
	VertexPool(const VertexPool& pool);
	VertexPool& operator= (const VertexPool& pool);        
	VertexPool& getPointer(){return *this;};
	~VertexPool();
    
	void addVertex(const Vertex& v);
    void reset();
    
	virtual Vertex& getVertex(int pos){return vertices_wrk[pos];};
	virtual void getTimedEquivalentVertices(std::vector<std::vector<int>>& equivalent_vertices, const int t_pos, const int up_lines, const int down_lines)const;
    
	const std::vector<Vertex>::iterator begin() { return vertices.begin(); }
	const std::vector<Vertex>::iterator end() { return vertices.end(); }
    
	const int countVertices()const {return vertices_wrk.size();};
	int allConnected();
    int t_last, up_max, down_max;
protected:
	std::vector<Vertex> vertices;
	std::vector<Vertex> vertices_wrk;
};
class Edge
{
public:
	Edge() :L(0), R(0) { line = N_; };
	Edge(int L, int R);
	Edge(const Edge& rhs);
	const int getFirst() const {return L;};
	const int getSecond() const {return R;};
	lineType getLineType() const {return line;};
	const bool operator<(const Edge &rhs) const;
	const bool operator==(const Edge &rhs) const;
	const void assignLineType(lineType l) { line = l; };
	~Edge();
private:
	int L, R;
	lineType line;
};
class Operator : public Vertex
{
public:
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
