#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <string>

class BitWriter {
public:
    explicit BitWriter(std::FILE* out);
    ~BitWriter();
    void writeBit(bool bit);
    void writeBits(const std::string& bits);
    void flush();

private:
    std::FILE* out_;
    unsigned char buffer_;
    int count_;
};

class BitReader {
public:
    explicit BitReader(std::FILE* in);
    ~BitReader();
    int readBit();

private:
    std::FILE* in_;
    unsigned char buffer_;
    int count_;
};

bool readFileToString(const std::string& path, std::string& out);
bool writeStringToFile(const std::string& path, const std::string& data);

#endif
