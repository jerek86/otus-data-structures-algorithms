#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <array>

#include "tester.hpp"

using namespace std::chrono;

class PrimeSearch {
public:
	PrimeSearch() = default;
	
	long get_count(long max_number) {
        long count = 0;
        
        for (long i = 0; i <= max_number; ++i) {
            if (is_prime(i)) {
                ++count;
            }
        }

        return count;
	}

private:
    bool is_even(long number) {
        return (number & 1) == 0;
    }

    bool is_prime(long number) {
        if (number < 2) {
            return false;
        }
        if (number == 2) {
            return true;
        }
        if (is_even(number)) {
            return false;
        }

        long MAX = std::sqrt(number);
        for (long i = 3; i <= MAX; ++i) {
            if (number % i == 0) {
                return false;
            }
        }

        return true;
    }

};

class PrimeSearchDevideOnPrimes {
public:
	PrimeSearchDevideOnPrimes() = default;
	
	long get_count(long max_number) {
        prime_numbers_.clear();
        
        for (long i = 0; i <= max_number; ++i) {
            if (is_prime(i)) {
                prime_numbers_.push_back(i);
            }
        }

        return prime_numbers_.size();
	}

private:
    bool is_prime(long number) {
        if (number < 2) {
            return false;
        }

        long MAX = std::sqrt(number);
        for (long prime : prime_numbers_) {
            if (prime > MAX) {
                break;
            }

            if (number % prime == 0) {
                return false;
            }   
        }

        return true;
    }

    std::vector<long> prime_numbers_;
};

class PrimeEratos {
public:
	PrimeEratos() = default;
	
	long get_count(long max_number) {
        std::vector<bool> sieve(max_number + 1, true);
        long count = 0;

        for (long i = 2; i <= max_number; ++i) {
            if (sieve[i]) {
                for (long j = 2 * i; j <= max_number; j += i) {
                    sieve[j] = false;
                }
                ++count;
            }
        }
        return count;
	}
};

class PrimeEratosLinear {
public:
	PrimeEratosLinear() = default;
	
	long get_count(long max_number) {
        std::vector<long> sieve(max_number + 1, 0);
        std::vector<long> primes;

        for (long i = 2; i <= max_number; ++i) {
            if (sieve[i] == 0) {
                sieve[i] = i;
                primes.emplace_back(i);
            }

            for (auto p : primes) {
                if ((p > sieve[i]) || (p * i > max_number)) {
                    break;
                }
                sieve[p * i] = p;
            }
        }
        return primes.size();
	}
};


bool test(std::string in, std::string out) {
    long in_num = std::stol(in);
	long long out_num = std::stoll(out);

	long long calculation_result = PrimeEratosLinear{}.get_count(in_num);
	std::cout << "in: " << in_num << ", result: " << calculation_result << ", expected value: " << out_num << std::endl;

	return calculation_result == out_num;
}

int main(int argc, char** argv)
{
	Tester{argc, argv, test}.run();
}
