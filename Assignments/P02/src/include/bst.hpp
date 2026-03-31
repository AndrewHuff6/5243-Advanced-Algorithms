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
    Counters c{};

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
        {
            return insert(node->left, value);
        }
        
        c.comparisons++;
        if (value > node->data)
        {
            return insert(node->right, value);
        }

        // duplicate value: do nothing
        return false;
    }

    // Recursive search helper
    bool contains(Node *node, int value) const
    {

        c.comparisons++;
        if (!node)    // if node is null
        {
            return false;
        }

        c.comparisons++;
        if (value == node->data)  // if the value is equal to node value
        {
            return true;
        }
        
        c.comparisons++;
        if (value < node->data)    // if the value is less than node value
        {
            return contains(node->left, value);
        }

        return contains(node->right, value);
    }

    // Find smallest node in subtree
    Node *findMin(Node *node) const
    {
        // lookup++
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
        if (!node)    // if node is null
        {
            return false;
        }

        c.comparisons++;
        if (value < node->data)    // if value is less than node value
        {
            return erase(node->left, value);
        }

        c.comparisons++;
        if (value > node->data)    // if value is greater than node value
        {
            return erase(node->right, value);
        }

        // Found node to delete
        c.structural_ops++;
        
        // Case 1: leaf node
        if (!node->left && !node->right)
        {
            delete node;
            node = nullptr;
            c.structural_ops++;
            return true;
        }

        // Case 2: only right child
        if (!node->left)
        {
            Node *temp = node;
            node = node->right;
            delete temp;
            c.structural_ops++;
            return true;
        }

        // Case 3: only left child
        if (!node->right)
        {
            Node *temp = node;
            node = node->left;
            delete temp;
            c.structural_ops++;
            return true;
        }

        // Case 4: two children
        Node *successor = findMin(node->right);
        node->data = successor->data;
        c.structural_ops++;
        return erase(node->right, successor->data);
    }

    // Postorder cleanup helper
    void clear(Node *node)
    {
        c.comparisons++
        if (!node)
        {
            return;
        }

        clear(node->left);
        clear(node->right);
        c.structural_ops++;
        delete node;
    }

public:
    Bst() : root(nullptr)
    {

    }

    // Reset the entire run
    void reset(){
         clear(root);
         root = nullptr;
         c = {}; 
    }

    // Resets the counters, set to empty
    void resetCounters(){
         c = {}; 
    }

    void save(string filename,bool dict=true){
        c.saveCounters(filename,dict);
    }

    virtual ~Bst()
    {
        clear(root);
    }

    Counters getCounters(){
        return c;
    }

    // example to read in json file and process it
    void runJobFile(std::string fname)
    {
        std::ifstream f(fname);
        json j = json::parse(f);
        // std::cout<<j<<std::endl;

        // iterate over json object and print out each operation with value
        // replace the print with actual operations, to process entire file.
        for (auto &element : j)
        {
            std::cout << element << '\n';
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

    bool insert(int value)
    {
        c.inserts++;
        return insert(root, value);
        
    }

    bool contains(int value) const
    {
        c.lookups++
        return contains(root, value);
    }

    bool erase(int value)
    {
        c.deletes++
        return erase(root, value);
    }

    virtual const char *name() const
    {
        return "BST";
    }
};
