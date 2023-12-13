#pragma once

#include <string>
#include <functional>

class Tester {
public:
	using RunTest = std::function<bool(std::string, std::string)>;

	Tester(int argc, char** argv, RunTest run_test);

	void run();

private:
	std::string in_file(int i) const;
	std::string out_file(int i) const;

	RunTest run_test_;
	std::string test_dir_path_;
};
