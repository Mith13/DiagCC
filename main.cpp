#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include "test.h"
#include "hugenholtz.h"
#include "auxiliary.h"
/*
class parent
{
public:
	parent(int age, std::string name)
	{
		Age = age;
		Name = name;
	};
	std::vector<std::unique_ptr<parent>> mitosis();
	virtual void print() {
		std::cout << Name << " is " << Age << " old\n";
	}
	~parent(){
		std::cout << "Deleting Parent" << std::endl;
	}
	int Age;
	std::string Name;
};
class kid : public parent
{
public:
	kid(int age, std::string name, int grade) :parent(age,name),grade(grade){};
	int grade;
	void print() {
		std::cout << Name << " is " << Age << " old with grade " << grade << std::endl;
	}
	~kid(){
		std::cout << "Deleting Diagram" << std::endl;
	}
};

std::vector<std::unique_ptr<parent>> parent::mitosis() {
	std::vector<std::unique_ptr<parent>> kids;

	std::unique_ptr<parent> first = std::make_unique<kid>(0, "kubko", 1);
	std::unique_ptr<parent> second = std::make_unique<kid>(0, "Matko", 3);

	//kids.push_back(std::make parent(0, "matko"));
	kids.push_back(std::move(first));
	kids.push_back(std::move(second));
	return kids;
}*/
using std::cout;
using std::endl;
/*struct myStack{
    std::vector<std::vector<int>>stack;
    std::vector<std::vector<int>> final_set;
    int idx;
};
void recur(std::vector<std::vector<int>>& stack, std::vector<int>& final_set, int idx)
{
	if (idx == stack.size()) {
		for (auto s : final_set)
		{
			
			{
				cout << s << " ";
			}
		}
		cout << endl;
		return;
	}
	do
	{
		final_set.insert(final_set.end(),stack[idx].begin(),stack[idx].end());
		recur(stack, final_set,idx+1);
		final_set.resize(final_set.size()-stack[idx].size());
	} while (std::next_permutation(stack[idx].begin(), stack[idx].end()));
}
/*void iterate(std::vector<std::vector<int>> stack, std::vector<std::vector<int>> final_set, int idx)
{
	std::stack <myStack>final_Stack;
    myStack first;
    first.stack=stack;
    first.final_set=final_set;
    first.idx=idx;
	final_Stack.push(first);
	while (!final_Stack.empty())
	{
        myStack tmp_stack=final_Stack.pop();
		if (idx == stack.size())
        {
            for (auto s : final_set)
            {
                for (auto e : s) 
                {
                    cout << e << " ";
                }
            }
            cout << endl;
            return;
        }

		     
             
        final_Stack.push(stack[++idx]);
	}
}*/
int main(int argc, char **argv)
{
	std::vector<int> set1{ 1,2,3 };
	std::vector<int> set2{ 4,5 };
	std::vector<std::vector<int>> sets;
	sets.push_back(set1);
	sets.push_back(set2);
	std::vector<int> final_set;
	//recur(sets, final_set, 0);
	/*
	do
	{
		final_set.clear();
		final_set.insert(final_set.end(), set1.begin(), set1.end());
		do
		{
			final_set.insert(final_set.end(), set2.begin(), set2.end());
			for (auto v : final_set)
			{
				cout << v << " ";
			}
			cout << endl;
			final_set.resize(final_set.size() - set2.size());
		} while (std::next_permutation(set2.begin(), set2.end()));
	} while (std::next_permutation(set1.begin(), set1.end()));
	
	final_set.clear();
	int index = -1;
	while (true)
	{
		do
		{
			index++;
			final_set.insert(final_set.end(),sets[index].begin(), sets[index].end());
			if (index == sets.size()-1) {
				for (auto v : final_set)
				{
					cout << v << " ";
				}
				cout << endl;
				final_set.resize(final_set.size() - set2.size());
				index--;
				//break;
			}
		} while (std::next_permutation(sets[index].begin(), sets[index].end()));
		index--;
	}
	*/
	Vertex v1;
    v1.setup(2,0,0);
    Vertex v2;
    v2.setup(0,3,1);
    Operator v3;
    v3.setup(0,2,2);
    Vertex v4(0,4,3);
    std::shared_ptr<VertexPool> vertices(std::make_shared<VertexPool>());
	
	vertices->addVertex(Vertex(2,0,0));
	vertices->addVertex(Operator(2,0,0));
    vertices->addVertex(Vertex(1,3,1));
	vertices->addVertex(Vertex(0,2,2));
/*
	vertices->addVertex(v1);
	vertices->addVertex(v1);
	vertices->addVertex(v2);
	vertices->addVertex(v3);
*/
	std::shared_ptr<VertexPool> vec2(vertices);
	cout<<"Starting the Diagram generation"<<endl;
    Tree strom(vertices);
	DiagramContainer<std::unique_ptr<Diagram>> diag_cont;
    std::vector<std::unique_ptr<Diagram>>& diagrams=strom.getDiagrams();
    for(auto& d:diagrams){
        std::vector<std::unique_ptr<Diagram>> HughDiagrams=d->createHugenholtzDiagrams();
        for(auto & Hd : HughDiagrams){
			cout << "Printing Hugenholtz" <<endl;
            Hd->printSimple();
			diag_cont.insert(std::move(Hd));
        }
    }
    
	std::string err = "Testing error";
	aux::abortMessage(err);

    return 0;        
}
