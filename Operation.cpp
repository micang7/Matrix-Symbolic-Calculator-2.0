#include "Operation.h"

#include <iostream>

Operation::Operation(const Napis& opr) : TreeNode(1), m_opr(opr) {}

Napis Operation::toNapisJustNode() const
{
	return m_opr;
}
