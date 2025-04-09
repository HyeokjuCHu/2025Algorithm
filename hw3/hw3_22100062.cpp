#include <iostream>
#include <climits>
#include <vector>
#include <ctime>  // clock()

using namespace std;

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

// DP 방식
int dpSolution(int k, int n) {
    vector<vector<int>> dp(k + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= k; i++) dp[i][0] = 0;
    for (int j = 1; j <= n; j++) dp[1][j] = j;

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

    // Recursive 방식 시간 측정
    clock_t start1 = clock();
    int rec_result = recursive(k, n);
    clock_t stop1 = clock();
    double rec_duration = (double)(stop1 - start1) / CLOCKS_PER_SEC;

    // DP 방식 시간 측정
    clock_t start2 = clock();
    int dp_result = dpSolution(k, n);
    clock_t stop2 = clock();
    double dp_duration = (double)(stop2 - start2) / CLOCKS_PER_SEC;

    // 출력
    cout << "\n--- Results ---" << endl;
    cout << "Recursive Solution: " << endl;
    cout << "Minimum number of drops = " << rec_result << endl;
    cout << "Time elapsed = " << rec_duration << " seconds" << endl;

    cout << "\nDP Solution: " << endl;
    cout << "Minimum number of drops = " << dp_result << endl;
    cout << "Time elapsed = " << dp_duration << " seconds" << endl;

    return 0;
}
