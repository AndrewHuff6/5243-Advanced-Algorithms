#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;
using namespace std;

class Bst
{
protected:
    struct Node
    {
        int data;
        Node *left;
        Node *right;
        int height;

        explicit Node(int value)
            : data(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node *root;
    mutable Counters c{};  // mutable so const methods can increment counters

    // Recursive insert helper
    bool insert(Node *&node, int value)
    {
        c.comparisons++;
        if (!node)
        {
            node = new Node(value);
            c.structural_ops++;
            c.shifts_relinks++;
            return true;
        }

        c.comparisons++;
        if (value < node->data)
            return insert(node->left, value);

        c.comparisons++;
        if (value > node->data)
            return insert(node->right, value);

        return false; // duplicate
    }

    // Recursive search helper
    bool contains(Node *node, int value) const
    {
        c.comparisons++;
        if (!node)
            return false;

        c.comparisons++;
        if (value == node->data)
            return true;

        c.comparisons++;
        if (value < node->data)
            return contains(node->left, value);

        return contains(node->right, value);
    }

    // Find smallest node in subtree
    Node *findMin(Node *node) const
    {
        while (node && node->left)
        {
            c.comparisons++;
            node = node->left;
        }
        return node;
    }

    // Recursive erase helper
    bool erase(Node *&node, int value)
    {
        c.comparisons++;
        if (!node)
            return false;

        c.comparisons++;
        if (value < node->data)
            return erase(node->left, value);

        c.comparisons++;
        if (value > node->data)
            return erase(node->right, value);

        // Found the node to delete
        c.structural_ops++;

        if (!node->left && !node->right)
        {
            delete node;
            node = nullptr;
            c.shifts_relinks++;
            return true;
        }

        if (!node->left)
        {
            Node *temp = node;
            node = node->right;
            delete temp;
            c.shifts_relinks++;
            return true;
        }

        if (!node->right)
        {
            Node *temp = node;
            node = node->left;
            delete temp;
            c.shifts_relinks++;
            return true;
        }

        // Two children: replace with in-order successor
        Node *successor = findMin(node->right);
        node->data = successor->data;
        c.shifts_relinks++;
        return erase(node->right, successor->data);
    }

    // Postorder cleanup
    void clear(Node *node)
    {
        if (!node)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    Bst() : root(nullptr) {}

    void resetCounters() { c = {}; }

    void reset()
    {
        clear(root);
        root = nullptr;
        c = {};
    }

    void save(string filename, bool dict = true)
    {
        c.saveCounters(filename, dict);
    }

    virtual ~Bst() { clear(root); }

    Counters getCounters() { return c; }

    void runJobFile(string fname)
    {
        ifstream f(fname);
        json j = json::parse(f);

        for (auto &element : j)
        {
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

    bool insert(int value)
    {
        c.inserts++;
        return insert(root, value);
    }

    bool contains(int value) const
    {
        c.lookups++;
        return contains(root, value);
    }

    bool erase(int value)
    {
        c.deletes++;
        return erase(root, value);
    }

    virtual const char *name() const { return "BST"; }
};