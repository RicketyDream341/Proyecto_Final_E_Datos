#include "Huffman.h"
#include "utils.h"
#include <queue>
#include <iostream>
using namespace std;

struct NodeCmp {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) const {
        return a->freq > b->freq;
    }
};

HuffmanNode* Huffman::buildTreeFromFreq(const unordered_map<unsigned char, ull>& freq) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, NodeCmp> pq;

    for (auto& p : freq)
        pq.push(new HuffmanNode(p.first, p.second));

    if (pq.empty()) return nullptr;
    if (pq.size() == 1) {
        HuffmanNode* only = pq.top(); pq.pop();
        return new HuffmanNode(only, new HuffmanNode((unsigned char)0, 0));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        pq.push(new HuffmanNode(left, right));
    }
    return pq.top();
}

void Huffman::generateCodes(HuffmanNode* node, const string& prefix) {
    if (!node) return;
    if (node->isLeaf()) {
        codes_[node->ch] = prefix.empty() ? "0" : prefix;
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
    }
    return new HuffmanNode(readTree(in), readTree(in));
}

bool Huffman::compress(const string& inputPath, const string& outputPath) {
    string data;
    if (!readFileToString(inputPath, data)) {
        cout << "Error leyendo archivo.\n";
        return false;
    }

    unordered_map<unsigned char, ull> freq;
    for (unsigned char c : data) freq[c]++;

    HuffmanNode* root = buildTreeFromFreq(freq);
    generateCodes(root, "");

    FILE* out = fopen(outputPath.c_str(), "wb");
    if (!out) return false;

    writeTree(out, root);
    fputc('\n', out);

    ull total = data.size();
    fwrite(&total, sizeof(total), 1, out);

    BitWriter bw(out);
    for (unsigned char c : data)
        bw.writeBits(codes_[c]);
    bw.flush();
    fclose(out);

    cout << "Archivo comprimido: " << outputPath << "\n";
    return true;
}

bool Huffman::decompress(const string& inputPath, const string& outputPath) {
    FILE* in = fopen(inputPath.c_str(), "rb");
    if (!in) {
        cout << "Error abriendo archivo comprimido.\n";
        return false;
    }

    HuffmanNode* root = readTree(in);
    fgetc(in);

    ull total = 0;
    fread(&total, sizeof(total), 1, in);

    BitReader br(in);
    string outText;
    outText.reserve((size_t)total);

    HuffmanNode* node = root;
    while (outText.size() < total) {
        int bit = br.readBit();
        if (bit == -1) break;
        node = (bit == 0 ? node->left : node->right);
        if (node->isLeaf()) {
            outText += (char)node->ch;
            node = root;
        }
    }
    fclose(in);

    if (!writeStringToFile(outputPath, outText)) {
        cout << "Error al escribir archivo.\n";
        return false;
    }

    cout << "Archivo descomprimido: " << outputPath << "\n";
    return true;
}

void Huffman::printCodes() const {
    cout << "Caracter | Código\n";
    for (auto& p : codes_) {
        if (p.first == '\n') cout << "\\n";
        else if (p.first == ' ') cout << "' '";
        else cout << p.first;
        cout << " -> " << p.second << endl;
    }
}

