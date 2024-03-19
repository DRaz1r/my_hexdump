#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cctype>
#include <iomanip>
#include <vector>
#include <cxxopts.hpp>

void hexdump(std::istream &is, std::size_t width = 16) {
    std::size_t addr = 0;
    std::vector<unsigned char> buffer(width);
    while (is.read(reinterpret_cast<char*>(buffer.data()), width) || is.gcount() > 0) {
        std::cout << std::setw(8) << std::setfill('0') << std::hex << addr << ' ';
        std::size_t count = is.gcount();
        for (std::size_t i = 0; i < count; ++i) {
            std::cout << ' ' << std::right << std::hex << std::setw(2) << std::setfill('0');
            std::cout << static_cast<unsigned int>(buffer[i]);
            ++addr;
        }
        if (count < width) {
            for (std::size_t i = 0; i < (width - count) * 3; i++) {
                std::cout << ' ';
            }
        }
        std::cout << "  |";
        for (std::size_t i = 0; i < count; ++i) {
            if (!std::isprint(buffer[i])) {
                buffer[i] = '.';
            }
            std::cout << buffer[i];
        }
        std::cout << "|\n";
    }
}

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
    std::istream *inputStream = &std::cin;
    std::ifstream ifs;
    if (path != "-") {
        ifs.open(path);
        if (!ifs.good()) {
            std::cerr << std::strerror(errno) << ": " << path << '\n';
            return errno;
        }
        inputStream = &ifs;
    }
    hexdump(*inputStream, width);
    return 0;
}
