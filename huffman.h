#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <string>
#include <cstdio>

using ull = unsigned long long;

struct HuffmanNode {
    unsigned char ch;
    ull freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char c, ull f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {}

    HuffmanNode(HuffmanNode* l, HuffmanNode* r)
        : ch(0), freq(l->freq + r->freq), left(l), right(r) {}

    bool isLeaf() const {
        return !left && !right;
    }
};

class Huffman {
private:
    std::unordered_map<unsigned char, std::string> codes_;

public:
    Huffman() = default;

    void generateCodes(HuffmanNode* node, const std::string& prefix);
    HuffmanNode* buildTreeFromFreq(const std::unordered_map<unsigned char, ull>& freq);
    void writeTree(FILE* out, HuffmanNode* node);
    HuffmanNode* readTree(FILE* in);
    bool compress(const std::string& inputPath, const std::string& outputPath);
    bool decompress(const std::string& inputPath, const std::string& outputPath);
    void printCodes() const;
};

#endif

