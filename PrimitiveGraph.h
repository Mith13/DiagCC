
#include <vector>
#include <set>
#include <stack>
#include <memory>
#include <string>

enum lineType { N_ = 0, H_ = 1, P_ = -1 };
class Vertex
{
public:
	Vertex() :up(0), down(0), t_pos(-1), index(-1),is_external(false) {};
	Vertex(int top, int bot, int tpos) :up(top), down(bot), t_pos(tpos), index(-1),is_external(false) {};
	Vertex(const Vertex& rhs);

	virtual void setup(int up, int down, int t_pos);
	virtual void setIndex(int i);

	void contractUp();
	void decontractUp();
	void contractDown();
	void decontractDown();
	void setExternal();

	const int getTimePosition() const;
	const int getUpLines() const;
	const int getDownLines() const;
	const int getIndex() const;
	bool operator<(const Vertex & rhs) const;

protected:
	int up, down;
	int t_pos;
	int index;
	bool is_external;
};
class VertexPool
{
public:
	VertexPool();
	VertexPool(std::vector<Vertex>& pool) :vertices(pool), vertices_wrk(pool) {};
	VertexPool(const VertexPool& pool);
	VertexPool& operator= (const VertexPool& pool);
	VertexPool& getPointer() { return *this; };
	~VertexPool();

	void addVertex(const Vertex& v);
	void reset();

	virtual Vertex& getVertex(int pos) { return vertices_wrk[pos]; };
	virtual void getTimedEquivalentVertices(std::vector<std::vector<int>>& equivalent_vertices, const int t_pos, const int up_lines, const int down_lines)const;

	const std::vector<Vertex>::iterator begin() { return vertices.begin(); }
	const std::vector<Vertex>::iterator end() { return vertices.end(); }

	const int countVertices()const { return vertices_wrk.size(); };
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
	const int getFirst() const { return L; };
	const int getSecond() const { return R; };
	lineType getLineType() const { return line; };
	const bool operator<(const Edge &rhs) const;
	const bool operator==(const Edge &rhs) const;
	const void assignLineType(lineType l) { line = l; };
	~Edge();
private:
	int L, R;
	lineType line;
};
// Namespace for graphs isomorphism determination
namespace graphs
{

	// Class for pseudographs
	// As we can have multiple edges from same node, we need to distinquish 
	// two edges from same node and two edges from different edges
	class PseudoEdge
	{
	public:
		PseudoEdge() :m_degree(0), m_nodes(0), m_node_types() {};
		int m_degree;
		int m_nodes;
		std::set<int> m_node_types;
		PseudoEdge& operator +=(const int& e) {
			m_degree += e;
			m_nodes++;
			if (e != 0) m_node_types.insert(e);
			return *this;
		}
	};

	// Node class for search tree
	class TreeNode
	{
	public:
		TreeNode(std::vector<std::set<int>>& data) :m_data(data), m_child(), m_it_target_vertex(), m_it_target_set() {};
		TreeNode(std::vector<std::set<int>>&& data) :m_data(std::forward<std::vector<std::set<int>>>(data)), m_child(), m_it_target_vertex(), m_it_target_set() {};
		TreeNode() = delete;
		std::vector<std::set<int>> m_data;
		std::set<int>::iterator m_it_target_vertex;
		std::vector<std::set<int>>::iterator m_it_target_set;
		std::vector<std::shared_ptr<TreeNode>>m_child;
	};

	class PrimitiveGraph
	{
	public:
		PrimitiveGraph() = delete;
		PrimitiveGraph(int n_nodes) :m_adjacent_list(n_nodes), m_adjacency_matrix(n_nodes, std::vector<int>(n_nodes)), m_canonical_label(n_nodes) {};
		PrimitiveGraph(std::vector<std::vector<int>> adjacency_matrix) :m_adjacency_matrix(adjacency_matrix) {
			for (int i = 0; i < adjacency_matrix.size(); i++) {
				for (int j = 0; j < i; j++) {
					if (adjacency_matrix[i][j] > 0) {
						m_adjacent_list[i].insert(j);
						m_adjacent_list[j].insert(i);
					}
				}
			}
		}
		PrimitiveGraph(const PrimitiveGraph& g);
		PrimitiveGraph(const PrimitiveGraph&& g);

		void insertEdge(int a1, int a2);
		void createCanonicalLabel();
		void printAdjacencyMatrix() const;

		bool isIsomoprhic(PrimitiveGraph& rhs);

		int getVertices() const { return m_adjacent_list.size(); }
		int getAdjacency(int i, int j) const { return m_adjacency_matrix[i][j]; }

		std::vector<int> m_canonical_label;
		~PrimitiveGraph();
	private:
		bool refineColouring(std::vector<std::set<int>>& colouring_out, std::vector<std::set<int>>& colouring_wrk) const;

		int lower_permutation(const std::vector<int>& perm1, const std::vector<int>& perm2) const;

		std::vector<std::vector<int>> findIsomorphisms() const;
		std::vector<int> next_permutation(std::stack<std::shared_ptr<TreeNode>>& tree) const;
		std::vector<std::set<int>> equipartition() const;

		std::vector<std::vector<int>> m_adjacency_matrix;
		std::vector<std::set<int>> m_adjacent_list;
	};

};