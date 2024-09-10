#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

void mulMat(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int filA = A.size();
    int colA = A[0].size();
    int filB = B.size();
    int colB = B[0].size();

    if (colA != filB) {
        return;
    }

    C.resize(filA, vector<int>(colB, 0));

    for (int i = 0; i < filA; ++i) {
        for (int j = 0; j < colB; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < colA; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

bool readF(ifstream& file, vector<vector<int>>& matrix) {
    int rows, cols;
    if (!(file >> rows >> cols)) return false;

    matrix.resize(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(file >> matrix[i][j])) return false;
        }
    }
    return true;
}

void CSV(const vector<pair<int, float>>& tims, const string& filename) {
    ofstream file(filename);
    file << "Matrix,T promedio [s]\n";  
    for (const auto& timing : tims) {
        file << timing.first << "," << timing.second << "\n"; 
    }
    file.close();
}

int main() {
    vector<pair<int, float>> tims;  
    const int repetir = 20;

    ifstream matrixFile("Ingresa aqui el dataset a evaluar.txt"); 
    if (!matrixFile.is_open()) {
        return 1;
    }

    while (true) {
        vector<vector<int>> A, B, C;

        if (!readF(matrixFile, A)) break;

        if (!readF(matrixFile, B)) break;

        if (A[0].size() != B.size()) {
            continue;
        }

        int size = A.size();  
        C.resize(size, vector<int>(B[0].size(), 0));

        float totalT = 0.0f;
        for (int j = 0; j < repetir; ++j) {
            auto start = high_resolution_clock::now();  
            mulMat(A, B, C);
            auto end = high_resolution_clock::now();  

            totalT += duration<float>(end - start).count(); 
        }

        float avgTime = totalT / repetir; 
        tims.push_back({size, avgTime});  
    }

    matrixFile.close();  

    CSV(tims, "tiempo_tradicional.csv");

    return 0;
}
