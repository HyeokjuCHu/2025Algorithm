/*
Reference: 
Dijkstra's Algorithm
https://yabmoons.tistory.com/364
https://learn.microsoft.com/ko-kr/cpp/standard-library/iomanip-functions?view=msvc-170

Floyd
https://ansohxxn.github.io/algorithm/floyd/

강의자료:
ch24-25

 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <limits>
#include <iomanip>

using namespace std;

const int INF = numeric_limits<int>::max() / 2;

//print the results
void printMatrix(const string &title, const vector<vector<int>> &dist,
                 const vector<string> &cities) {
  cout << title << endl;
  int n = cities.size();

  //header row
  cout << left << setw(10) << " ";
  for (const auto &city : cities) {
    cout << setw(8) << city.substr(0, 7);
  }
  cout << endl;

  //city's shortest path
  for (int i = 0; i < n; ++i) {
    cout << left << setw(10) << cities[i].substr(0, 9);
    for (int j = 0; j < n; ++j) {
      if (dist[i][j] >= INF) {
        cout << setw(8) << "INF";
      } else {
        cout << setw(8) << dist[i][j];
      }
    }
    cout << endl;
  }
  cout << endl;
}

bool readGraph(const string &filename, vector<string> &cities,
               vector<vector<int>> &adjMatrix) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Cannot open " << filename << endl;
    return false;
  }

  string line, word;
  if (getline(file, line)) {
    stringstream ss(line);
    while (ss >> word) {
      cities.push_back(word);
    }
  }

  int n = cities.size();
  if (n == 0) {
    cerr << "Error: Intel file is empty or header is missing." << endl;
    return false;
  }
  adjMatrix.assign(n, vector<int>(n));

  for (int i = 0; i < n; ++i) {
    if (!getline(file, line)) {
      cerr << "Incomplete matrix data. Expected " << n
           << " rows, but file ended." << endl;
      return false;
    }
    stringstream ss(line);
    ss >> word;
    for (int j = 0; j < n; ++j) {
      if (!(ss >> word)) {
        cerr << "Error: Incomplete matrix row for " << cities[i] << endl;
        return false;
      }
      if (word == "INF") {
        adjMatrix[i][j] = INF;
      } else {
        adjMatrix[i][j] = stoi(word);
      }
    }
  }
  file.close();
  return true;
}

// Dijkstra's Algorithm
vector<int> dijkstra(int startNode, int n,
                     const vector<vector<int>> &adjMatrix) {
  vector<int> dist(n, INF);
  dist[startNode] = 0;

  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
    pq;
  pq.push({0, startNode});

  while (!pq.empty()) {
    int d = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (d > dist[u]) continue;

    for (int v = 0; v < n; ++v) {
      if (adjMatrix[u][v] != INF && dist[u] + adjMatrix[u][v] < dist[v]) {
        dist[v] = dist[u] + adjMatrix[u][v];
        pq.push({dist[v], v});
      }
    }
  }
  return dist;
}

//Floyd-Warshall Algorithm
vector<vector<int>> floydWarshall(int n,
                                  const vector<vector<int>> &adjMatrix) {
  vector<vector<int>> dist = adjMatrix;

  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) { // Start node
      for (int j = 0; j < n; ++j) { // End node
        if (dist[i][k] < INF && dist[k][j] < INF) {
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }
  return dist;
}

int main() {
  vector<string> cities;
  vector<vector<int>> adjMatrix;

  if (!readGraph("homework6.data", cities, adjMatrix)) {
    return 1; // Mission abort.
  }

  int n = cities.size();

  vector<vector<int>> dijkstraResults(n, vector<int>(n));
  for (int i = 0; i < n; ++i) {
    dijkstraResults[i] = dijkstra(i, n, adjMatrix);
  }
  printMatrix(
    "1) The shortest distance between cities using Dijkstra's algorithm is:",
    dijkstraResults, cities);

  vector<vector<int>> floydResults = floydWarshall(n, adjMatrix);
  printMatrix(
    "2) The shortest distance between cities using Floyd's algorithm is:",
    floydResults, cities);

  return 0;