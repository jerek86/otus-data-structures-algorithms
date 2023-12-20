#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <array>

#include "tester.hpp"

using namespace std::chrono;

class FiboRecurcive {
public:
	FiboRecurcive() = default;
	
	long long get_value(long n) {
        if (n <= 1) {
            return n;
        }

        return get_value(n-1) + get_value(n-2);
	}
};

class FiboIterative {
public:
	FiboIterative() = default;
	
	long long get_value(long n) {
        if (n < 2) {
            return n;
        }
        
        long long V_n_2 = 0;
        long long V_n_1 = 1;
        
        long long V_n;

        for (int i = 2; i <= n; ++i) {
            V_n = V_n_1 + V_n_2;

            V_n_2 = V_n_1;
            V_n_1 = V_n;
        }

        return V_n;
	}
};


class FiboBine {
public:
	FiboBine() = default;
	
	long long get_value(long n) {
        const double SQRT_5 = std::sqrt(5.0);

        long double first = get_power((1 + SQRT_5)*0.5, n);
        long double second = get_power((1 - SQRT_5)*0.5, n);

        long double result_float_point = (first - second) / SQRT_5;

        return static_cast<long long>(result_float_point + 0.5);
	}

private:
	double get_power(long double number, long power) {
		double result = 1.;

        while (power) {
            result *= number;
            --power;
        }

        return result;
	}
};

class FiboMatrix {
public:
	FiboMatrix() = default;
	
	long long get_value(long n) {
        Matrix2D result{1, 0, 1, 0};
        Matrix2D base{1, 1, 1, 0};

        while (n) {
            if (is_odd(n)) {
                result = result.multipty(base);
            }
            base = base.multipty(base);
            n /= 2;
        }

        return result.a22();
	}

private:
    class Matrix2D {
    public:
        Matrix2D() = delete;
        Matrix2D(long long a11, long long a12, long long a21, long long a22)
         : values_({a11, a12, a21, a22}) {}

        long long a11() const {return values_[0];}
        long long a12() const {return values_[1];}
        long long a21() const {return values_[2];}
        long long a22() const {return values_[3];}

        Matrix2D multipty(const Matrix2D& b) {
            return Matrix2D{a11()*b.a11() + a12()*b.a21(), a11()*b.a12() + a12()*b.a22(),
                            a21()*b.a11() + a22()*b.a21(), a21()*b.a12() + a22()*b.a22()};
        }

    private:
        std::array<long long, 4> values_;
    };

    bool is_odd(long number) {
        return number & 1 == 1;
    }
};



bool test(std::string in, std::string out) {
    long in_num = std::stol(in);
	long long out_num = std::stoll(out);

	long long calculation_result = FiboIterative{}.get_value(in_num);
	std::cout << "in: " << in_num << ", result: " << calculation_result << ", expected value: " << out_num << std::endl;

	return calculation_result == out_num;
}

int main(int argc, char** argv)
{
    long long prev = 0;
    for (int i = 20; i < 1000; ++i) {
        auto start = high_resolution_clock::now();
        auto res = FiboMatrix{}.get_value(i);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);

        if (res < prev) {
            break;
        }

        std::cout << "n: " << i << ", result: " << res << " time: " << duration.count() << " ms" << std::endl;
        prev = res;
    }

	Tester{argc, argv, test}.run();
}
