#include <iostream>
#include <climits>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 재귀 방식
int recursive(int k, int n) {
    if (k == 1) return n;
    if (n == 0 || n == 1) return n;

    int min_attempts = INT_MAX;

    for (int x = 1; x <= n; x++) {
        int breaks = recursive(k - 1, x - 1);
        int survives = recursive(k, n - x);
        int worst_case = 1 + max(breaks, survives);

        min_attempts = min(min_attempts, worst_case);
    }

    return min_attempts;
}

// 동적 계획법 방식
int dpSolution(int k, int n) {
    vector<vector<int>> dp(k + 1, vector<int>(n + 1, 0));

    // Base case
    for (int i = 1; i <= k; i++) dp[i][0] = 0; // 0층은 0번 시도
    for (int j = 1; j <= n; j++) dp[1][j] = j; // 공 하나면 일일이 시도

    // DP 채우기
    for (int i = 2; i <= k; i++) {
        for (int j = 1; j <= n; j++) {
            dp[i][j] = INT_MAX;
            for (int x = 1; x <= j; x++) {
                int worst = 1 + max(dp[i - 1][x - 1], dp[i][j - x]);
                dp[i][j] = min(dp[i][j], worst);
            }
        }
    }

    return dp[k][n];
}

int main() {
    int n, k;
    cout << "Enter the number of meters (n): ";
    cin >> n;
    cout << "Enter the number of fragile objects (k): ";
    cin >> k;

    // Recursive 측정
    auto start_rec = high_resolution_clock::now();
    int rec_result = recursive(k, n);
    auto end_rec = high_resolution_clock::now();
    auto duration_rec = duration_cast<milliseconds>(end_rec - start_rec);

    // DP 측정
    auto start_dp = high_resolution_clock::now();
    int dp_result = dpSolution(k, n);
    auto end_dp = high_resolution_clock::now();
    auto duration_dp = duration_cast<milliseconds>(end_dp - start_dp);

    cout << "\n--- Results ---" << endl;
    cout << "Recursive Solution:" << endl;
    cout << "Minimum number of drops: " << rec_result << endl;
    cout << "Execution time: " << duration_rec.count() << " ms" << endl;

    cout << "\nDP Solution:" << endl;
    cout << "Minimum number of drops: " << dp_result << endl;
    cout << "Execution time: " << duration_dp.count() << " ms" << endl;

    return 0;
}
