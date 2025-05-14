/*
Reference:
Brute Force 관련
ChatGpt
Branch and bound 관련
https://www.youtube.com/watch?v=x2VnAMSH9Is
DP 관련
hw3_22100062.cpp
Greedy 관련
https://github.com/Kumar-laxmi/Algorithms/blob/main/C%2B%2B/Greedy-Algorithm/fractional_knapsack.cpp

chrono library https://en.cppreference.com/w/cpp/chrono
CPU 측정 https://velog.io/@zeouscik/C-%EC%BD%94%EB%93%9C-%EC%8B%A4%ED%96%89-%EC%8B%9C%EA%B0%84

*/
#include <iostream> //기본 입출력
#include <vector>   //벡터
#include <algorithm>    //sort
#include <chrono>   //코드 실행 시간 측정 및 처리
#include <cstdlib>  //rand
#include <ctime>    //CPU 시간 처리
#include <queue>    //stl queue

using namespace std;
using namespace std::chrono;

struct Item { //구조체
    int weight, benefit;
    double ratio;
};

vector<Item> generateItems(int n) {
    srand(100);
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].benefit = rand() % 500 + 1;
        items[i].weight = rand() % 100 + 1;
        items[i].ratio = (double)items[i].benefit / items[i].weight;
    }
    return items;
}

// 1. Brute Force
int knapsackBrute(const vector<Item>& items, int W, int idx = 0, int currWeight = 0, int currBenefit = 0) {
    if (idx == items.size()) return (currWeight <= W) ? currBenefit : 0;
    int include = knapsackBrute(items, W, idx + 1, currWeight + items[idx].weight, currBenefit + items[idx].benefit);
    int exclude = knapsackBrute(items, W, idx + 1, currWeight, currBenefit);
    return max(include, exclude);
}

// 2. Greedy (Fractional)
double knapsackGreedy(vector<Item> items, int W) {
    sort(items.begin(), items.end(), [](Item a, Item b) {
        return a.ratio > b.ratio;
    });
    double totalBenefit = 0;
    for (auto& item : items) {
        if (W >= item.weight) {
            W -= item.weight;
            totalBenefit += item.benefit;
        } else {
            totalBenefit += item.ratio * W;
            break;
        }
    }
    return totalBenefit;
}

// 3. Dynamic Programming
int knapsackDP(const vector<Item>& items, int W) {
    int n = items.size();
    vector<vector<int>> dp(n+1, vector<int>(W+1, 0));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (items[i-1].weight <= w)
                dp[i][w] = max(dp[i-1][w], items[i-1].benefit + dp[i-1][w - items[i-1].weight]);
            else
                dp[i][w] = dp[i-1][w];
        }
    }
    return dp[n][W];
}

// 4. Branch and Bound
struct Node {
    int level, benefit, weight;
    double bound;
    bool operator<(const Node& n) const {
        return bound < n.bound;
    }
};

double bound(const Node& u, int W, int n, const vector<Item>& items) {
    if (u.weight >= W) return 0;
    double profitBound = u.benefit;
    int j = u.level + 1;
    int totWeight = u.weight;

    while (j < n && totWeight + items[j].weight <= W) {
        totWeight += items[j].weight;
        profitBound += items[j].benefit;
        j++;
    }
    if (j < n)
        profitBound += (W - totWeight) * items[j].ratio;
    return profitBound;
}

int knapsackBB(vector<Item> items, int W) {
    sort(items.begin(), items.end(), [](Item a, Item b) {
        return a.ratio > b.ratio;
    });

    priority_queue<Node> Q;
    Node u, v;
    int n = items.size();
    v.level = -1; v.benefit = v.weight = 0;
    v.bound = bound(v, W, n, items);
    Q.push(v);
    int maxProfit = 0;

    while (!Q.empty()) {
        v = Q.top(); Q.pop();
        if (v.bound > maxProfit && v.level < n - 1) {
            u.level = v.level + 1;
            u.weight = v.weight + items[u.level].weight;
            u.benefit = v.benefit + items[u.level].benefit;

            if (u.weight <= W && u.benefit > maxProfit)
                maxProfit = u.benefit;

            u.bound = bound(u, W, n, items);
            if (u.bound > maxProfit) Q.push(u);

            u.weight = v.weight;
            u.benefit = v.benefit;
            u.bound = bound(u, W, n, items);
            if (u.bound > maxProfit) Q.push(u);
        }
    }
    return maxProfit;
}

// 테스트 및 출력
void test(int n) {
    auto items = generateItems(n);
    int W = n * 25;

    cout << "Items: " << n << ", Capacity: " << W << endl;

    auto start = high_resolution_clock::now();
    int b = knapsackBrute(items, W);
    auto end = high_resolution_clock::now();
    auto t1 = duration<double, milli>(end - start).count();

    start = high_resolution_clock::now();
    double g = knapsackGreedy(items, W);
    end = high_resolution_clock::now();
    auto t2 = duration<double, milli>(end - start).count();

    start = high_resolution_clock::now();
    int d = knapsackDP(items, W);
    end = high_resolution_clock::now();
    auto t3 = duration<double, milli>(end - start).count();

    start = high_resolution_clock::now();
    int bb = knapsackBB(items, W);
    end = high_resolution_clock::now();
    auto t4 = duration<double, milli>(end - start).count();

    cout << "Brute Force   : " << t1 << "ms / " << b << endl;
    cout << "Greedy        : " << t2 << "ms / " << g << endl;
    cout << "Dynamic Prog. : " << t3 << "ms / " << d << endl;
    cout << "Branch&Bound  : " << t4 << "ms / " << bb << endl << endl;
}

int main() {
    test(11);
    test(21);
    test(31);
    test(100);
    test(1000);
    test(10000);
    return 0;
}
