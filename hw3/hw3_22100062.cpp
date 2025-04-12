#include <iostream>
#include <climits>
#include <vector>
#include <ctime>

using namespace std;

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

int DP(int k, int n) {
    int dp[k + 1][n + 1];

    for (int i = 1; i <= k; i++) {
        dp[i][0] = 0;
        dp[i][1] = 1;
    }
    for (int j = 1; j <= n; j++) {
        dp[1][j] = j;
    }

    for (int i = 2; i <= k; i++) {  // Objects
        for (int j = 2; j <= n; j++) {  // Meters

            dp[i][j] = INT_MAX;

            // Try dropping from every height X (1 to j)
            for (int x = 1; x <= j; x++) {
                // case 1: when object breaks
                int b = dp[i - 1][x - 1];
                // case 2: when object survives
                int s = dp[i][j - x];
                int worst = 1 + max(b, s);
                dp[i][j] = min(dp[i][j], worst);  // Take minimum over all trials
            } // end of for x

        } // end of for j
    } // end of for i

    return dp[k][n];
}


int main() {
    int n, k;
    cout << "Enter the number of meters (n): ";
    cin >> n;
    cout << "Enter the number of fragile objects (k): ";
    cin >> k;

    clock_t start1 = clock();
    int rec_result = recursive(k, n);
    clock_t stop1 = clock();
    double rec_duration = (double)(stop1 - start1) / CLOCKS_PER_SEC;

    clock_t start2 = clock();
    int dp_result = DP(k, n);
    clock_t stop2 = clock();
    double dp_duration = (double)(stop2 - start2) / CLOCKS_PER_SEC;

    cout << "\n--- Results ---" << endl;
    cout << "Recursive Solution: " << endl;
    cout << "Minimum number of drops = " << rec_result << endl;
    cout << "Time elapsed = " << rec_duration << " seconds" << endl;

    cout << "\nDP Solution: " << endl;
    cout << "Minimum number of drops = " << dp_result << endl;
    cout << "Time elapsed = " << dp_duration << " seconds" << endl;

    return 0;
}
