#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdio>

class BitWriter {
private:
    std::FILE* out_;
    unsigned char buffer_;
    int count_;

public:
    BitWriter(std::FILE* out);
    ~BitWriter();
    void writeBit(bool bit);
    void writeBits(const std::string& bits);
    void flush();
};

class BitReader {
private:
    std::FILE* in_;
    unsigned char buffer_;
    int count_;

public:
    BitReader(std::FILE* in);
    ~BitReader();
    int readBit();
};

bool readFileToString(const std::string& path, std::string& out);
bool writeStringToFile(const std::string& path, const std::string& data);

#endif
