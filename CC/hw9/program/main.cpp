#include <bits/stdc++.h>
using namespace std;

typedef unsigned int ui;

/* Ref: https://en.wikipedia.org/wiki/Partition_problem */

bool solve(const vector<int>& arr) {

    /* Partition solving algorithm */

    // declaration
    ui N = arr.size();
    int K = accumulate(arr.begin(), arr.end(), 0);

    if(K == 0 || K%2) return false;

    vector< vector<bool> > p(K/2+1, vector<bool>(N+1, false));
    
    // Initialization
    for(int i = 0; i <= N; i++) p[0][i] = true;

    for(int i = 1; i <= K/2; i++) {
        for(int j = 1; j <= N; j++) {
            if(i - arr[j-1] >= 0) {
                p[i][j] = (p[i][j-1] || p[i-arr[j-1]][j-1]); 
            } else {
                p[i][j] = p[i][j-1];
            }
        }
    }

    return p[K/2][N];
}

bool is_number(string num) {
    for(ui i = 0; i < num.length(); i++) {
        if(!isdigit(num[i])) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    
    if(argc <= 1) {
        cerr << "Not enough arguments\n";
        return EXIT_SUCCESS;
    }

    vector<int> arr;
    for(int i = 1; i < argc; i++) {
    
        if(!is_number(argv[i])) continue;

        arr.push_back(atoi(argv[i]));
    }

    cout << solve(arr) << endl;
}