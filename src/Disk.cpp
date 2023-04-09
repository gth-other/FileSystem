#include "Disk.hpp"


Disk::Disk() = default;
Disk::Disk(const std::string &path) {
    this->path = path;
}
void Disk::format() {
    std::array<Entry, Disk::TableSize> table{};
    this->writeTable(table);
}
void Disk::addFile(const std::string &filePath) {
    std::array<Entry, Disk::TableSize> table = this->loadTable();

    uint64_t tableIndex = -1;
    for (uint64_t i = 0; i < Disk::TableSize; i = i + 1) {
        if (table[i].getFormattedFilename() == Disk::extractName(filePath)) {
            throw "Накопитель уже содержит файл с введеным именем.";
        }
        if (table[i].getStart() == 0) {
            tableIndex = i;
            break;
        }
    }
    if (tableIndex == -1) {
        throw "Таблица файловой системы закончилась.";
    }

    uint64_t start;
    if (tableIndex == 0) {
        start = Disk::TableSize * Entry::Size;
    }
    else {
        start = table[tableIndex - 1].getEnd() + 1;
    }

    std::ifstream file(filePath);
    std::ofstream disk(this->path);
    if (!file.is_open()) {
        throw "Не удалось открыть файл.";
    }
    if (!disk.is_open()) {
        throw "Не удалось открыть накопитель.";
    }

    disk.seekp((int64_t)start);
    char buff;
    uint64_t size = 0;
    while (file.get(buff)) {
        if (!disk.put(buff)) {
            throw "Место на накопителе закончилось.";
        }
        size = size + 1;
    }
    table[tableIndex] = Entry(Disk::extractName(filePath), start, start + size);

    this->writeTable(table);
}
void Disk::getFile(const std::string &fileName, const std::string &newFilePath) {
    std::array<Entry, Disk::TableSize> table = this->loadTable();

    uint64_t tableIndex = -1;
    for (uint64_t i = 0; i < Disk::TableSize; i = i + 1) {
        if (table[i].getFormattedFilename() == fileName) {
            tableIndex = i;
            break;
        }
    }
    if (tableIndex == -1) {
        throw "Накопитель не содержит файла с введенным именем.";
    }

    std::ofstream file(newFilePath);
    std::ifstream data(this->path);
    if (!file.is_open()) {
        throw "Не удалось открыть файл.";
    }
    if (!file.is_open()) {
        throw "Не удалось открыть накопитель.";
    }

    data.seekg((int64_t)table[tableIndex].getStart());
    for (uint64_t i = 0; i < table[tableIndex].getEnd() - table[tableIndex].getStart() + 1; i = i + 1) {
        file.put((char)data.get());
    }
}
std::ostream &operator<<(std::ostream &ostream, const Disk &disk) {
    std::array<Entry, Disk::TableSize> table = disk.loadTable();

    ostream << "\e[1;32m";
    for (uint64_t i = 0; i < 64; i = i + 1) {
        ostream << '#';
    }
    ostream << '\n';

    for (uint64_t i = 0; i < Disk::TableSize; i = i + 1) {
        std::string filename = table[i].getFormattedFilename();
        if (filename.empty()) continue;

        uint64_t start = table[i].getStart();
        uint64_t end = table[i].getEnd();
        ostream << filename << " (" << (float)(end - start + 1) / 1024.f / 1024.f << " MB)" << '\n';
    }

    for (uint64_t i = 0; i < 64; i = i + 1) {
        ostream << '#';
    }
    ostream << "\e[0m";

    return ostream;
}
std::array<Entry, Disk::TableSize> Disk::loadTable() const {
    std::ifstream data(this->path);
    if (!data.is_open()) {
        throw "Не удалось открыть накопитель.";
    }

    std::array<Entry, Disk::TableSize> table{};

    std::array<char, Entry::Size> buff{};
    for (uint64_t i = 0; i < Disk::TableSize; i = i + 1) {
        data.seekg((int64_t)(i * Entry::Size));
        for (uint64_t j = 0; j < Entry::Size; j = j + 1) {
            buff[j] = (char)data.get();
        }
        table[i] = Entry(buff);
    }

    return table;
}
void Disk::writeTable(std::array<Entry, Disk::TableSize> table) {
    std::ofstream data(this->path);
    if (!data.is_open()) {
        throw "Не удалось открыть накопитель.";
    }

    std::array<char, Entry::Size> buff{};
    for (uint64_t i = 0; i < Disk::TableSize; i = i + 1) {
        buff = table[i].getBuff();
        data.seekp((int64_t)(i * Entry::Size));
        for (uint64_t j = 0; j < Entry::Size; j = j + 1) {
            data.put(buff[j]);
        }
    }
}
std::string Disk::extractName(const std::string &path) {
    for (uint64_t i = path.size() - 1; true; i = i - 1) {
        if (i == 0) {
            return path;
        }
        if (path[i] == '/') {
            return path.substr(i + 1);
        }
    }
}