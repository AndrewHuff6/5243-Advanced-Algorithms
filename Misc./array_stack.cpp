#include <iostream>

using namespace std;

struct Stack {
private:
	int* stack;
	int top;
	int size;

	bool full() {
		return false;
	}
	bool empty() {
		return false;
	}

public:
	Stack() :stack(new int[10]), top(-1), size(10) {}	// default size 10
	Stack(int size) :stack(new int[size]), top(-1), size(size) {}

	// Push
	int push(int x) {
		if (!full()) {
			top++;	// preincrement top
			stack[top] = x;	// assign to x
			return x;
		}
		else {
			cout << "Stack is full!" << endl;
		}
		return -9999999; // return if stack is full, sentinel value
	}

	// Pop
	int pop() {
		if (!empty()) {
			int x = stack[top]; // get top element
			top--;	// decrement top
			return x;
		}
		else {
			cout << "Stack is empty!" << endl;
		}
		return -9999999; // return if stack is empty, sentinel value
	}

	// Print the stack
	void print() {
		for (int i = top; i >= size; i--) {
			cout << stack[i] << " ";
		}
		cout << endl;
	}
};

int main(int argc, char** argv) {
	Stack stack;
	stack.push(4);
	stack.push(11);
	stack.push(76);
	stack.print();
}
