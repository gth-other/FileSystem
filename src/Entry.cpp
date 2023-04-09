#include "Entry.hpp"


Entry::Entry() {
    for (uint64_t i = 0; i < Entry::FilenameSize; i = i + 1) {
        this->filename[i] = 0;
    }
    this->start = 0;
    this->end = 0;
}
Entry::Entry(const std::string &formattedFilename, uint64_t start, uint64_t end) {
    if (formattedFilename.size() > Entry::FilenameSize) {
        throw "Длина введенного имени файла превышает установленный лимит.";
    }

    for (uint64_t i = 0; i < formattedFilename.size(); i = i + 1) {
        this->filename[i] = formattedFilename[i];
    }
    for (uint64_t i = formattedFilename.size(); i < Entry::FilenameSize; i = i + 1) {
        this->filename[i] = 0;
    }

    this->start = start;
    this->end = end;
}
Entry::Entry(std::array<char, Entry::Size> buff) {
    for (uint64_t i = 0; i < Entry::FilenameSize; i = i + 1) {
        this->filename[i] = buff[i];
    }

    this->start = 0;
    this->end = 0;
    for (uint64_t i = 0; i < Entry::NumbersSize; i = i + 1) {
        this->start = (this->start << Entry::NumbersSize) + (buff[Entry::FilenameSize + i] & 0xFF);
    }
    for (uint64_t i = 0; i < Entry::NumbersSize; i = i + 1) {
        this->end = (this->end << Entry::NumbersSize) + (buff[Entry::FilenameSize + Entry::NumbersSize + i] & 0xFF);
    }
}
std::array<char, Entry::FilenameSize> Entry::getFilename() const {
    return this->filename;
}
std::string Entry::getFormattedFilename() const {
    std::string formattedFilename;

    for (uint64_t i = 0; i < Entry::FilenameSize; i = i + 1) {
        if (this->filename[i] == 0) {
            break;
        }
        formattedFilename.push_back(this->filename[i]);
    }

    return formattedFilename;
}
uint64_t Entry::getStart() const {
    return this->start;
}
uint64_t Entry::getEnd() const {
    return this->end;
}
std::array<char, Entry::Size> Entry::getBuff() const {
    std::array<char, Entry::Size> buff{};

    for (uint64_t i = 0; i < Entry::FilenameSize; i = i + 1) {
        buff[i] = this->filename[i];
    }

    for (uint64_t i = 0; i < Entry::NumbersSize; i = i + 1) {
        buff[Entry::FilenameSize + i] = (char)((this->start >> Entry::NumbersSize * (7 - i)) & 0xFF);
    }
    for (uint64_t i = 0; i < Entry::NumbersSize; i = i + 1) {
        buff[Entry::FilenameSize + Entry::NumbersSize + i] = (char)((this->end >> Entry::NumbersSize * (7 - i)) & 0xFF);
    }

    return buff;
}