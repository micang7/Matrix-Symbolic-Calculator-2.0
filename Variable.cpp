#include "Variable.h"

#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "BinaryOperation.h"
#include "Constant.h"
#include "Matrix.h"

Variable::Variable(char name)
{
	m_name = name;
}

TreeNode* Variable::clone() const
{
	return new Variable(*this);
}

bool Variable::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated)
{
	return false;
}

bool Variable::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated)
{
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated)
{
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated)
{
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated)
{
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated)
{
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated)
{
	return false;
}

Napis Variable::toNapis() const
{
	return m_name;
}
