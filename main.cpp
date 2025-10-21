#include "MATRIZBASE.h"


int main() {
    std::cout << "--- Sistema gerico de Algebra Lineal --->" << std::endl;
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << std::endl << std::endl;

    MatrizBase<float>* A = nullptr;
    MatrizBase<float>* B = nullptr;
    MatrizBase<float>* C = nullptr;

    const int FILAS = 3;
    const int COLUMNAS = 2;

    try {
        // 1. Creacion de Matriz Dinamica a traves del puntero base
        std::cout << "// 1. Creacion de Matriz Dinamica (a traves del puntero base)" << std::endl;
        A = new MatrizDinamica<float>(FILAS, COLUMNAS);
        A->cargarValores();

        std::cout << "Matriz A (Dinamica) =" << std::endl;
        A->imprimir();
        std::cout << std::endl;

        // 2. Creacion de Matriz Estatica a traves del puntero base
        std::cout << "// 2. Creacion de Matriz Estatica (a traves del puntero base)" << std::endl;
        std::cout << "Creando Matriz Estatica B (" << FILAS << "x" << COLUMNAS << ")..." << std::endl;

        B = new MatrizEstatica<float, FILAS, COLUMNAS>();
        B->cargarValores();

        std::cout << "Matriz B (Estatica) =" << std::endl;
        B->imprimir();
        std::cout << std::endl;

        // 3. Operacion Polimorfica (Suma A + B)
        std::cout << "// 3. Operacion Polimorfica (Suma) - Matriz C = A + B" << std::endl;
        std::cout << "SUMANDO: Matriz A (Dinamica) + Matriz B (Estatica)..." << std::endl;

        // Uso del operador sobrecargado '+' que llama al metodo virtual 'sumar'
        C = *A + *B;

        if (C) {
            std::cout << std::endl << "Matriz Resultado C (" << C->getFilas() << "x" << C->getColumnas() << ", Tipo FLOAT):" << std::endl;
            C->imprimir();
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        // Manejo de errores de tiempo de ejecucion (ej. dimensiones incompatibles)
        std::cerr << "Fallo en tiempo de ejecucion: " << e.what() << std::endl;
    }

    // 4. Limpieza de Memoria y Destructores Virtuales
    std::cout << ">> 4. Demostracion de Limpieza de Memoria <<" << std::endl;

    if (C) {
        std::cout << "Llamando al destructor de C..." << std::endl;
        delete C; // Destructor virtual
    }

    if (A) {
        std::cout << "Llamando al destructor de A..." << std::endl;
        delete A; // Destructor virtual
    }

    if (B) {
        std::cout << "Llamando al destructor de B..." << std::endl;
        delete B; // Destructor virtual
    }

    std::cout << "Sistema cerrado. Fin del programa." << std::endl;
    return 0;
}