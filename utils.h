#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <string>

class BitWriter {
public:
    explicit BitWriter(std::FILE* out);
    ~BitWriter();
    void writeBit(bool bit);
    void writeBits(const std::string& bits); // bits string "0101..."
    void flush();

private:
    std::FILE* out_;
    unsigned char buffer_;
    int count_; // number of bits currently in buffer (0..7)
};

class BitReader {
public:
    explicit BitReader(std::FILE* in);
    ~BitReader();
    // returns -1 on EOF
    int readBit();

private:
    std::FILE* in_;
    unsigned char buffer_;
    int count_; // remaining bits in buffer (0..7)
};

bool readFileToString(const std::string& path, std::string& out);
bool writeStringToFile(const std::string& path, const std::string& data);

#endif // UTILS_H
