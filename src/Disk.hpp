#include <iostream>
#include <fstream>
#include <algorithm>
#include "Entry.hpp"


#pragma once


class Disk {
public:
    Disk();
    Disk(const std::string &path);

    void format();
    void addFile(const std::string& filePath);
    void getFile(const std::string& fileName, const std::string& newFilePath);
    friend std::ostream& operator <<(std::ostream& ostream, const Disk& disk);
private:
    static constexpr uint64_t TableSize = 2048;

    std::string path;

    [[nodiscard]] std::array<Entry, Disk::TableSize> loadTable() const;
    void writeTable(std::array<Entry, Disk::TableSize> table);
    static std::string extractName(const std::string &path);
};