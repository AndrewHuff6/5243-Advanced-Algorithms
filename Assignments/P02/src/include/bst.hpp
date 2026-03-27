#pragma once

#include "counters.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;


using namespace std;

// struct Counters {
//     long long comparisons = 0;
//     long long structural_ops = 0;
//     long long inserts = 0;
//     long long deletes = 0;
//     long long lookups = 0;
//     long long resize_events = 0;

//     ostream& operator <<(ostream& os, const Counter &o){
//         return os << o.comparisons<<" "<<o.structural_ops<<" "<<o.inserts<<std::endl;
//     }
// };

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
            c.structural_ops++;
            node = new Node(value);
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

        // comparison+
        if (!node)
        {

            return false;
        }

        // comparison++
        if (value == node->data)
        {

            return true;
        }
        // comparison++
        if (value < node->data)
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
            // comparison++
            node = node->left;
        }
        return node;
    }

    // Recursive erase helper
    bool erase(Node *&node, int value)
    {

        // comparison++
        if (!node)
        {
            return false;
        }

        // comparison++
        if (value < node->data)
        {
            return erase(node->left, value);
        }

        // comparison++
        if (value > node->data)
        {
            return erase(node->right, value);
        }

        // Found node to delete

        // Case 1: leaf node
        if (!node->left && !node->right)
        {
            //
            // structural_ops++
            delete node;
            node = nullptr;
            return true;
        }

        // Case 2: only right child
        if (!node->left)
        {
            Node *temp = node;
            node = node->right;
            // structural_ops++
            delete temp;
            return true;
        }

        // Case 3: only left child
        if (!node->right)
        {
            Node *temp = node;
            node = node->left;
            // structural_ops++
            delete temp;
            return true;
        }

        // Case 4: two children
        Node *successor = findMin(node->right);
        node->data = successor->data;
        return erase(node->right, successor->data);
    }

    // Postorder cleanup helper
    void clear(Node *node)
    {
        // compare++
        if (!node)
        {
            return;
        }

        clear(node->left);
        clear(node->right);
        // structural_ops++
        delete node;
    }

public:
    Bst() : root(nullptr)
    {

    }

    void reset(){
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
        }
    }

    bool insert(int value)
    {
        // insert++
        c.inserts++;
        return insert(root, value);
        
    }

    bool contains(int value) const
    {
        // lookup++
        return contains(root, value);
    }

    bool erase(int value)
    {
        // delete++
        return erase(root, value);
    }

    virtual const char *name() const
    {
        return "BST";
    }
};
