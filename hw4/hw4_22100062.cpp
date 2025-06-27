/*
Reference:
Brute Force 관련
ChatGpt
Branch and bound 관련
https://www.youtube.com/watch?v=x2VnAMSH9Is
https://seungjuitmemo.tistory.com/110
DP 관련
hw3_22100062.cpp
Greedy 관련
https://github.com/Kumar-laxmi/Algorithms/blob/main/C%2B%2B/Greedy-Algorithm/fractional_knapsack.cpp

chrono library https://en.cppreference.com/w/cpp/chrono
CPU 측정 https://velog.io/@zeouscik/C-%EC%BD%94%EB%93%9C-%EC%8B%A4%ED%96%89-%EC%8B%9C%EA%B0%84
*/

//100부터는 시간 오래 걸립니다.

#include <iostream> //기본 입출력
#include <vector>   //벡터
#include <algorithm>    //sort
#include <chrono>   //코드 실행 시간 측정 및 처리
#include <cstdlib>  //rand
#include <ctime>    //CPU 시간 처리
#include <queue>    //stl queue

using namespace std;
using namespace std::chrono;

struct Item {//구조체
    int weight, benefit;
    double ratio;
};

//generate random items
vector<Item> generateItems(int n) {
    srand(100);
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].benefit = rand() % 500 + 1;
        items[i].weight = rand() % 100 + 1;
        items[i].ratio = (double)items[i].benefit / items[i].weight;
    }
    return items; //return the generated list of items
}

// Brute Force
int knapsackBrute(const vector<Item>& items, int W, int idx = 0, int currWeight = 0, int currBenefit = 0) {
    if (idx == items.size()) return (currWeight <= W) ? currBenefit : 0;
    int include = knapsackBrute(items, W, idx + 1, currWeight + items[idx].weight, currBenefit + items[idx].benefit);
    int exclude = knapsackBrute(items, W, idx + 1, currWeight, currBenefit);
    return max(include, exclude);
}

// Greedy (Fractional)
double knapsackGreedy(vector<Item> items, int W) {
    sort(items.begin(), items.end(), [](Item a, Item b) {
        return a.ratio > b.ratio;
    });
    double totalBenefit = 0;
     //greedy pick items to fill the knapsack
    for (auto& item : items) {
        if (W >= item.weight) { //item can fit in the knapsack
            W -= item.weight; //가능한 용량 줄이기
            totalBenefit += item.benefit;
        } else {//take the fractional part
            totalBenefit += item.ratio * W;
            break;
        }
    }
    return totalBenefit;
}

//dynamic Programming
int knapsackDP_optimized(const vector<Item>& items, int W) {
    vector<int> dp(W + 1, 0);
    for (auto& item : items) {
        for (int w = W; w >= item.weight; --w) {
            dp[w] = max(dp[w], dp[w - item.weight] + item.benefit);
        }
    }
    return dp[W];
}

// Branch and Bound
struct Node {
    int level, benefit, weight;
    double bound;
    bool operator<(const Node& n) const {
        return bound < n.bound;// compare nodes based on bound
    }
};

double bound(const Node& u, int W, int n, const vector<Item>& items) {
    if (u.weight >= W) return 0; //최대용량 넘어가면 0으로 return
    double profitBound = u.benefit;
    int weightSum = u.weight;
    int i = u.level + 1;

    while (i < n && weightSum + items[i].weight <= W) {
        weightSum += items[i].weight;
        profitBound += items[i].benefit;
        i++;
    }

    if (i < n)
        profitBound += (W - weightSum) * items[i].ratio;

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
    v.bound = bound(v, W, n, items);// calculate the bound for the initial node
    Q.push(v);//add the initial node to the priority queue
    int maxProfit = 0;

    while (!Q.empty()) {
        v = Q.top(); Q.pop();
        if (v.bound > maxProfit && v.level < n - 1) {
            u.level = v.level + 1;
            u.weight = v.weight + items[u.level].weight;
            u.benefit = v.benefit + items[u.level].benefit;

            if (u.weight <= W && u.benefit > maxProfit) //더 나은 답이면 maxProfit 최신화
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

// 테스트 함수
void test(int n) {
    auto items = generateItems(n);
    int W = n * 25;// set knapsack capacity based on the number of items

    cout << "Items: " << n << ", Capacity: " << W << endl;

    auto start = high_resolution_clock::now(); //auto를 사용함으로써 반환 타입을 정확히 명시함을 간소화 가능
    int b = knapsackBrute(items, W);
    auto end = high_resolution_clock::now();
    auto t1 = duration<double, milli>(end - start).count();
    cout << "Brute Force   : " << t1 << "ms / " << b << endl;

    start = high_resolution_clock::now();
    double g = knapsackGreedy(items, W);
    end = high_resolution_clock::now();
    auto t2 = duration<double, milli>(end - start).count();
    cout << "Greedy        : " << t2 << "ms / " << g << endl;

    start = high_resolution_clock::now();
    int d = knapsackDP_optimized(items, W);
    end = high_resolution_clock::now();
    auto t3 = duration<double, milli>(end - start).count();
    cout << "Dynamic Prog. : " << t3 << "ms / " << d << endl;

    start = high_resolution_clock::now();
    int bb = knapsackBB(items, W);
    end = high_resolution_clock::now();
    auto t4 = duration<double, milli>(end - start).count();
    cout << "Branch&Bound  : " << t4 << "ms / " << bb << endl;

    cout << "------------------------------\n";
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
