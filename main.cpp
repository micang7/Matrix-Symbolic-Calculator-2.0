#include <iostream>
#include "AbstractSyntaxTree.h"

using namespace std;

int main() {

	AbstractSyntaxTree tests[]{
		//AbstractSyntaxTree("6*8-5^2+12/4")
		//AbstractSyntaxTree("0*a-5^2+12/4")
		AbstractSyntaxTree("2+2")
		//AbstractSyntaxTree("2+3+a"),
		//AbstractSyntaxTree("2+(3+a)")
	};

	return 0;
}