#include <iostream>
#include <vector>
#include <string>

using namespace std;

int N = 0;
int* map = nullptr;

static inline int index(int x, int y) {
    return (y * N) + x;
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

static inline void walk(int x, int y) {
    if (x < 0 || x >= N) return;
    if (y < 0 || y >= N) return;
    if (map[index(x,y)] == 0) return;
    
    map[index(x,y)] = 0;

    walk(x - 1, y);
    walk(x + 1, y);
    walk(x, y - 1);
    walk(x, y + 1);
}

int main() {
    string line;

    getline(cin, line);
    N = stoi(line);

    map = new int [N*N];

    for (int y = 0; y < N; ++y) {
        getline(cin, line);
 
        int x = 0;
        for (auto num : string_split(line, " ")) {
            map[index(x, y)] = stoi(num);
            ++x;
        }
    }

    int islands = 0;
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            if (map[index(x, y)] == 1) {
                islands++;
                walk (x, y);
            }
        }
    }

    cout << islands << endl;

    delete [] map;

    return 0;
}
