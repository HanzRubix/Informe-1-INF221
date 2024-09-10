#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
using namespace chrono;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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

        if (dataset.empty()) continue; 

        double totalT = 0.0;

        for (int j = 0; j < repetir; ++j) {
            vector<int> datasetC = dataset;

            auto start = high_resolution_clock::now();
            quickSort(datasetC, 0, datasetC.size() - 1);
            auto end = high_resolution_clock::now();

            totalT += duration<double>(end - start).count();
        }

        double avgTime = totalT / repetir;
        tims.push_back({dataset.size(), avgTime}); 
    }

    datasetFile.close();

    CSV(tims, "tiempo_quick.csv");

    return 0;
}
