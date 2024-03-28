#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <functional>
#include <limits>
#include <optional>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace std::chrono;

class SubstringSearch {
public:
    SubstringSearch() = default;

    int full_scan(const string& text, const string& mask) {
        int text_index = 0;
        while (text_index <= (text.length() - mask.length())) {
            int mask_index = 0;
            while (mask_index <= mask.length() && 
                   text[text_index + mask_index] == mask[mask_index]) {
                mask_index++;
            }
            if (mask_index == mask.length() + 1) {
                return text_index;
            }
            text_index++;
        }
        return -1;
    }

    int BMH(const string& text, const string& mask) {
        fill_BMH_shift(mask);

        int text_index = 0;
        while (text_index <= (text.length() - mask.length())) {
            int m = mask.length() - 1;
            while (m >= 0 && 
                   text[text_index + m] == mask[m]) {
                m--;
            }
            if (m < 0) {
                return text_index;
            }
            text_index += BMH_shift_[text[text_index + mask.length() - 1]];
        }
        return -1;
    }

    int BM(const string& text, const string& mask) {
        fill_BM_shift(mask);
        // dump_BM_shift();

        int n = text.length();
        int m = mask.length();

        for (int i = 0, j = 0; i <= n - m && j >= 0; i += BM_shift_[j+1]) {
            for (j = m - 1; j >= 0 && mask[j] == text[i + j]; j--);
            if (j < 0) {
                return i;
            }
        }

        return -1;
    }

private:
    void fill_BMH_shift(const string& mask) {
        for (size_t i = 0; i < 256; ++i) {
            BMH_shift_[i] = mask.length();
        }
        for (size_t i = 0; i < mask.length() - 1; ++i) {
            BMH_shift_[mask[i]] = mask.length() - i - 1;
        }
    }
    size_t BMH_shift_[256];

    void fill_BM_shift(const string& mask) {
        int n = mask.length();

        BM_shift_.resize(n + 1, n);
        std::vector<int> z(n, 0);

        int l = 0, r = 0;
        for (int j = 1; j < n; ++j) {
            if (j <= r) {
                z[j] = min(r - j + 1, z[j - l]);
            } 
            while (j + z[j] < n && mask[n - 1 - z[j]] == mask[n - 1 - (j + z[j])]) {
                z[j]++;
            }
            if (j + z[j] - 1 > r) {
                l = j;
                r = j + z[j] - 1;
            }
        }

        for (int j = n - 1; j > 0; j--) {
            BM_shift_[n - z[j]] = j;
        }
        
        for (int j = 1, r = 0; j <= n - 1; j++) {
            if (j + z[j] == n) {
                for (; r <= j; r++) {
                    if (BM_shift_[r] == n) {
                        BM_shift_[r] = j;
                    }
                }
            }
        }
    }
    void dump_BM_shift() {
        cout << "BM_shift:" << endl;
        for (int shift : BM_shift_) {
            cout << shift << " ";
        }
        cout << endl;
    }

    std::vector<int> BM_shift_;
};


void test(const string& text, const string& mask, size_t count) {
    int result = SubstringSearch{}.BMH(text, mask);
    cout << "'" << mask << "'" << " starts from symbol " << result << " in " << "'" << text << "'" << endl;

    auto start = high_resolution_clock::now();

    for (size_t j = 0; j < count; j++) {
        SubstringSearch{}.BMH(text, mask);
    }

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	std::cout << count << " cycles works for " << duration.count() << " ms" << std::endl;
}

int main() {

    // test("data structures and algorithms", "rithm", 10000000);
    test("abeccaabadbabbaabeccaabadbabbaabeccaabadbabbaabeccaabadbabbaabeccaabadbabbaabeccaabadbabbad", "abadbabbad", 10000000);

    return 0;
}
