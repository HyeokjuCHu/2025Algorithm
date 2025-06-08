#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <limits>
#include <iomanip>

using namespace std;

const int INF = 1e9;
const int MAX_V = 20;

int V;
vector<string> city_names;
vector<vector<int>> adj_matrix;
vector<vector<pair<int, int>>> adj_list;

// 문자열 "INF"를 정수 INF로 변환
int parseWeight(const string& s) {
    if (s == "INF") return INF;
    return stoi(s);
}

// 그래프 파일 읽기
void readGraph(const string& filename) {
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string value;
        vector<int> row;
        while (getline(iss, value, '\t')) {
            row.push_back(parseWeight(value));
        }
        adj_matrix.push_back(row);
    }
    V = adj_matrix.size();

    // 인접 리스트 생성
    adj_list.resize(V);
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (adj_matrix[i][j] != INF && i != j) {
                adj_list[i].emplace_back(j, adj_matrix[i][j]);
            }
        }
    }
}

// Dijkstra Algorithm (from source to all vertices)
vector<int> dijkstra(int src) {
    vector<int> dist(V, INF);
    dist[src] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, src);

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj_list[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

// Floyd-Warshall Algorithm
vector<vector<int>> floydWarshall() {
    vector<vector<int>> dist = adj_matrix;
    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    return dist;
}

// 결과 출력
void printDistanceMatrix(const vector<vector<int>>& dist) {
    for (const auto& row : dist) {
        for (int d : row) {
            if (d == INF) cout << "INF\t";
            else cout << d << "\t";
        }
        cout << "\n";
    }
}

int main() {
    readGraph("homework6.data");

    cout << "1) The shortest distance between cities using Dijkstra's algorithm is:\n";
    vector<vector<int>> dij_result(V, vector<int>(V));
    for (int i = 0; i < V; ++i)
        dij_result[i] = dijkstra(i);
    printDistanceMatrix(dij_result);

    cout << "\n2) The shortest distance between cities using Floyd's algorithm is:\n";
    auto floyd_result = floydWarshall();
    printDistanceMatrix(floyd_result);

    return 0;
}
