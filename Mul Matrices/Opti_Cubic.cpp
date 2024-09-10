#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

typedef vector<vector<int>> Matrix;

void transposeMatrix(const Matrix& B, Matrix& B_T) {
    int n = B.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B_T[j][i] = B[i][j];
        }
    }
}

void mulMat(const Matrix& A, const Matrix& B_T, Matrix& C) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B_T[j][k];
            }
        }
    }
}

bool readF(ifstream& file, Matrix& matrix) {
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

void CSV(const vector<pair<int, double>>& tims, const string& filename) {
    ofstream file(filename);
    file << "Matrix,T promedio [s]\n";  
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

    while (true) {
        Matrix A, B, B_T, C;

        if (!readF(matrixFile, A)) {
            break;
        }
        
        if (!readF(matrixFile, B)) {
            break;
        }

        int n = A.size(); 
        B_T.resize(n, vector<int>(n));
        C.resize(n, vector<int>(n, 0));

        transposeMatrix(B, B_T);  

        double totalT = 0.0;
        for (int j = 0; j < repetir; ++j) {
            auto start = high_resolution_clock::now();  
            mulMat(A, B_T, C);
            auto end = high_resolution_clock::now();  

            totalT += duration<double>(end - start).count();  
        }

        double avgTime = totalT / repetir;  
        tims.push_back({n, avgTime});  

    }

    matrixFile.close(); 

    CSV(tims, "tiempo_optimizado.csv");

    return 0;
}
