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
protected:
	Node* head;
	Node* tail;

	// Push to the end of the list
	void endPush(int x) {
		Node* temp = new Node(x);
		// if the list is empty
		if(!head) {
			head = tail = temp;
		}
		// if the list is not empty
		else {
			// set tail's next to temp's address
			tail->next = temp;
			// set the tail equal to the new value
			tail = temp;
		}
	}

	// Push to the front of list
	void frontPush(int x) {
		Node* temp = new Node(x);
		// if the list is empty
		if(!head) {
			head = tail = temp;
		}
		// if the list is not empty
		else {
			// set head's next to temp's address
			temp->next = head;
			// set the head equal to the new value
			head = temp;
		}
	}

public:
	LinkedList() : head(nullptr), tail(nullptr) {}

	// Print method
	void print() {
		Node* travel = head;
		// traverses the entire linked list until null is reached
		while(travel) {
			cout << "[" << travel->data<<"]";
			if(travel->next) {
				cout << "->";
			}
			travel = travel->next;
		}
	}
};

class OrderedList: public LinkedList {
public:
	void push(int x) {
		// if it's empty
		if(!head) {
			frontPush(x);
		}
		// if not empty
		else {
			if(x <= head->data) {
				frontPush(x);
			}
			else if (x >= tail->data) {
				endPush(x);
			}
			// ordered part
			else {
				Node* temp = new Node(x);
				Node* travel = head;
				while(temp->data > travel->next->data) {
					travel = travel->next;
				}
				// insert happens here
				temp->next = travel->next;
				travel->next = temp;

			}
		}
		
	}
};

int main(int argc, char** argv) {
	// LinkedList L;
	OrderedList L;
	for(int i=0; i < 10; i++) {
		// L.endPush(i); // 0-9
		// L.frontPush(i); // 9-0
		L.push(rand()%1000);
	}
	L.print();
	cout << endl;

	return 0;
}
