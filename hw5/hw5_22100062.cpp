/*
강의 자료: week_10_DFSapp_up.pdf  9쪽부터 아래 다
웹: https://yoongrammer.tistory.com/86
*/
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stack>

using namespace std;

const int MAX_NODES = 10; //10개로 제한

struct Graph {
    map<char, vector<char>> adjList; // 인접 리스트
    set<char> vertices;              // vertices
};

map<char, int> startTime;
map<char, int> finishTime;
set<char> visited;
stack<char> topoStack;
int timer = 0;

// 정점 추가
void addVertex(Graph& g, char v) {
    g.vertices.insert(v);
    if (g.adjList.find(v) == g.adjList.end()) {
        g.adjList[v] = vector<char>();
    }
}

// add edge
void addEdge(Graph& g, char from, char to) {
    addVertex(g, from);
    addVertex(g, to);

    vector<char>& neighbors = g.adjList[from];
    if (find(neighbors.begin(), neighbors.end(), to) == neighbors.end()) {
        neighbors.push_back(to);
        sort(neighbors.begin(), neighbors.end()); // sort alphabetical
    }
}

// DFS visit
void dfsVisit(Graph& g, char v) {
    visited.insert(v);
    startTime[v] = ++timer;

    for (char neighbor : g.adjList[v]) {
        if (visited.find(neighbor) == visited.end()) {
            dfsVisit(g, neighbor);
        }
    }

    finishTime[v] = ++timer;
    topoStack.push(v); // for topological sort
}

// DFS start
void DFS(Graph& g) {
    for (char v : g.vertices) {
        if (visited.find(v) == visited.end()) {
            dfsVisit(g, v);
        }
    }
}

// adj list
void printGraph(Graph& g) {
    cout << "\nAdjacency List:\n";
    for (char v : g.vertices) {
        cout << v << ": ";
        for (char neighbor : g.adjList[v]) {
            cout << neighbor << " ";
        }
        cout << '\n';
    }
}

// time print
void printTimes() {
    cout << "\nStart and Finish Times:\n";
    for (auto& p : startTime) {
        cout << p.first << ": start=" << startTime[p.first]
             << ", finish=" << finishTime[p.first] << '\n';
    }
}

// topological sort result print
void printTopologicalSort() {
    cout << "\nTopological Order:\n";
    while (!topoStack.empty()) {
        cout << topoStack.top() << " ";
        topoStack.pop();
    }
    cout << '\n';
}

int main() {
    Graph g;

    addEdge(g, 'A', 'B');
    addEdge(g, 'A', 'C');
    addEdge(g, 'B', 'D');
    addEdge(g, 'C', 'D');
    addEdge(g, 'D', 'E');
    addEdge(g, 'E', 'F');

    printGraph(g);
    DFS(g);
    printTimes();
    printTopologicalSort();

    return 0;
}
