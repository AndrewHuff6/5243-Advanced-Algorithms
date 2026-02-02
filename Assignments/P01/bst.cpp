// Altered bst.cpp file - Andrew Huff
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>    // added cmath for prior code to run

using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;

    Node(int x) {
        data = x;
        left = right = nullptr;
    }
};

class GraphvizBST {
public:
    static void saveDotFile(const std::string &filename, const std::string &dotContent) {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << dotContent;
            outFile.close();
            std::cout << "DOT file saved: " << filename << std::endl;
        } else {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
    }

    static std::string generateDot(const Node *root) {
        std::string dot = "digraph BST {\n";
        dot += "    node [fontname=\"Arial\"];\n";
        dot += generateDotHelper(root);
        dot += "}\n";
        return dot;
    }

private:
    static std::string generateDotHelper(const Node *node) {
        if (!node)
            return "";
        std::string result;
        if (node->left) {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->left->data) + " [label=\"L\"];\n";
            result += generateDotHelper(node->left);
        } else {
            std::string nullNode = "nullL" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        if (node->right) {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->right->data) + " [label=\"R\"];\n";
            result += generateDotHelper(node->right);
        } else {
            std::string nullNode = "nullR" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        return result;
    }
};

class Bst {
    Node *root;

    void _print(Node *subroot) {
        if (!subroot) {
            return;
        } else {
            _print(subroot->left);
            cout << subroot->data << " ";
            _print(subroot->right);
        }
    }
    void _insert(Node *&subroot, int x) {
        if (!subroot) { // if(root == nullptr)
            subroot = new Node(x);
        } else {
            if (x < subroot->data) {
                _insert(subroot->left, x);
            } else {
                _insert(subroot->right, x);
            }
        }
    }
    /* 
    Implemented delete method
    When choosing a replacement value after 
    deleting an inner node with two children,
    consider using Inorder Predecessor (largest)
    value in the left subtree)
    Three cases: 
     1. Deleting a leaf node without children
     2. Deleting a node with only one child
     3. Deleting a node with two children
    */
    void _delete(Node *&subroot, int x) {
        // If the node doesn't exist
        if (!subroot) {
            cout<< "Tree cannot delete because it is empty." << endl; // Indicates the node is not found
            return;
        }
        // Search the left first
        if (x < subroot->data) {
            _delete(subroot->left, x);
        }
        // Or the right
        else if (x > subroot->data ) {
            _delete(subroot->right, x);
        }
        else {
            // Case 1 - deleting node without children
            // If there aren't any subroots to the node
            if (!subroot->left && !subroot->right) {
                // delete it and set to null
                delete subroot, subroot = nullptr;
            }
            // Case 2 - deleting node with one child
            // If there exists a right subroot
            else if (!subroot->left && subroot->right) {
                // Creating a temporary node to be used
                Node *temp = subroot;
                // Set the subroot equal to the left subroot
                subroot = subroot->right;
                // Delete the temporary variable after use
                delete temp;
            }
            // If there exists a left subroot
            else if (subroot->left && !subroot->right){
                // Creating a temporary node to be used
                Node *temp = subroot;
                // Set the subroot equal to the right subroot
                subroot = subroot->left;
                // Delete the temporary variable after use
                delete temp;
            }
            else {
                // Case three - deleting node with two children, inorder predecessor
                Node *pred = subroot->left; // predecessor is the nearest left value
                // Search for the greatest value to the left
                while (pred->right) {
                    pred = pred->right;
                }
                // Set the pred data to the subroot data
                subroot->data = pred->data;
                // Delete the pred node
                _delete(subroot->left, pred->data);
            }
        }        
    }
    int _ipl(Node *root, int depth = 0) {
        if (!root)
            return 0; // Base case: Empty subtree contributes 0 to IPL
        return depth + _ipl(root->left, depth + 1) + _ipl(root->right, depth + 1);
    }

public:
    Bst() { root = nullptr; }
    void insert(int x) { _insert(root, x); }
    void deletion(int x) { _delete(root, x); }     // inline to match the others
    bool search(int key) { return 0; }
    void print() { _print(root); }
    void saveDotFile(const std::string &filename) {
        std::string dotContent = GraphvizBST::generateDot(root);
        GraphvizBST::saveDotFile(filename, dotContent);
    }

    /**
     * Computes the Internal Path Length (IPL) of a Binary Search Tree (BST).
     *
     * Definition:
     * The Internal Path Length (IPL) of a BST is the sum of the depths of all nodes in the tree.
     * The depth of a node is the number of edges from the root to that node.
     *
     * Example:
     *        10
     *       /  \
     *      5    15
     *     / \     \
     *    2   7    20
     *
     * IPL = (depth of 10) + (depth of 5) + (depth of 15) + (depth of 2) + (depth of 7) + (depth of 20)
     *     = 0 + 1 + 1 + 2 + 2 + 2 = 8
     *
     * @param root Pointer to the root node of the BST.
     * @param depth Current depth of the node (default is 0 for the root call).
     * @return The sum of depths of all nodes (Internal Path Length).
     */
    int ipl() {
        return _ipl(root);
    }
};

bool unique_value(int *arr, int n, int x) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == x) {
            return false;
        }
    }
    return true;
}

int main() {
    Bst tree;
    int root = pow(2, 15) / 2;
    int max = pow(2, 15) - 1;
    vector<int> arr;
    arr.push_back(root);
    tree.insert(root);
    for (int i = 1; i < 5000; i++) {
        int r = rand() % max;
        while (!unique_value(arr.data(), arr.size(), r)) {
            r = rand() % max;
        }
        tree.insert(r);
        arr.push_back(r);
    }

    tree.print();
    tree.saveDotFile("bst_snapshot.dot");

    Bst tree2;
    tree2.insert(10);
    tree2.insert(5);
    tree2.insert(15);
    tree2.insert(2);
    tree2.insert(7);
    tree2.insert(20);
    cout << "Internal Path Length: " << tree2.ipl() << endl;

    cout <<"--------------------------------------------" << endl;

    // Prints the original BST
    cout << "Original tree: ";
    tree2.print();
    cout << endl;

    // Case 1 test
    tree2.deletion(2);  // Delete leaf node
    cout << "Deleting 2 (leaf): ";
    tree2.print();
    cout << endl;

    // Case 2 test
    tree2.deletion(15);  // Delete node with only right child
    cout << "Deleting 15 (one child): ";
    tree2.print();
    cout << endl;

    // Case 3 test
    tree2.deletion(5);  // Delete node with two children
    cout << "Deleting 5 (two children): ";
    tree2.print();
    cout << endl;

    // Case 4 test
    tree2.deletion(10);
    cout << "Deleting 10 (root): ";
    tree2.print();
    cout << endl;
    
    cout <<"--------------------------------------------" << endl;

    // Empty tree test
    // Should print the "cannot delete" message 
    Bst tree4;
    tree4.print();
    tree4.deletion(1); 

    cout <<"--------------------------------------------" << endl;

    // Tree with one node test
    // Should not return anything. Will be stored, but then deleted before it's printed
    Bst tree5;
    tree5.insert(100);
    tree5.deletion(100); 
    tree5.print();
    cout << "NOTE: This is supposed to return nothing!" << endl;
    cout << "Inserting one element, deleting that element!" << endl;
    cout << endl;

    cout <<"--------------------------------------------" << endl;

    // Sequential (Linked-List) test
    Bst tree6;
    tree6.insert(1);
    tree6.insert(2);
    tree6.insert(3);
    tree6.insert(4);
    tree6.insert(5);
    tree6.insert(6);

    // Prints the original BST
    cout << "Original tree: ";
    tree6.print();
    cout << endl;

    // Case 1 test
    tree6.deletion(1);  // Delete leaf node
    cout << "Deleting 1 (leaf): ";
    tree6.print();
    cout << endl;

    // Case 2 test
    tree6.deletion(5);  // Delete node with only right child
    cout << "Deleting 5 (one child): ";
    tree6.print();
    cout << endl;

    // Case 3 test
    tree6.deletion(2);  // Delete node with two children
    cout << "Deleting 2 (two children): ";
    tree6.print();
    cout << endl;

    // Case 4 test
    tree6.deletion(4);
    cout << "Deleting 4 (root): ";
    tree6.print();
    cout << endl;
}
