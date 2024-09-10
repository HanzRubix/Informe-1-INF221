#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
using namespace chrono;

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void readF(const string& line, vector<int>& dataset) {
    stringstream ss(line);
    int num;
    dataset.clear();
    while (ss >> num) {
        dataset.push_back(num);
    }
}

void CSV(const vector<pair<int, double>>& tims, const string& filename) {
    ofstream file(filename);
    file << "Tamanio de datos,T promedio [s]\n";
    for (const auto& timing : tims) {
        file << timing.first << "," << timing.second << "\n";
    }
    file.close();
}

int main() {
    vector<pair<int, double>> tims; 
    const int repetir = 20; 

    ifstream datasetFile("Ingresa aqui el dataset a evaluar.txt");
    string line;

    while (getline(datasetFile, line)) {
        vector<int> dataset;
        readF(line, dataset);

        double totalT = 0.0;

        for (int j = 0; j < repetir; ++j) {
            vector<int> datasetC = dataset;

            auto start = high_resolution_clock::now();
            insertionSort(datasetC);
            auto end = high_resolution_clock::now();

            totalT += duration<double>(end - start).count();
        }

        double avgTime = totalT / repetir;
        tims.push_back({dataset.size(), avgTime}); 
    }

    datasetFile.close();

    CSV(tims, "tiempo_insertion.csv");

    return 0;
}
