#include "Huffman.h"
#include "utils.h"
#include <queue>
#include <unordered_map>
#include <cstdio>

std::unordered_map<unsigned char, std::string> globalCodes;

using ull = unsigned long long;

struct NodeCmp {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) const {
        if (a->freq != b->freq)
            return a->freq > b->freq;
        return a->ch > b->ch;
    }
};

HuffmanNode* Huffman::buildTreeFromFreq(const std::unordered_map<unsigned char, ull>& freq) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, NodeCmp> pq;
    for (auto& p : freq) pq.push(new HuffmanNode(p.first, p.second));

    if (pq.empty()) return nullptr;
    if (pq.size() == 1) {
        HuffmanNode* only = pq.top(); pq.pop();
        HuffmanNode* dummy = new HuffmanNode((unsigned char)0, 0);
        return new HuffmanNode(only, dummy);
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        pq.push(new HuffmanNode(left, right));
    }

    return pq.top();
}

void Huffman::generateCodes(HuffmanNode* node, const std::string& prefix) {
    if (!node) return;
    if (node->isLeaf()) {
       codes_[node->ch] = prefix.empty() ? "0" : prefix;
        globalCodes[node->ch] = codes_[node->ch];

    } else {
        generateCodes(node->left, prefix + "0");
        generateCodes(node->right, prefix + "1");
    }
}

void Huffman::writeTree(FILE* out, HuffmanNode* node) {
    if (!node) return;
    if (node->isLeaf()) {
        fputc('1', out);
        fputc(node->ch, out);
    } else {
        fputc('0', out);
        writeTree(out, node->left);
        writeTree(out, node->right);
    }
}

HuffmanNode* Huffman::readTree(FILE* in) {
    int flag = fgetc(in);
    if (flag == EOF) return nullptr;
    if (flag == '1') {
        int c = fgetc(in);
        return new HuffmanNode((unsigned char)c, 0);
    } else {
        HuffmanNode* left = readTree(in);
        HuffmanNode* right = readTree(in);
        return new HuffmanNode(left, right);
    }
}

bool Huffman::compress(const std::string& inputPath, const std::string& outputPath) {
    std::string data;
    if (!readFileToString(inputPath, data)) {
        printf("Error: no se pudo leer el archivo de entrada.\n");
        return false;
    }

    std::unordered_map<unsigned char, ull> freq;
    for (unsigned char c : data) freq[c]++;

    HuffmanNode* root = buildTreeFromFreq(freq);
    generateCodes(root, "");

    FILE* out = fopen(outputPath.c_str(), "wb");
    if (!out) return false;

    writeTree(out, root);
    fputc('\n', out);

    unsigned long long total = data.size();
    fwrite(&total, sizeof(total), 1, out);

    BitWriter bw(out);
    for (unsigned char c : data)
        bw.writeBits(codes_[c]);
    bw.flush();
    fclose(out);

    printf("Compresion completada: %s\n", outputPath.c_str());
    return true;
}

bool Huffman::decompress(const std::string& inputPath, const std::string& outputPath) {
    FILE* in = fopen(inputPath.c_str(), "rb");
    if (!in) {
        printf("Error: no se pudo abrir el archivo comprimido.\n");
        return false;
    }

    HuffmanNode* root = readTree(in);
    fgetc(in);

    unsigned long long totalChars = 0;
    fread(&totalChars, sizeof(totalChars), 1, in);

    BitReader br(in);
    std::string outText;
    outText.reserve((size_t)totalChars);

    HuffmanNode* node = root;
    while (outText.size() < totalChars) {
        int b = br.readBit();
        if (b == -1) break;
        node = (b == 0 ? node->left : node->right);
        if (node->isLeaf()) {
            outText.push_back((char)node->ch);
            node = root;
        }
    }
    fclose(in);

    if (!writeStringToFile(outputPath, outText)) {
        printf("Error: no se pudo escribir el archivo de salida.\n");
        return false;
    }

    printf("Descompresion completada: %s\n", outputPath.c_str());
    return true;
}
