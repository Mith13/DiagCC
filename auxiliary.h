#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <type_traits>

#include <vector>
#include <set>
#include <stack>

#define abortMessage(msg) __AbortMessage(msg,__FILE__,__LINE__)
namespace aux
{
    int __AbortMessage(std::string error, const char* file, int line);
}
namespace math
{
    template<class T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix
    {
    public:
        Matrix(int size_x,int size_y);
        Matrix(const Matrix& mat);
        T& operator=(const Matrix& mat);
        T& operator()(int i, int j);
        ~Matrix();
    private:
        T* data;
        size_t cols,rows;
    };
	
}
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

	class Graph
	{
	public:
		Graph() = delete;
		Graph(int n_nodes) :m_adjacent_list(n_nodes), m_adjacency_matrix(n_nodes, std::vector<int>(n_nodes)), m_canonical_label(n_nodes) {};
		Graph(const Graph& g);
		Graph(const Graph&& g);

		void insertEdge(int a1, int a2);
		void createCanonicalLabel();
		void printAdjacencyMatrix() const;

		bool isIsomoprhic(Graph& rhs);

		int getVertices() const { return m_adjacent_list.size(); }
		int getAdjacency(int i, int j) const { return m_adjacency_matrix[i][j]; }

		std::vector<int> m_canonical_label;
		~Graph();
	private:
		bool refineColouring(std::vector<std::set<int>>& colouring_out, std::vector<std::set<int>>& colouring_wrk) const;

		int lower_permutation(const std::vector<int>& perm1, const std::vector<int>& perm2) const;

		std::vector<std::vector<int>> findIsomorphisms() const;
		std::vector<int> next_permutation(std::stack<std::shared_ptr<TreeNode>>& tree) const;
		std::vector<std::set<int>> equipartition() const;

		std::vector<std::vector<int>> m_adjacency_matrix;
		std::vector<std::set<int>> m_adjacent_list;
	};

}
