#include "Huffman.h"
#include <iostream>
#include <cstddef>
using namespace std;

int main() {
    system("chcp 65001 > nul");
    Huffman huff;
    int opcion;
    string input, output;

    do {
        cout << "\n===== COMPRESOR HUFFMAN =====\n";
        cout << "1. Comprimir archivo\n";
        cout << "2. Descomprimir archivo\n";
        cout << "3. Mostrar códigos Huffman\n";
        cout << "4. Salir\n";
        cout << "Selecciona: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Archivo a comprimir: ";
            cin >> input;
            cout << "Guardar como: ";
            cin >> output;
            huff.compress(input, output);
            break;

        case 2:
            cout << "Archivo a descomprimir: ";
            cin >> input;
            cout << "Guardar como: ";
            cin >> output;
            huff.decompress(input, output);
            break;

        case 3:
            huff.printCodes();
            break;

        case 4:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opción inválida\n";
        }

    } while (opcion != 4);

    return 0;
}
