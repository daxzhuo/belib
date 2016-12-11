#include "myvector.h"
#include <iostream>
class Myclass {
public:
	Myclass() {

	}
	Myclass(const Myclass & m) {
		std::cerr << "ctor on " << this << std::endl;
	}
};


int main() {
	dz::myvector<Myclass> mv(1);
	Myclass a;
	mv.insert(mv.begin(), a);
	mv.insert(mv.begin(), a);
	mv.push_back(a);
	mv.push_back(a);

	return 0;
}