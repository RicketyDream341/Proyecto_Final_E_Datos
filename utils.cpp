#include "utils.h"
#include <fstream>
#include <iterator>

// ============================================================
//  BitWriter — MSB-first (confirmado por prueba 0xAA)
// ============================================================
BitWriter::BitWriter(std::FILE* out) : out_(out), buffer_(0), count_(0) {}
BitWriter::~BitWriter() { flush(); }

void BitWriter::writeBit(bool bit) {
    buffer_ = static_cast<unsigned char>((buffer_ << 1) | (bit ? 1 : 0));
    ++count_;
    if (count_ == 8) {
        std::fputc(buffer_, out_);
        buffer_ = 0;
        count_ = 0;
    }
}

void BitWriter::writeBits(const std::string& bits) {
    for (char c : bits)
        writeBit(c == '1');
}

void BitWriter::flush() {
    if (count_ > 0) {
        buffer_ <<= (8 - count_);
        std::fputc(buffer_, out_);
        buffer_ = 0;
        count_ = 0;
    }
}

// ============================================================
//  BitReader — MSB-first (debe coincidir con el escritor)
// ============================================================
BitReader::BitReader(std::FILE* in) : in_(in), buffer_(0), count_(0) {}
BitReader::~BitReader() {}

int BitReader::readBit() {
    if (count_ == 0) {
        int c = std::fgetc(in_);
        if (c == EOF) return -1;
        buffer_ = static_cast<unsigned char>(c);
        count_ = 8;
    }
    int bit = (buffer_ >> 7) & 1;  // toma el bit más significativo
    buffer_ <<= 1;
    --count_;
    return bit;
}

// ============================================================
//  Lectura y escritura de archivos
// ============================================================
bool readFileToString(const std::string& path, std::string& out) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    out.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return true;
}

bool writeStringToFile(const std::string& path, const std::string& data) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    return true;
}
