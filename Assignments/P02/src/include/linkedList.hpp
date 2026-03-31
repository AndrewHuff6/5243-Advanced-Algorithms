#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

class LinkedList {
private:
    struct Node {
        int data;
        Node *next;

        Node(int v) : data(v), next(nullptr) {}
    };

    Node *head;
    mutable Counters c{};

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node *curr = head;
        while (curr) {
            Node *temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    // Resets only the counters
    void resetCounters() {
        c = {};
    }

    // Deletes all of the nodes
    void reset() {
        Node *curr = head;
        while (curr) {
            Node *temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = nullptr;
        c = {};
    }

    bool insert(int value) {
        c.inserts++;
        if (contains(value))
            return false;

        // Store value to the front
        Node *n = new Node(value);
        n->next = head;
        head = n;
        c.structural_ops++;
        c.shifts_relinks++;

        return true;
    }

    bool contains(int value) const {
        c.lookups++;
        Node *curr = head;

        while (curr) {
            c.comparisons++;    // another comparison
            if (curr->data == value)
                return true;
            curr = curr->next;
        }

        return false;
    }

    bool erase(int value) {
        c.deletes++;
        Node *curr = head;
        Node *prev = nullptr;

        while (curr) {
            c.comparisons++;     // another comparison
            if (curr->data == value) {
                if (prev)
                    prev->next = curr->next;
                    c.shifts_relinks++;
                else
                    head = curr->next;
                    c.shifts_relinks++;

                delete curr;
                c.structural_ops++;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    // Read and process a JSON workload file
    void runJobFile(string fname) {
        ifstream f(fname);
        json j = json::parse(f);
 
        for (auto &element : j) {
            string op = element[0];
            int value = element[1];
            // printing
            if (op == "insert")
                insert(value);
            else if (op == "contains")
                contains(value);
            else if (op == "delete")
                erase(value);
        }
    }

    void print() const {
        Node *curr = head;
        while (curr) {
            std::cout << curr->data << " ";
            curr = curr->next;
        }

        std::cout << "\n";
    }

    const char *name() const {
        return "LinkedList";
    }
};
