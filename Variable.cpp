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

TreeNode* Variable::shift()
{
	return new Variable(std::move(*this));
}

AbstractSyntaxTree Variable::compute1(const Napis& opr, TreeNode* right) const
{
	return right->compute2(*this, opr);
}

Napis Variable::toNapis() const
{
	return m_name;
}
