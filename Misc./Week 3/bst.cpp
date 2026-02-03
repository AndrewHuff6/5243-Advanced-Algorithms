#include <iostream>
#include <time.h>

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node():data(0), left(nullptr), right(nullptr){}
    Node(int data):data(data), left(nullptr), right(nullptr){}
};

class Bst {
private:
    Node* root;
    void _insert(Node* &sub_root, Node* &node) {
        if(!sub_root) {
            sub_root = node;
            return;
        }
        if(node->data < sub_root->data) {
            _insert(sub_root->left,node);
        }
        else {
            _insert(sub_root->right,node);
        }
    }
    void _print(Node* sub_root) {
        if(!sub_root) {
            return;
        }
        // Preorder traversal
        _print(sub_root->left);
        cout << sub_root->data << " ";
        _print(sub_root->right);
        // Postorder traversal
    }
public:
    // Constructor
    Bst() {
        root = nullptr;
    }
    void insert(int x) {
        Node *temp = new Node(x);
        if(!root) {
            root = temp;
        }
        else {
            _insert(root,temp);
        }
        return;
    }
    int search() {
        return 0;
    }
    // Print method
    void print() {
        _print(root);
    }
};

int main(int argc, char** argv) {
    srand(time(0));
    Bst bst;
    bst.insert(500);
    bst.insert(8);
    bst.insert(7);
    for (int i = 0; i < 10; i++) {
        int r = rand()%1000;
        bst.insert(r);
    }
    cout << endl;
    bst.print();
    cout << endl;
    return 0;
}