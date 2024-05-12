#include <iostream>
#include <vector>
#include <string>

using namespace std;

inline static bool odd(int number) {
    return (number & 1) == 1;
}
inline static bool even(int number) {
    return (number & 1) == 0;
}

int NOD(int a, int b) {
    if (a == b) return a;
    if (a == 0) return b;
    if (b == 0) return a;
    if (even(a) && even(b)) return NOD(a >> 1, b >> 1) << 1;
    if (even(a) && odd(b)) return NOD(a >> 1, b);
    if (odd(a) && even(b)) return NOD(a, b >> 1);
    if (a > b) {
        return NOD((a - b), b);
    }
    
    return NOD(a, (b - a));
}

vector<string> string_split(string str, string delimiter) {
    vector<string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);

    return result;
}

int main() {
    string line;

    cin >> line;

    vector<string> input;
    for (string& substr : string_split(line, "+")) {
        auto temp = string_split(substr, "/");
        input.insert(input.end(), temp.begin(), temp.end());
    }

    int a = stoi(input[0]);
    int b = stoi(input[1]);
    int c = stoi(input[2]);
    int d = stoi(input[3]);

    int x = (a * d) + (b * c);
    int y = b * d;

    int nod = NOD(x, y);

    x /= nod;
    y /= nod;

    cout << x << "/" << y << endl;

    return 0;
}
