#pragma once

#include <map>

class Graph {
protected:
	struct Node {
		Node() : visited(false) {}
		set<int> edges;
		bool visited;
	};
	map<int, Node> graph_;

	void clearVisited() {
		for (auto &p : graph_) {
			p.second.visited = false;
		}
	}

	void DFS(int node) {
		if (!graph_[node].visited) {
			graph_[node].visited = true;
			for (int i : graph_[node].edges) {
				DFS(i);
			}
			doAction(node);
		}
	}

public:
	Graph() {}
	virtual ~Graph() {}
	int nodes() { return graph_.size(); }
	int edges() {
		int edges = 0;
		for (auto &p : graph_) {
			edges += p.second.edges.size();
		}
		return edges;
	}
	const map<int, Node>* graph() const { return &graph_; }

	virtual void doAction(int i) = 0;
	virtual void cleanUp() = 0;

	void DFSForest(stack<int> &order) {
		while(!order.empty()) {
			DFS(order.top());
			order.pop();
			cleanUp();
		}
		clearVisited();
	}

	void addNode() {
		graph_[nodes()];
	}

	void addEdge(int from, int to) {
		graph_[from].edges.insert(to);
	}

	friend ostream& operator<< (ostream &out, const Graph &g) {
		for (auto &p : (*g.graph())) {
			out << "R" << p.first << ":";
			bool first = true;
			for (int i : p.second.edges) {
				if (!first) out << ",";
				else first = false;
				out << "R" << i;
			}
			out << endl;
		}
		return out;
	}
};

class PostorderGraph : public Graph {
private:
	stack<int> postorderStack_;
public:
	PostorderGraph() {}
	virtual ~PostorderGraph() {}
	void doAction(int i) {
		postorderStack_.push(i);
	}
	void cleanUp() {}
	stack<int>* postorderStack() { return &postorderStack_; }
};

class SCCGraph : public Graph {
private:
	vector<set<int> > SCCs_;
	set<int> currSCC;
public:
	SCCGraph() {}
	virtual ~SCCGraph() {}
	void doAction(int i) {
		currSCC.insert(i);
	}
	void cleanUp() {
		if (currSCC.size() > 0) {
			SCCs_.push_back(currSCC);
			currSCC.clear();
		}
	}
	PostorderGraph reverse() {
		PostorderGraph reverse;
		for (int i = 0; i < nodes(); i++) {
			reverse.addNode();
		}
		for (auto &p : graph_) {
			for (int i : p.second.edges) {
				reverse.addEdge(i, p.first);
			}
		}
		return reverse;
	}
	vector<set<int> >* SCCs() { return &SCCs_; }
};
