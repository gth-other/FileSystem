#include <array>
#include <string>
#include <cstdint>


#pragma once


class Entry {
public:
    static constexpr uint64_t FilenameSize = 256;
    static constexpr uint64_t NumbersSize = 8;
    static constexpr uint64_t Size = Entry::FilenameSize + 2 * Entry::NumbersSize;

    Entry();
    Entry(const std::string &formattedFilename, uint64_t start, uint64_t end);
    Entry(std::array<char, Entry::Size> buff);

    [[nodiscard]] std::array<char, Entry::FilenameSize> getFilename() const;
    [[nodiscard]] std::string getFormattedFilename() const;
    [[nodiscard]] uint64_t getStart() const;
    [[nodiscard]] uint64_t getEnd() const;
    [[nodiscard]] std::array<char, Entry::Size> getBuff() const;
private:
    std::array<char, Entry::FilenameSize> filename{};
    uint64_t start;
    uint64_t end;
};