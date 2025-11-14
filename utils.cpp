#include "utils.h"
#include <fstream>
using namespace std;

BitWriter::BitWriter(FILE* out) : out_(out), buffer_(0), count_(0) {}
BitWriter::~BitWriter() { flush(); }

void BitWriter::writeBit(bool bit) {
    buffer_ = (buffer_ << 1) | (bit ? 1 : 0);
    if (++count_ == 8) {
        fputc(buffer_, out_);
        buffer_ = 0;
        count_ = 0;
    }
}

void BitWriter::writeBits(const string& bits) {
    for (char c : bits)
        writeBit(c == '1');
}

void BitWriter::flush() {
    if (count_ > 0) {
        buffer_ <<= (8 - count_);
        fputc(buffer_, out_);
        buffer_ = 0;
        count_ = 0;
    }
}

BitReader::BitReader(FILE* in) : in_(in), buffer_(0), count_(0) {}
BitReader::~BitReader() {}

int BitReader::readBit() {
    if (count_ == 0) {
        int c = fgetc(in_);
        if (c == EOF) return -1;
        buffer_ = c;
        count_ = 8;
    }
    int bit = (buffer_ >> 7) & 1;
    buffer_ <<= 1;
    --count_;
    return bit;
}

bool readFileToString(const string& path, string& out) {
    ifstream in(path, ios::binary);
    if (!in) return false;
    out.assign((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    return true;
}

bool writeStringToFile(const string& path, const string& data) {
    ofstream out(path, ios::binary);
    if (!out) return false;
    out.write(data.data(), data.size());
    return true;
}
