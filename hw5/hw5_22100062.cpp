#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stack>

using namespace std;

const int MAX_NODES = 10;

struct Graph {
    map<char, vector<char>> adjList; // 인접 리스트
    set<char> vertices;              // 정점 목록
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

// 간선 추가 (알파벳 정렬 유지)
void addEdge(Graph& g, char from, char to) {
    addVertex(g, from);
    addVertex(g, to);

    vector<char>& neighbors = g.adjList[from];
    if (find(neighbors.begin(), neighbors.end(), to) == neighbors.end()) {
        neighbors.push_back(to);
        sort(neighbors.begin(), neighbors.end()); // 알파벳 순 정렬
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
    topoStack.push(v); // 위상 정렬을 위한 후위 스택
}

// DFS 시작
void DFS(Graph& g) {
    for (char v : g.vertices) {
        if (visited.find(v) == visited.end()) {
            dfsVisit(g, v);
        }
    }
}

// 인접 리스트 출력
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

// 시간 정보 출력
void printTimes() {
    cout << "\nStart and Finish Times:\n";
    for (auto& p : startTime) {
        cout << p.first << ": start=" << startTime[p.first]
             << ", finish=" << finishTime[p.first] << '\n';
    }
}

// 위상 정렬 결과 출력
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

    // 예시 DAG 구성 (사이클 없음)
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
