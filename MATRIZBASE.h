#ifndef MATRIZBASE_H
#define MATRIZBASE_H

#include <iostream>
#include <stdexcept>

// Declaracion anticipada de clases template
template <typename T> class MatrizDinamica;
template <typename T, int M, int N> class MatrizEstatica;

// --------------------------------------------------------
// DECLARACION DE LA CLASE BASE ABSTRACTA
// --------------------------------------------------------
template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    // Constructor y Destructor Virtual
    MatrizBase(int filas, int columnas);
    virtual ~MatrizBase();

    // Metodos Virtuales Puros
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    // Acceso a dimensiones
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};

// --------------------------------------------------------
// DECLARACION DE MATRIZ DINAMICA
// --------------------------------------------------------
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos; 

    // Declaraciones FRIEND: Solucion para el error de template

    template <typename U, int M, int N> // Usamos U para evitar conflicto con T de esta clase
    friend class MatrizEstatica;

    // MatrizDinamica es friend de si misma
    friend class MatrizDinamica;

    // Funciones para la Regla de los Cinco
    void freeMemory();
    void deepCopy(const MatrizDinamica<T>& otra);

public:
    // Constructor
    MatrizDinamica(int filas, int columnas);
    // Destructor
    virtual ~MatrizDinamica() override;
    // Constructor de Copia
    MatrizDinamica(const MatrizDinamica<T>& otra);
    // Operador de Asignacion
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra);

    // Implementacion de la Interfaz Base
    void cargarValores() override;
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override;
    void imprimir() const override;
};

// --------------------------------------------------------
// DECLARACION DE MATRIZ ESTATICA
// --------------------------------------------------------
template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N]; // Arreglo de tamano fijo en el stack

public:
    // Declaraciones FRIEND para el acceso a '_datos'
    friend class MatrizDinamica<T>; // Amistad con MatrizDinamica (solo requiere T)
    friend class MatrizEstatica;

public:
    // Constructor
    MatrizEstatica();
    // Destructor
    virtual ~MatrizEstatica() override;

    // Regla de los Cinco
    MatrizEstatica(const MatrizEstatica& otra) = default;
    MatrizEstatica& operator=(const MatrizEstatica& otra) = default;

    // Implementacion de la Interfaz Base
    void cargarValores() override;
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override;
    void imprimir() const override;
};

// Sobrecarga Global del Operador +
template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& m1, const MatrizBase<T>& m2) {
    return m1.sumar(m2);
}

// =========================================================
// DEFINICION/IMPLEMENTACION DE LOS METODOS
// =========================================================

// Implementacion de MatrizBase
template <typename T>
MatrizBase<T>::MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}

template <typename T>
MatrizBase<T>::~MatrizBase() = default;


// Implementacion de MatrizDinamica
template <typename T>
void MatrizDinamica<T>::freeMemory() {
    if (_datos != nullptr) {
        for (int i = 0; i < this->_filas; ++i) {
            delete[] _datos[i];
        }
        delete[] _datos;
        _datos = nullptr;
    }
}

template <typename T>
void MatrizDinamica<T>::deepCopy(const MatrizDinamica<T>& otra) {
    this->_filas = otra._filas;
    this->_columnas = otra._columnas;

    // 1. Asignar nueva memoria
    _datos = new T*[this->_filas];
    for (int i = 0; i < this->_filas; ++i) {
        _datos[i] = new T[this->_columnas];
        // 2. Copiar los valores
        for (int j = 0; j < this->_columnas; ++j) {
            _datos[i][j] = otra._datos[i][j];
        }
    }
}

template <typename T>
MatrizDinamica<T>::MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas) {
    _datos = new T*[this->_filas];
    for (int i = 0; i < this->_filas; ++i) {
        _datos[i] = new T[this->_columnas];
        for (int j = 0; j < this->_columnas; ++j) {
            _datos[i][j] = T();
        }
    }
    std::cout << "Creando Matriz Dinamica de " << this->_filas << "x" << this->_columnas << "..." << std::endl;
}

template <typename T>
MatrizDinamica<T>::~MatrizDinamica() {
    std::cout << "Liberando memoria de Matriz Dinamica..." << std::endl;
    freeMemory();
}

template <typename T>
MatrizDinamica<T>::MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(0, 0), _datos(nullptr) {
    deepCopy(otra);
}

template <typename T>
MatrizDinamica<T>& MatrizDinamica<T>::operator=(const MatrizDinamica<T>& otra) {
    if (this != &otra) {
        // 1. Liberar los recursos antiguos
        freeMemory();
        // 2. Copiar los recursos nuevos
        deepCopy(otra);
    }
    return *this;
}

template <typename T>
void MatrizDinamica<T>::cargarValores() {
    for (int i = 0; i < this->_filas; ++i) {
        for (int j = 0; j < this->_columnas; ++j) {
            this->_datos[i][j] = (T)((float)(i * this->_columnas + j + 1) * 1.5f);
        }
    }
}

template <typename T>
MatrizBase<T>* MatrizDinamica<T>::sumar(const MatrizBase<T>& otra) const {
    if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
        throw std::runtime_error("Error: Las dimensiones no son compatibles para la suma.");
    }

    std::cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << std::endl;
    MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

    // Caso 1: Dinamica + Dinamica
    if (const MatrizDinamica<T>* otra_dinamica = dynamic_cast<const MatrizDinamica<T>*>(&otra)) {
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra_dinamica->_datos[i][j];
            }
        }
    // Caso 2: Dinamica + Estatica
    } else if (const MatrizEstatica<T, 3, 2>* otra_estatica = dynamic_cast<const MatrizEstatica<T, 3, 2>*>(&otra)) {
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra_estatica->_datos[i][j];
            }
        }
    } else {
        throw std::runtime_error("Error: Tipo de la segunda matriz incompatible para la suma.");
    }

    return resultado;
}

template <typename T>
void MatrizDinamica<T>::imprimir() const {
    for (int i = 0; i < this->_filas; ++i) {
        std::cout << "| ";
        for (int j = 0; j < this->_columnas; ++j) {
            std::cout << _datos[i][j] << " | ";
        }
        std::cout << std::endl;
    }
}

// Implementacion de MatrizEstatica
template <typename T, int M, int N>
MatrizEstatica<T, M, N>::MatrizEstatica() : MatrizBase<T>(M, N) {}

template <typename T, int M, int N>
MatrizEstatica<T, M, N>::~MatrizEstatica() = default;

template <typename T, int M, int N>
void MatrizEstatica<T, M, N>::cargarValores() {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            this->_datos[i][j] = (T)((float)(i * N + j + 1) / 2.0f);
        }
    }
}

template <typename T, int M, int N>
MatrizBase<T>* MatrizEstatica<T, M, N>::sumar(const MatrizBase<T>& otra) const {
    if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
        throw std::runtime_error("Error: Las dimensiones no son compatibles para la suma.");
    }

    std::cout << "(La suma es manejada por el metodo virtual de MatrizEstatica)" << std::endl;
    MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();

    // Caso 1: Estatica + Estatica
    if (const MatrizEstatica<T, M, N>* otra_estatica = dynamic_cast<const MatrizEstatica<T, M, N>*>(&otra)) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra_estatica->_datos[i][j];
            }
        }
    // Caso 2: Estatica + Dinamica
    } else if (const MatrizDinamica<T>* otra_dinamica = dynamic_cast<const MatrizDinamica<T>*>(&otra)) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra_dinamica->_datos[i][j];
            }
        }
    } else {
        throw std::runtime_error("Error: Tipo de la segunda matriz incompatible para la suma.");
    }

    return resultado;
}

template <typename T, int M, int N>
void MatrizEstatica<T, M, N>::imprimir() const {
    for (int i = 0; i < M; ++i) {
        std::cout << "| ";
        for (int j = 0; j < N; ++j) {
            std::cout << _datos[i][j] << " | ";
        }
        std::cout << std::endl;
    }
}

#endif