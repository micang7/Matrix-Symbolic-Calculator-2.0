#include <iostream>
#include "AbstractSyntaxTree.h"

using namespace std;

int main() {

	AbstractSyntaxTree tests[]{
		AbstractSyntaxTree("2+{{2,4},{3,5}}")
	};

	return 0;
}