#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
#include <cstddef>
#include <iostream>

using json = nlohmann::json;

class SortedArraySet {
private:
    int *data;
    std::size_t count;
    std::size_t capacity;
    mutable Counters c{};

    void resize(std::size_t newCapacity) {
        int *newData = new int[newCapacity];

        for (std::size_t i = 0; i < count; i++) {
            newData[i] = data[i];
            c.shifts_relinks++;
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
        c.resize_events++;
        c.shifts_relinks++;
    }

    // Returns the index where value is found,
    // or where it should be inserted to maintain sorted order.
    std::size_t lowerBound(int value) const {
        std::size_t left = 0;
        std::size_t right = count;

        while (left < right) {
            std::size_t mid = left + (right - left) / 2;
            c.comparisons++;
            if (data[mid] < value) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        return left;
    }

public:
    explicit SortedArraySet(std::size_t initialCapacity = 10)
        : data(new int[initialCapacity]), count(0), capacity(initialCapacity) {}

    ~SortedArraySet() {
        delete[] data;
    }

    // Reset only the counters
    void resetCounters() {
        c = {};
    }

    // Reset the entire array
    void reset() {
        delete[] data;
        capacity = 10;
        data = new int[capacity];
        count = 0;
        c = {};
    }

    void save(string filename, bool dict = true) {
        c.saveCounters(filename, dict);
    }
 
    Counters getCounters() {
        return c;
    }

    std::size_t size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    bool contains(int value) const {
        c.lookups++;
        c.comparisons++;
        if (count == 0) {
            return false;
        }

        std::size_t pos = lowerBound(value);

        c.comparisons++;    // comparison on the next line
        return pos < count && data[pos] == value;
    }

    bool insert(int value) {
        c.inserts++;
        std::size_t pos = lowerBound(value);

        // Duplicate guard: do not insert if already present
        c.comparisons++;
        if (pos < count && data[pos] == value) {
            return false;
        }

        // Grow array if full
        if (count == capacity) {
            resize(capacity * 2);
        }

        // Shift elements right to make room
        for (std::size_t i = count; i > pos; i--) {
            data[i] = data[i - 1];
            c.shifts_relinks++;
        }

        data[pos] = value;
        count++;
        c.structural_ops++;

        return true;
    }

    bool erase(int value) {
        c.deletes++;

        c.comparisons++;
        if (count == 0) {
            return false;
        }

        std::size_t pos = lowerBound(value);

        // check if the value exists at that position
        c.comparisons++;
        if (pos >= count || data[pos] != value) {
            return false;
        }

        // Eager delete:
        // shift everything left immediately to close the gap
        for (std::size_t i = pos; i + 1 < count; i++) {
            data[i] = data[i + 1];
            c.shifts_relinks++;
        }

        count--;
        c.structural_ops++;
        
        return true;
    }

    // Read and process a JSON workload file
    void runJobFile(string fname) {
        ifstream f(fname);
        json j = json::parse(f);
 
        for (auto &element : j) {
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

    void print() const {
        std::cout << "[";
        for (std::size_t i = 0; i < count; i++) {
            std::cout << data[i];
            if (i + 1 < count) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }

    const char *name() const {
        return "SortedArraySet";
    }
};
