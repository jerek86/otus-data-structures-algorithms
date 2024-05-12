#include <iostream>
#include <vector>
#include <string>

using namespace std;

constexpr int MAX_N = 100;

int main() {
    int n;
    int tree[MAX_N][MAX_N];

    cin >> n;

    if (n <= 0 || n >= MAX_N) {
        cout << "n is invalid: " << n << endl;
        return -1;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            cin >> tree[i][j];
        }
    }

    for (int i = n - 2; i >= 0; --i) {
        for (int j = 0; j <= i; ++j) {
            tree[i][j] += max(tree[i+1][j], tree[i+1][j+1]);
        }
    }

    cout << tree[0][0];

    return 0;
}
