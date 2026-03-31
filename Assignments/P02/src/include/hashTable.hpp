#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <cstddef>
#include <vector>

using json = nlohmann::json;

class HashTable {
private:
    // derived from claude ai
    const vector<int> primes{
        7, 11, 23, 47, 97, 197, 397, 797, 1597,
        3203, 6421, 12853, 25717, 51437
    };
    
    std::vector<std::vector<int>> table;
    std::size_t capacity;
    size_t numElements;
    int primeIndex;         // tracks where we are in PRIME_CAPACITIES
    mutable Counters c{};

    // Integer hash function
    std::size_t hash(int key) const {
        return static_cast<std::size_t>(key) * 2654435761u;
    }

    // Compress hash value into a valid bucket index
    std::size_t indexFor(int key) const {
        return hash(key) % capacity;
    }

    // Function to calculate the load factor
    double loadFactor() const {
        return static_cast<double>(numElements) / static_cast<double>(capacity);
    }

    // Resize and rehash everything into a larger table
    void resize() {
        primeIndex++;
        if (primeIndex >= (int)PRIME_CAPACITIES.size()) {
            // Already at max capacity — don't resize further
            primeIndex--;
            return;
        }
 
        c.resize_events++;                          // a resize is happening
        capacity = PRIME_CAPACITIES[primeIndex];
        vector<vector<int>> newTable(capacity);
 
        // Rehash all existing elements into the new table
        for (auto &bucket : table) {
            for (int key : bucket) {
                size_t idx = hash(key) % capacity;
                newTable[idx].push_back(key);
                c.structural_ops++;                 // each element moved to new bucket
                c.shifts_relinks++;                 // each element relinked in new table
            }
        }
 
        table = move(newTable);
    }
    
public:
    HashTable() : capacity(7), numElements(0), primeIndex(0) {
        table.resize(capacity);
    }
 
    void resetCounters() {
        c = {};
    }
 
    void reset() {
        primeIndex = 0;
        capacity = primes[0];  // back to 7
        numElements = 0;
        table.clear();
        table.resize(capacity);
        c = {};
    }
 
    void save(string filename, bool dict = true) {
        c.saveCounters(filename, dict);
    }
 
    Counters getCounters() {
        return c;
    }
 
    bool insert(int key) {
        c.inserts++;
 
        size_t idx    = indexFor(key);
        auto &bucket = table[idx];
 
        // Check for duplicates first
        for (int value : bucket) {
            c.comparisons++;    // comparing each element
            if (value == key)
                return false;   // duplicate
        }
 
        bucket.push_back(key);
        numElements++;
        c.structural_ops++;    // new element added to table
        c.shifts_relinks++;    // element linked into chain
 
        // Resize if load factor exceeds 0.75
        if (loadFactor() > 0.75)
            resize();
 
        return true;
    }
 
    bool contains(int key) const {
        c.lookups++;
 
        size_t idx = indexFor(key);
        const auto &bucket = table[idx];
 
        for (int value : bucket) {
            c.comparisons++;    // comparing each bucket element
            if (value == key)
                return true;
        }
 
        return false;
    }
 
    bool erase(int key) {
        c.deletes++;
 
        size_t idx = indexFor(key);
        auto &bucket = table[idx];
 
        for (size_t i = 0; i < bucket.size(); i++) {
            c.comparisons++;    // comparing each element
            if (bucket[i] == key) {
                bucket[i] = bucket.back();
                bucket.pop_back();
                numElements--;
                c.structural_ops++;    // element removed
                c.shifts_relinks++;    // swap-pop is a relink
                return true;
            }
        }
        return false;
    }
 
    // Read and process a JSON workload file
    void runJobFile(string fname) {
        ifstream f(fname);
        json j = json::parse(f);
 
        for (auto &element : j)
        {
            string op = element["op"];
            int value = element["value"];
 
            if (op == "insert")
                insert(value);
            else if (op == "contains")
                contains(value);
            else if (op == "delete")
                erase(value);
        }
    }
 
    const char *name() const {
        return "HashTable";
    }
};
