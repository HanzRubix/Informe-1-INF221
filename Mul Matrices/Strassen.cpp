#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

typedef vector<vector<int>> Matrix;

Matrix add(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix C(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

Matrix subtract(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix C(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

Matrix strassen(const Matrix &A, const Matrix &B) {
    int n = A.size();
    if (n == 1) {
        Matrix C(1, vector<int>(1));
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int newSize = n / 2;
    Matrix A11(newSize, vector<int>(newSize));
    Matrix A12(newSize, vector<int>(newSize));
    Matrix A21(newSize, vector<int>(newSize));
    Matrix A22(newSize, vector<int>(newSize));
    Matrix B11(newSize, vector<int>(newSize));
    Matrix B12(newSize, vector<int>(newSize));
    Matrix B21(newSize, vector<int>(newSize));
    Matrix B22(newSize, vector<int>(newSize));

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    Matrix M1 = strassen(add(A11, A22), add(B11, B22));
    Matrix M2 = strassen(add(A21, A22), B11);
    Matrix M3 = strassen(A11, subtract(B12, B22));
    Matrix M4 = strassen(A22, subtract(B21, B11));
    Matrix M5 = strassen(add(A11, A12), B22);
    Matrix M6 = strassen(subtract(A21, A11), add(B11, B12));
    Matrix M7 = strassen(subtract(A12, A22), add(B21, B22));

    Matrix C(n, vector<int>(n));
    Matrix C11 = add(subtract(add(M1, M4), M5), M7);
    Matrix C12 = add(M3, M5);
    Matrix C21 = add(M2, M4);
    Matrix C22 = add(subtract(add(M1, M3), M2), M6);

    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }

    return C;
}

bool readMatrixFromFile(ifstream& file, Matrix& matrix) {
    int fil, col;
    if (!(file >> fil >> col)) return false;

    matrix.resize(fil, vector<int>(col));
    for (int i = 0; i < fil; ++i) {
        for (int j = 0; j < col; ++j) {
            if (!(file >> matrix[i][j])) return false;
        }
    }
    return true;
}

void saveExecutionTimesToCSV(const vector<pair<int, double>>& tims, const string& filename) {
    ofstream file(filename);
    file << "Matrix Size,T promedio [s]\n"; 
    for (const auto& timing : tims) {
        file << timing.first << "," << timing.second << "\n"; 
    }
    file.close();
}

int main() {
    vector<pair<int, double>> tims; 
    const int repetir = 20; 

    ifstream matrixFile("Ingresa aqui el dataset a evaluar.txt");
    if (!matrixFile.is_open()) {
        return 1;
    }

    int matrixIndex = 1;
    while (true) {
        Matrix A, B, C;

        if (!readMatrixFromFile(matrixFile, A)) break;

        if (!readMatrixFromFile(matrixFile, B)) break;

        if (A[0].size() != B.size()) {
            continue;
        }

        double totalT = 0.0;
        for (int j = 0; j < repetir; ++j) {
            auto start = high_resolution_clock::now();
            C = strassen(A, B);
            auto end = high_resolution_clock::now();

            totalT += duration<double>(end - start).count();
        }

        double avgTime = totalT / repetir;
        tims.push_back({A.size(), avgTime});

        matrixIndex++;
    }

    matrixFile.close();

    saveExecutionTimesToCSV(tims, "tiempo_strassen.csv");

    return 0;
}
