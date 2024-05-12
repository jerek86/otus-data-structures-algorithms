#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    int n;

    cin >> n;

    long f5 = 1;
    long f8 = 1;
    long f55 = 0;
    long f88 = 0;

    for (int i = 1; i < n; ++i) {
        long n5 = f8 + f88;
        long n8 = f5 + f55;
        long n55 = f5;
        long n88 = f8;

        f5 = n5;
        f8 = n8;
        f55 = n55;
        f88 = n88;
    }

    long result = f5 + f8 + f55 + f88;

    cout << result << endl;

    return 0;
}
