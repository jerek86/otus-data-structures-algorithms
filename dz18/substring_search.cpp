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

    int finite_state_machine_search(const string& text, const string& mask) {
        init_FSM_delta(mask);
        int q = 0;
        for (int i = 0; i < text.length(); ++i) {
            q = FSM_delta_[FSM_delta_index(q, text[i])];
            if (q == mask.length()) {
                return i - mask.length() + 1;
            }
        }

        return -1;
    }

    int KMP(const string& text, const string& mask) {
        string total = mask + "#" + text;
        std::vector<int> pi(total.length() + 1);
        pi[0] = 0;
        pi[1] = 0;
        for (int i = 1; i < total.length(); ++i) {
            int len = pi[i];

            while (len > 0 && total[len] != total[i]) {
                len = pi[len];
            }
            if (total[len] == total[i]) {
                ++len;
            }
            pi[i + 1] = len;
            
            if (len == mask.length()) {
                return i - 2*mask.length();
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


    enum { ALPHABET_SIZE = 256 };
    int FSM_delta_index(int mask_index, int symbol) {
        return (mask_index * ALPHABET_SIZE) + symbol;
    }
    string left(const string& str, int k) {
        return str.substr(0, k);
    }
    string right(const string& str, int k) {
        return str.substr(str.length() - k);
    }
    void init_FSM_delta(const string& mask) {
        FSM_delta_.resize(FSM_delta_index(mask.length(), 0), 0);
        for (int q = 0; q < mask.length(); ++q) {
            for (int i = 0; i < ALPHABET_SIZE; ++i) {
                char c = static_cast<char>(i);
                int k = q + 1;
                if (q == mask.length()) {
                    --k;
                }
                string line = left(mask, q) + c;
                while (left(mask, k) != right(line, k) && k >= 0) {
                    --k;
                }
                FSM_delta_[FSM_delta_index(q, i)] = k;
            }
        }
    }
    
    std::vector<int> FSM_delta_;


    void init_KMP_pi(const string& mask) {
        KMP_pi_.resize(mask.length() + 1);
        KMP_pi_[0] = 0;
        KMP_pi_[1] = 0;
        for (int i = 1; i < mask.length(); ++i) {
            int len = KMP_pi_[i];
            while (len > 0 && mask[len] != mask[i]) {
                len = KMP_pi_[len];
            }
            if (len > 0) {
                ++len;
            }
            KMP_pi_[i + 1] = len;
        }
    }
    
    std::vector<int> KMP_pi_;
};


void test(const string& text, const string& mask, size_t count) {
    int result = SubstringSearch{}.KMP(text, mask);
    cout << "'" << mask << "'" << " starts from symbol " << result << " in " << "'" << text << "'" << endl;

    auto start = high_resolution_clock::now();

    for (size_t j = 0; j < count; j++) {
        SubstringSearch{}.KMP(text, mask);
    }

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	std::cout << count << " cycles works for " << duration.count() << " ms" << std::endl;
}

int main() {

    // test("data structures and algorithms", "rithm", 10000000);
    test("abeccaabadbabbad", "abbad", 10000000);
    // test("abeccaabadbabbaabeccaabadbabbaabeccaabadbabbaabeccaabadbabbaabeccaabadbabbaabeccaabadbabbad", "abadbabbad", 10000000);

    return 0;
}
