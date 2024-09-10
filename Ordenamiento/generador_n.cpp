#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void saveF(const vector<vector<int>>& datasets, const string& filename) {
    ofstream file(filename);
    for (const auto& dataset : datasets) {
        for (int num : dataset) {
            file << num << " ";
        }
        file << endl; 
    }
    file.close();
}

vector<int> genRandom(int size, int maxValue) {
    vector<int> dataset(size);
    for (int i = 0; i < size; ++i) {
        dataset[i] = rand() % maxValue;
    }
    return dataset;
}

vector<int> genSort(int size) {
    vector<int> dataset(size);
    for (int i = 0; i < size; ++i) {
        dataset[i] = i + 1;
    }
    return dataset;
}

vector<int> genRever(int size) {
    vector<int> dataset = genSort(size);
    reverse(dataset.begin(), dataset.end());
    return dataset;
}

vector<int> genSemiSort(int size) {
    vector<int> dataset = genSort(size);
    for (int i = 0; i < size / 4; ++i) {
        swap(dataset[rand() % size], dataset[rand() % size]);
    }
    return dataset;
}

typedef vector<int> (*GenFunc)(int);

void generateAndSaveDatasets(const string& filename, GenFunc genFunc, const vector<int>& sizes) {
    vector<vector<int>> datasets;
    for (int size : sizes) {
        datasets.push_back(genFunc(size));
    }
    saveF(datasets, filename);
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    vector<int> sizes;
    for (int size = 1000; size <= 10000; size += 1000) {
        sizes.push_back(size);
    }

    // Genera datasets aleatorios
    generateAndSaveDatasets("dataset_random.txt", [](int size) { return genRandom(size, 10000); }, sizes);

    // Genera datasets ordenados
    generateAndSaveDatasets("dataset_sorted.txt", genSort, sizes);

    // genera datasets reversamente ordenados
    generateAndSaveDatasets("dataset_reversed.txt", genRever, sizes);

    // genera datasets semi ordenados
    generateAndSaveDatasets("dataset_semi_sorted.txt", genSemiSort, sizes);

    return 0;
}
