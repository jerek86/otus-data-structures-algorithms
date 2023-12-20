#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include "tester.hpp"

class PowerLinear {
public:
	PowerLinear() = default;
	
	double get_power(double number, long power) {
		double result = 1.;

        while (power) {
            result *= number;
            --power;
        }

        return result;
	}
};

class PowerLogLinear {
public:
	PowerLogLinear() = default;
	
	double get_power(long double number, long power) {
		if (power == 0) {
            return 1.;
        }
        
        long double result = number;
        long temp_count = 1;

        while (temp_count*2 < power) {
            result *= result;
            temp_count *= 2;
        }

        for (long i = temp_count; i < power; ++i) {
            result *= number;
        }

        return result;
	}
};

class PowerLog {
public:
	PowerLog() = default;
	
	double get_power(long double number, long power) {
		long double result = 1.;

        while (power) {
            if (is_odd(power)) {
                result *= number;
            }
            number *= number;
            power /= 2;
        }

        return result;
	}

    bool is_odd(long number) {
        return number & 1 == 1;
    }
};

bool compare(double v1, double v2, int precision) {
    double eps = 1. / PowerLinear{}.get_power(10, precision);
    return std::abs(v1 - v2) < eps;
}

bool test(std::string in, std::string out) {
    auto in_vec = string_split(in, "\r\n");
    if (in_vec.size() < 2) {
        std::cout << "unexpected input: " << in << std::endl;
        return false;
    }

    auto out_vec = string_split(out, ".");
    int precision = 0.;
    int numbers_to_show = out_vec[0].size();
    if (out_vec.size() > 1) {
        precision = out_vec[1].size();
        numbers_to_show = std::max (numbers_to_show, precision);
    }

    double number = std::stod(in_vec[0]);
    long power = std::stol(in_vec[1]);
    double out_num = std::stod(out);

    double calculation_result = PowerLog{}.get_power(number, power);
    std::cout << "number: " << in_vec[0] << ", power: " << power << ", result: " 
              << std::setprecision(numbers_to_show) << calculation_result << ", expected value: " << out_num << std::endl;

	return compare(calculation_result, out_num, precision);
}


int main(int argc, char** argv)
{
	Tester{argc, argv, test}.run();
}
