#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

// color codes
#if !defined(_WIN32) || !defined(__CYGWIN__)
#define YEL "\e[0;33m"
#define GRN "\e[0;32m"
#define BGRN "\e[1;32m"
#define UGRN "\e[4;32m"
#define COLOR_RESET "\e[0m"
#else // _WIN32
#define YEL ""
#define GRN ""
#define BGRN ""
#define UGRN ""
#define COLOR_RESET ""
#endif // _WIN32

using std::vector;
using byte_t = unsigned char;
using u32_t = unsigned int;

/*
   This function takes two arguments: "base" is the base
   of the logarithm, and "x" is the number whose logarithm
   is being calculated.
*/
double logarithm(double base, double x)
{
    return std::log(x) / std::log(base);
}

// calculate Shannon's entropy: formula =>
// ENT = -Σ p(i) * log2(p(i)),
// where p(i) = probability(i) = i / length_of_data and
// 'i' is a single "byte_of_data"
double calc_entropy(const vector<byte_t> &data)
{
    // Create a counter for all the 256 different
    // possible byte values
    u32_t possible_bytes[256]{};

    // Increment the counter if the byte has the same
    // value as one of the keys
    for (auto &byte_idx : data)
        possible_bytes[byte_idx] += 1;

    // compute entropy
    double entropy{};
    size_t data_len{data.size()};
    for (auto &count : possible_bytes)
    {
        if (!count)
            continue;

        double prob{static_cast<double>(count) /
                    static_cast<double>(data_len)};
        entropy -= prob * logarithm(2, prob);
    }

    return entropy;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cerr << YEL
                  << "[-] Please provide a file whose "
                     "entropy is to be calculated.\n\n"
                  << "Usage: " << argv[0]
                  << " <file_whose_entropy_is_to_be_"
                     "calculated>"
                  << COLOR_RESET;

        return 1;
    }

    // open file whose entropy is to be got
    std::ifstream in_file(argv[1],
                          std::ios::in | std::ios::binary);
    if (!in_file.is_open())
    {
        std::cerr << YEL
                  << "[-] failed to open file for "
                     "some reason.\n"
                  << COLOR_RESET;
        return 1;
    }

    // get length of file
    in_file.seekg(0, in_file.end);
    size_t length = in_file.tellg();
    in_file.seekg(0, in_file.beg);

    // store file bytes into buffer
    std::vector<byte_t> buf(length, 0x0);
    in_file.read((char *)buf.data(), buf.size());

    // get entropy
    double ent{calc_entropy(buf)};
    std::cout << GRN << "+ File entropy is " << BGRN << UGRN
              << ent << "\n"
              << COLOR_RESET;

    in_file.close();
    return 0;
}
