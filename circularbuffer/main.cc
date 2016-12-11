#include "circularbuffer.h"
#include <iostream>

class My {
public:
	My() {
		std::cout << "ctor" << std::endl;
	}
	My(My&& m) {
		std::cout << "move" << std::endl;
	}
	~My() {
		std::cout << "delete" << std::endl;
	}
};

int main() {
	CircularBuffer<My, 8> cb;
	My a;
	My b;
	cb.push(std::move(a));
	cb.push(std::move(b));
	//std::cout << cb.pop() << std::endl;
	cb.pop();
	cb.pop();
	//std::cout << cb.pop() << std::endl;

}