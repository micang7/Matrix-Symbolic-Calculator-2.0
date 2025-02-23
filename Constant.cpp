#include "Constant.h"

#include "AbstractSyntaxTree.h"
#include "BinaryOperation.h"
#include "Matrix.h"
#include <iostream>

Constant::Constant(int val)
{
	m_val = val;
}

TreeNode* Constant::clone() const
{
	return new Constant(*this);
}

Napis Constant::toNapis() const
{
	return Napis(m_val);
}
