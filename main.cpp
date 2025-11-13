#include "Huffman.h"
#include <iostream>
#include <string>

// ============================================================
//  Programa principal para el compresor Huffman
// ============================================================
//
// Uso:
//   huffman.exe -c input.txt output.bin   → comprime
//   huffman.exe -d input.bin output.txt   → descomprime
//
// ============================================================

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Uso:\n";
        std::cout << "  huffman.exe -c <entrada.txt> <salida.bin>   (comprimir)\n";
        std::cout << "  huffman.exe -d <entrada.bin> <salida.txt>   (descomprimir)\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    Huffman huff;

    if (mode == "-c") {
        std::cout << "=== COMPRESIÓN HUFFMAN ===\n";
        if (!huff.compress(inputPath, outputPath)) {
            std::cerr << "Error al comprimir el archivo.\n";
            return 1;
        }
    } else if (mode == "-d") {
        std::cout << "=== DESCOMPRESIÓN HUFFMAN ===\n";
        if (!huff.decompress(inputPath, outputPath)) {
            std::cerr << "Error al descomprimir el archivo.\n";
            return 1;
        }
    } else {
        std::cerr << "Modo no reconocido. Usa -c o -d.\n";
        return 1;
    }

    std::cout << "Proceso completado correctamente.\n";
    return 0;
}
