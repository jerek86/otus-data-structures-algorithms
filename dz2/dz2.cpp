#include <iostream>
#include <vector>
#include <string>

#include "tester.hpp"

class LuckyTicketCounterNestedLoops {
public:
	LuckyTicketCounterNestedLoops() = default;
	
	long count_all_tickets() {
		long count = 0;

		for (int i = 0; i<10; ++i) {
			for (int j = 0; j<10; ++j) {
				for (int k = 0; k<10; ++k) {
					for (int a = 0; a<10; ++a) {
						for (int b = 0; b<10; ++b) {
							for (int c = 0; c<10; ++c) {
								if (i + j + k == a + b + c) {
									++count;
								}
							}
						}
					}
				}
			}
		}

		return count;
	}
};

class LuckyTicketCounterRecursive {
public:
	LuckyTicketCounterRecursive() = default;
	
	long count_all_tickets(int N) {
		return next(N, 0, 0);
	}

	long next(int n, long sum_a, long sum_b) {
		long count = 0;
		
		if (n == 0) {
			if (sum_a == sum_b) {
				return 1;
			}
			return 0;
		}

		for (int a = 0; a < 10; ++a) {
			for (int b = 0; b < 10; ++b) {
				count += next(n - 1, sum_a + a, sum_b + b);
			}
		}

		return count;
	}
};

class LuckyTicketCounterDP {
public:
	LuckyTicketCounterDP() = default;
	
	long count_all_tickets(int N) {
		std::vector<long> summs(10, 1);

		for (int i = 1; i < N; ++i) {
			summs = get_next(summs);
		}

		return count_summs(summs);
	}

	std::vector<long> get_next(std::vector<long> prev) {
		std::vector<long> summs(prev.size() + 9, 0);
		
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < prev.size(); ++j) {
				summs.at(i + j) += prev[j];
			}
		}

		return summs;
	}

	long count_summs(std::vector<long> summs) {
		long count = 0;

		for (long summ : summs) {
			count += summ*summ;
		}

		return count;
	}

	std::vector<long> summs_;
};

bool test(std::string in, std::string out) {
	long in_num = std::stoi(in);
	long out_num = std::stol(out);

	return LuckyTicketCounterDP{}.count_all_tickets(in_num) == out_num;
}


int main(int argc, char** argv)
{
	Tester{argc, argv, test}.run();
}
