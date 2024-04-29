#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


using namespace std;
using namespace std::chrono;

class RLE {
public:
    RLE() = default;

    vector<char> compress(const vector<char>& input) {
        auto it = input.begin();
        char last_symbol = *it++;
        int counter = 1;

        vector<char> output;
        output.push_back(last_symbol);

        for (; it != input.end(); ++it) {
            if (*it != last_symbol || counter == 255) {
                output.push_back(counter);
                last_symbol = *it;
                output.push_back(last_symbol);
                counter = 1;
            } else {
                counter++;
            }
        }
        output.push_back(counter);

        return output;
    }

    vector<char> decompress(const vector<char>& input) {
        if (input.size() % 2 != 0) {
            cerr << "bad size" << endl;
            return vector<char>{}; 
        }

        vector<char> output;

        for (auto it = input.begin(); it != input.end(); ++it) {
            char symbol = *it;
            int count = static_cast<int>(*++it);

            for (int i = 0; i < count; ++i) {
                output.push_back(symbol);
            }
        }

        return output;
    }

    void dump_compressed_data(const vector<char>& input) {
        if (input.size() % 2 != 0) {
            cerr << "bad size" << endl;
            return;
        }

        for (auto it = input.begin(); it != input.end(); ++it) {
            char symbol = *it;
            int count = static_cast<int>(*++it);

            cout << "symbol " << symbol << " : " << count << " times" << endl;
        }
    }
};

class RLEPlus {
public:
    RLEPlus() = default;

    vector<char> compress(const vector<char>& input) {
        auto it = input.begin();
        char last_symbol = *it++;
        int counter = 1;

        vector<char> output;
        vector<char> temp;
        temp.push_back(last_symbol);

        for (; it != input.end(); ++it) {
            if (*it == last_symbol) {
                counter++;
                if (counter < 3) { // save to single order
                    temp.push_back(*it);
                    if (temp.size() >= 128) {
                        int single_order_symbols_count = temp.size();
                        output.push_back(static_cast<char>(-single_order_symbols_count));
                        output.insert(output.end(), temp.begin(), temp.begin() + single_order_symbols_count);
                        temp.clear();
                    }
                } else if (counter == 3) { // process single order
                    if (temp.size() > 2) {
                        char single_order_symbols_count = temp.size() - 2;
                        output.push_back(-single_order_symbols_count);
                        output.insert(output.end(), temp.begin(), temp.begin() + single_order_symbols_count);
                    }                    
                    temp.clear();
                }
                if (counter == 127) {
                    output.push_back(static_cast<char>(counter));
                    output.push_back(last_symbol);
                    counter = 0;
                }
            } else {
                if (counter > 2) { // save multi symbols sequence
                    output.push_back(static_cast<char>(counter));
                    output.push_back(last_symbol);
                }

                temp.push_back(*it);
                if (temp.size() >= 128) {
                    int single_order_symbols_count = temp.size();
                    output.push_back(static_cast<char>(-single_order_symbols_count));
                    output.insert(output.end(), temp.begin(), temp.begin() + single_order_symbols_count);
                    temp.clear();
                }
                counter = 1;
            }
            last_symbol = *it;
        }
        if (counter > 2) { // save multi symbols sequence
            output.push_back(static_cast<char>(counter));
            output.push_back(last_symbol);
        } else {
            char single_order_symbols_count = temp.size();
            output.push_back(-single_order_symbols_count);
            output.insert(output.end(), temp.begin(), temp.begin() + single_order_symbols_count);
        }

        return output;
    }

    vector<char> decompress(const vector<char>& input) {
        vector<char> output;

        for (auto it = input.begin(); it != input.end(); ++it) {
            int count = static_cast<int>(*it++);
            char symbol = *it;

            if (count > 0) {
                for (int i = 0; i < count; ++i) {
                    output.emplace_back(symbol);
                }
            } else {
                for (int i = 0; i < -count; ++i) {
                    output.emplace_back(*it++);
                }
                --it;
            }
        }

        return output;
    }

    void dump_compressed_data(const vector<char>& input) {
        for (auto it = input.begin(); it != input.end(); ++it) {
            int count = static_cast<int>(*it++);
            char symbol = *it;

            if (count > 0) {
                cout << "symbol " << symbol << " : " << count << " times" << endl;
            } else {
                if (count == 0) {
                    return;
                }
                for (int i = 0; i < -count; ++i) {
                    cout << "symbol " << *it++ << " : 1 times" << endl;
                }
                --it;
            }
        }
    }
};

void test(const string& str) {
    vector<char> input;

    copy(str.c_str(), str.c_str()+str.length(), back_inserter(input));
    
    auto compress_data = RLE{}.compress(input);
    RLE{}.dump_compressed_data(compress_data);

    auto decompmressed_data = RLE{}.decompress(compress_data);

    if (decompmressed_data == input) {
        cout << "all right!" << endl;
    } else {
        cout << "decompmressed_data != input" << endl;
    }
}


std::vector<char> read_file(const string& file_name) {
   std::ifstream file(file_name);
   std::ostringstream ss;
   ss << file.rdbuf();
   const std::string& s = ss.str();
   
   return std::vector<char>{s.begin(), s.end()};
}

void write_file(const string& file_name, const std::vector<char>& data) {
   std::ofstream file(file_name, std::ios::out|std::ios::binary);

    copy(data.cbegin(), data.cend(),
        ostreambuf_iterator<char>(file));

    file.close();
}

void dump_usage() {
    cout << "Usage: rle mode input_file_name output_file_name" << endl;
    cout << "modes:" << endl;
    cout << "  -c       compress" << endl;
    cout << "  -d       decompress" << endl;
}

int main(int argc, char** argv) {
    // test("aaaaabccddddeee");

    if (argc < 4) {
        dump_usage();
        return 0;
    }

    auto input = read_file(argv[2]);
    if (input.empty()) {
        cout << "input file is empty" << endl;
        return -1;
    }
    
    vector<char> output;
    if (string{argv[1]} == "-c") {
        output = RLE{}.compress(input);
    } else if (string{argv[1]} == "-d") {
        output = RLE{}.decompress(input);
    } else {
        cout << "bad mode: \"" << argv[1] << "\"" << endl;
        return -2;
    }
    
    cout << "input.size() = " << input.size() << endl;
    cout << "output.size() = " << output.size() << endl;

    write_file(argv[3], output);

    return 0;
}
