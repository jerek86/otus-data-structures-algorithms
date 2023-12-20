#include "tester.hpp"

#include <iostream>
#include <fstream>
#include <chrono>

using namespace std::chrono;

Tester::Tester(int argc, char** argv, RunTest run_test)
 : run_test_(run_test) {
	if (argc < 2) {
		std::cout << "test dir path not specifed" << std::endl;
		exit(1);
	}

	test_dir_path_ = argv[1];
}

std::string Tester::in_file(int i) const {
	return test_dir_path_ + "test." + std::to_string(i) + ".in";
}
std::string Tester::out_file(int i) const {
	return test_dir_path_ + "test." + std::to_string(i) + ".out";
}

void Tester::run() {
	std::cout << "test dir path: " << test_dir_path_ << std::endl;

	int num = 0;
	while (1) {
		std::ifstream fin(in_file(num));
		std::ifstream fout(out_file(num));

		if (!fin.good() || !fout.good()) {
			break;
		}

		std::string in{std::istreambuf_iterator<char>(fin),
                 	   std::istreambuf_iterator<char>()};
		std::string out{std::istreambuf_iterator<char>(fout),
                 	   std::istreambuf_iterator<char>()};

		auto start = high_resolution_clock::now();
		std::string result = run_test_(in, out) ? "True" : "False";
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);

		std::cout << "test " << num << " result: " << result << ", time " << duration.count() << " ms" << std::endl;

		++num;
	}
}

std::vector<std::string> string_split(std::string string, std::string delimiter)
{
    std::vector<std::string> result;
    if (!string.empty()) {
        int start = 0;
        do {
            int idx = string.find(delimiter, start);
            if (idx == std::string::npos) {
                break;
            }
 
            int length = idx - start;
            result.push_back(string.substr(start, length));
            start += (length + delimiter.size());
        } while (true);
        result.push_back(string.substr(start));
    }
 
    return result;
}
