#include <iostream>

using namespace std;

// one pointer implies a singly linked list
// two pointers implies a doubly linked list
// N pointers implies graphs
struct Node {
	// data here
	int data;
	Node* next;

	Node(int data) : data(data), next(nullptr) {}
};

// singly linked list
class LinkedList {
	Node* head;
	Node* tail;
public:
	LinkedList() : head(nullptr), tail(nullptr) {}
};

int main(int argc, char** argv) {
	LinkedList L;
	return 0;
}
