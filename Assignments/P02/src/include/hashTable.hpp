#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

class HashTable {
private:
    vector<int> PRIME_CAPACITIES;   // moved out of const to fix scope error

    vector<vector<int>> table;
    size_t capacity;
    size_t numElements;
    int primeIndex;
    mutable Counters c{};

    size_t hashFn(int key) const {
        return static_cast<size_t>(key) * 2654435761u;
    }

    size_t indexFor(int key) const {
        return hashFn(key) % capacity;
    }

    double loadFactor() const {
        return static_cast<double>(numElements) / static_cast<double>(capacity);
    }

    void resize() {
        primeIndex++;
        if (primeIndex >= (int)PRIME_CAPACITIES.size()) {
            primeIndex--;
            return;
        }

        c.resize_events++;
        capacity = PRIME_CAPACITIES[primeIndex];
        vector<vector<int>> newTable(capacity);

        for (auto &bucket : table) {
            for (int key : bucket) {
                size_t idx = hashFn(key) % capacity;
                newTable[idx].push_back(key);
                c.structural_ops++;
                c.shifts_relinks++;
            }
        }

        table = move(newTable);
    }

public:
    HashTable() : capacity(7), numElements(0), primeIndex(0) {
        PRIME_CAPACITIES = {7, 11, 23, 47, 97, 197, 397, 797, 1597,
                            3203, 6421, 12853, 25717, 51437};
        table.resize(capacity);
    }

    void resetCounters() { c = {}; }

    void reset() {
        primeIndex  = 0;
        capacity    = PRIME_CAPACITIES[0];
        numElements = 0;
        table.clear();
        table.resize(capacity);
        c = {};
    }

    void save(string filename, bool dict = true) {
        c.saveCounters(filename, dict);
    }

    Counters getCounters() { return c; }

    bool insert(int key) {
        c.inserts++;

        size_t idx    = indexFor(key);
        auto  &bucket = table[idx];

        for (int value : bucket) {
            c.comparisons++;
            if (value == key)
                return false;
        }

        bucket.push_back(key);
        numElements++;
        c.structural_ops++;
        c.shifts_relinks++;

        if (loadFactor() > 0.75)
            resize();

        return true;
    }

    bool contains(int key) const {
        c.lookups++;

        size_t      idx    = indexFor(key);
        const auto &bucket = table[idx];

        for (int value : bucket) {
            c.comparisons++;
            if (value == key)
                return true;
        }

        return false;
    }

    bool erase(int key) {
        c.deletes++;

        size_t idx    = indexFor(key);
        auto  &bucket = table[idx];

        for (size_t i = 0; i < bucket.size(); i++) {
            c.comparisons++;
            if (bucket[i] == key) {
                bucket[i] = bucket.back();
                bucket.pop_back();
                numElements--;
                c.structural_ops++;
                c.shifts_relinks++;
                return true;
            }
        }

        return false;
    }

    void runJobFile(string fname) {
        ifstream f(fname);
        json j = json::parse(f);

        for (auto &element : j) {
            string op    = element["op"];
            int    value = element["value"];

            if (op == "insert")
                insert(value);
            else if (op == "contains")
                contains(value);
            else if (op == "delete")
                erase(value);
        }
    }

    const char *name() const { return "HashTable"; }
};