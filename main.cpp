#include "Huffman.h"
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

extern std::unordered_map<unsigned char, std::string> globalCodes;

void mostrarMenu(Huffman &huff) {
    while (true) {
        std::cout << "======COMPRESOR HUFFMAN=======\n";
        std::cout<< endl;
        std::cout << "1. Comprimir archivo\n";
        std::cout << "2. Descomprimir archivo\n";
        std::cout << "3. Mostrar codigos Huffman\n";
        std::cout << "4. Salir\n";
        std::cout << "Selecciona: ";

        int opcion;
        std::cin >> opcion;
        std::cin.ignore();

        std::string in, out;

        if (opcion == 1) {
            std::cout << "Archivo a comprimir: ";
            std::getline(std::cin, in);
            std::cout << "Guardar como: ";
            std::getline(std::cin, out);

            if (huff.compress(in, out))
                std::cout << "Archivo comprimido correctamente.\n";
            else
                std::cout << "Error al comprimir archivo.\n";
        }
        else if (opcion == 2) {
            std::cout << "Archivo a descomprimir: ";
            std::getline(std::cin, in);
            std::cout << "Guardar como: ";
            std::getline(std::cin, out);

            if (huff.decompress(in, out))
                std::cout << "Archivo descomprimido correctamente.\n";
            else
                std::cout << "Error al descomprimir archivo.\n";
        }
        else if (opcion == 3) {
            std::cout << "\nCodigos Huffman\n";

            if (globalCodes.empty()) {
                std::cout << "Debe haber un archivo comprimido antes.\n";
            } else {
                for (auto &p : globalCodes) {
                    unsigned char c = p.first;
                    std::string code = p.second;

                    if (c == '\n')
                        std::cout << "[\\n] = " << code << "\n";
                    else if (c == ' ')
                        std::cout << "[espacio] = " << code << "\n";
                    else
                        std::cout << c << " = " << code << "\n";
                }
            }
        }
        else if (opcion == 4) {
            std::cout << "Saliendo\n";
            break;
        }
        else {
            std::cout << "Opcion invalida.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    Huffman huff;

    if (argc == 4) {
        std::string mode = argv[1];
        std::string input = argv[2];
        std::string output = argv[3];

        if (mode == "-c") {
            if (huff.compress(input, output))
                std::cout << "Compresion finalizada correctamente.\n";
            else
                std::cout << "Error en la compresion.\n";
        }
        else if (mode == "-d") {
            if (huff.decompress(input, output))
                std::cout << "Descompresion finalizada correctamente.\n";
            else
                std::cout << "Error en la descompresion.\n";
        }
        else {
            std::cout << "Comando no reconocido.\n";
            std::cout << "Ejemplos:\n";
            std::cout << "  huffman.exe -c input.txt output.bin\n";
            std::cout << "  huffman.exe -d input.bin output.txt\n";
        }

        return 0;
    }

    mostrarMenu(huff);

    return 0;
}
