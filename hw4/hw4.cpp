#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

int bruteforce(int capacity, int b[], int w[], int n){
    int maxBenefit = 0;
    long long size = (1LL << n); // 2^n

    for(int i = 0; i < size; i++){
        int total_w = 0;
        int total_b = 0;

        for(int j = 0; j < n; j++){
            if(i & (1 << j)){    // j번째 아이템 고른 경우
                total_w += w[j];
                if (total_w > capacity) break;
                total_b += b[j];
            }
        }
        if(total_w <= capacity){
            maxBenefit = max(total_b, maxBenefit);
        }
    }

    return maxBenefit;
}

const int seed = 100;   // seed

// Brute force Algorithm
int main() {
    cout << "1. Brute force" << endl;
    cout << "Number of Items | Processing time in milliseconds / Maximum benefit value" << endl;

    for(int i = 1; i <= 3; i++){
        int item = i * 10 + 1;  // Generate item counts 11, 21, 31
        int capacity = item * 25;

        int benefit[item];
        int weight[item];
        srand(seed);
        for(int i = 0; i < item; i++){
            benefit[i] = rand() % 500 + 1;
            weight[i] = rand() % 100 + 1;
        }

        clock_t start = clock();
        int max = bruteforce(capacity, benefit, weight, item);
        clock_t end = clock();

        double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

        cout << item << " | " << duration << " / " << max << endl;
    }
    
    return 0;
}
