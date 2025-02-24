#include "Constant.h"

#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "BinaryOperation.h"
#include "Matrix.h"
#include "Variable.h"
#include <iostream>

Constant::Constant(int val)
{
	m_val = val;
}

TreeNode* Constant::clone() const
{
	return new Constant(*this);
}

bool Constant::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated)
{
	return false;
}

bool Constant::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated)
{
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated)
{
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated)
{
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated)
{
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated)
{
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated)
{
	return false;
}

Napis Constant::toNapis() const
{
	return Napis(m_val);
}
