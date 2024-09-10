#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
using namespace chrono;


void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
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
            mergeSort(datasetC, 0, datasetC.size() - 1);
            auto end = high_resolution_clock::now();

            totalT += duration<double>(end - start).count();
        }

        double avgTime = totalT / repetir;
        tims.push_back({dataset.size(), avgTime}); 
    }

    datasetFile.close();


    CSV(tims, "tiempo_merge.csv");

    return 0;
}
