#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <cctype>
#include <iomanip>
// Range-V3 库中的一个视图，用于将范围划分为指定大小的块
#include <range/v3/view/chunk.hpp>
// 一个轻量级的C++命令行选项解析库
#include <cxxopts.hpp>
// 支持C++20范围库的功能
#include <ranges>

void hexdump(std::ranges::input_range auto const &range, std::size_t width = 16) {
    // 类型推导: 推导出范围内元素的类型
    using T = std::ranges::range_value_t<decltype(range)>;

    // 00096e90  6d 5f 64 65 76 69 63 65  20 72 64 3b 0a 20 20 20  |m_device rd;.   |
    std::size_t addr = 0;
    std::vector<char> saved;
    for (auto chunk: range | ranges::views::chunk(width)) {
        std::cout << std::setw(8) << std::setfill('0') << std::hex << addr << ' ';
        for (auto c: chunk) {
            std::cout << ' ' << std::right << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0');
            std::cout << (std::uint64_t)(std::make_unsigned_t<T>)c;
            ++addr;
            if constexpr (sizeof(T) == sizeof(char) && std::convertible_to<T, char>) {
                saved.push_back(static_cast<char>(c));
            }
        }
        // 使用if constexpr来判断元素类型，以确定是否可以将元素直接转换为可打印的字符
        if constexpr (sizeof(T) == sizeof(char) && std::convertible_to<T, char>) {
            if (addr % width != 0) {
                for (std::size_t i = 0; i < (width - addr % width) * 3; i++) {
                    std::cout << ' ';
                }
            }
            std::cout << "  |";
            for (auto c: saved) {
                if (!std::isprint(c)) {
                    c = '.';
                }
                std::cout << c;
            }
            std::cout << "|";
            saved.clear();
        }
        std::cout << '\n';
    }
}

struct IstreamRange {
    std::istreambuf_iterator<char> _begin, _end;

    IstreamRange(std::istream &is)
    : _begin(std::istreambuf_iterator<char>(is))
    , _end(std::istreambuf_iterator<char>()) {}

    auto begin() const {
        return _begin;
    }

    auto end() const {
        return _end;
    }
};

int main(int argc, char **argv) {
    cxxopts::Options options("hexdump", "A command line utility for printing the hexadecimal and ASCII representation of a file.");
    options.add_options()("f,file", "Input file", cxxopts::value<std::string>()->default_value("-"));
    options.add_options()("w,width", "Number of bytes per line", cxxopts::value<std::size_t>()->default_value("16"));
    // add help
    options.add_options()("h,help", "Print usage");
    auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }
    auto path = args["file"].as<std::string>();
    auto width = args["width"].as<std::size_t>();
    if (path == "-") {
        hexdump(IstreamRange(std::cin), width);
        return 0;
    }
    std::ifstream ifs(path);
    if (!ifs.good()) {
        std::cerr << std::strerror(errno) << ": " << path << '\n';
        return errno;
    }
    hexdump(IstreamRange(ifs), width);
    return 0;
}
