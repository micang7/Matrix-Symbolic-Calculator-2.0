#include "Variable.h"

Variable::Variable(char name)
{
	m_name = name;
}

TreeNode* Variable::clone() const
{
	return new Variable(*this);
}

Napis Variable::toNapis() const
{
	return m_name;
}
