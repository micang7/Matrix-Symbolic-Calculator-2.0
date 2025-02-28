#include <iostream>
#include "AbstractSyntaxTree.h"

using namespace std;

int main() {

	AbstractSyntaxTree tests[]{
		//AbstractSyntaxTree("2+2"),
		//AbstractSyntaxTree("---2"),
		//AbstractSyntaxTree("6*8-5^2+12/4"),
		//AbstractSyntaxTree("0*a-5^2+12/4"),
		//AbstractSyntaxTree("a+5-(2-3)"),
		//AbstractSyntaxTree("5-2+3+a"),
		AbstractSyntaxTree("2+c+((a+3)+b)"),
		AbstractSyntaxTree("2+c+(3+a)+b"),
		AbstractSyntaxTree("2+c+3+a+b"),
		AbstractSyntaxTree("2+3+c+a+b"),
		AbstractSyntaxTree("2+3+a+c+b"),
		AbstractSyntaxTree("2+3+a+b+c")
	};

	return 0;
}