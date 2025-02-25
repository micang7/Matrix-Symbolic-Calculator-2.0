#include <iostream>
#include "AbstractSyntaxTree.h"

using namespace std;

int main() {

	AbstractSyntaxTree tests[]{
		//AbstractSyntaxTree("6*8-5^2+12/4")
		//AbstractSyntaxTree("2+2")
		AbstractSyntaxTree("2*a/(3*b)"),
		AbstractSyntaxTree("2+a+(4+b)+3"),
		AbstractSyntaxTree("2+a+4+b+3")
	};

	return 0;
}