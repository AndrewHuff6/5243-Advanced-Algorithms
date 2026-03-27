#include <iostream>

class LinkedList {
private:
    struct Node {
        int data;
        Node *next;

        Node(int v) : data(v), next(nullptr) {}
    };

    Node *head;

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

    bool insert(int value) {

        if (contains(value))
            return false;

        Node *n = new Node(value);

        n->next = head;
        head = n;

        return true;
    }

    bool contains(int value) const {
        Node *curr = head;

        while (curr) {
            if (curr->data == value)
                return true;

            curr = curr->next;
        }

        return false;
    }

    bool erase(int value) {
        Node *curr = head;
        Node *prev = nullptr;

        while (curr) {
            if (curr->data == value) {

                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;

                delete curr;
                return true;
            }

            prev = curr;
            curr = curr->next;
        }

        return false;
    }

    void print() const {
        Node *curr = head;

        while (curr) {
            std::cout << curr->data << " ";
            curr = curr->next;
        }

        std::cout << "\n";
    }
};
