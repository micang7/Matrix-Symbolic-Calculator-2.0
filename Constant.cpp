#include "Constant.h"

#include "Parser.h"
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

TreeNode* Constant::shift()
{
	return new Constant(std::move(*this));
}

AbstractSyntaxTree Constant::evaluate1(const Napis& opr, TreeNode* right) const
{
	// 1 * x = x;   0 + x = x;   reduction
	if ((m_val == 1 && opr == '*') ||
		(m_val == 0 && opr == '+'))
		return AbstractSyntaxTree(*right);

	// 0 - x = -x;   reduction
	if (m_val == 0 && opr == '-')
		return ~AbstractSyntaxTree(*right);

	// 0 * x = 0;   0 / x = 0   reduction
	if (m_val == 0 && (opr == '*' || opr == '/'))
		return AbstractSyntaxTree(Constant(0));

	return right->evaluate2(*this, opr);
}

AbstractSyntaxTree Constant::evaluate2(const Constant& left, const Napis& opr) const
{
	// x * 1 = x;   x / 1 = x   x + 0 = x;   x - 0 = x;   reduction
	if ((m_val == 1 && (opr == '*' || opr == '/')) ||
		(m_val == 0 && (opr == '+' || opr == '-')))
		return AbstractSyntaxTree(left);

	// x * 0 = 0;   reduction
	if (m_val == 0 && opr == '*')
		return AbstractSyntaxTree(Constant(0));

	// division by zero error
	if (m_val == 0 && opr == '/')
		throw std::runtime_error("Error: AST: Division by zero!");

	if (opr == '+') return left + *this;
	if (opr == '-') return left - *this;
	if (opr == '*') return left * *this;
	if (opr == '/') return left / *this;
	if (opr == '^') return left ^ *this;
	return AbstractSyntaxTree();
}

AbstractSyntaxTree Constant::operator+(const Constant& constant2) const
{
	int result = (m_parent->isNegation() ? -m_val : m_val) +
		(constant2.m_parent->isNegation() ? -constant2.m_val : constant2.m_val);
	return (result < 0 ?
		~AbstractSyntaxTree(Constant(-result)) :
		AbstractSyntaxTree(Constant(result)));
}

AbstractSyntaxTree Constant::operator-(const Constant& constant2) const
{
	int result = (m_parent->isNegation() ? -m_val : m_val) -
		(constant2.m_parent->isNegation() ? -constant2.m_val : constant2.m_val);
	return (result < 0 ?
		~AbstractSyntaxTree(Constant(-result)) :
		AbstractSyntaxTree(Constant(result)));
}

AbstractSyntaxTree Constant::operator*(const Constant& constant2) const
{
	int result = (m_parent->isNegation() ? -m_val : m_val) *
		(constant2.m_parent->isNegation() ? -constant2.m_val : constant2.m_val);
	return (result < 0 ?
		~AbstractSyntaxTree(Constant(-result)) :
		AbstractSyntaxTree(Constant(result)));
}

AbstractSyntaxTree Constant::operator/(const Constant& constant2) const
{
	int nwd = NWD(m_val, constant2.m_val);
	if (nwd == constant2.m_val)
		return ((m_parent->isNegation() xor constant2.m_parent->isNegation()) ?
			~AbstractSyntaxTree(Constant(m_val / constant2.m_val)) :
			AbstractSyntaxTree(Constant(m_val / constant2.m_val)));
	return ((m_parent->isNegation() xor constant2.m_parent->isNegation()) ?
		~(AbstractSyntaxTree(Constant(m_val / nwd)) / Constant(constant2.m_val / nwd)) :
		AbstractSyntaxTree(Constant(m_val / nwd)) / Constant(constant2.m_val / nwd));
}

AbstractSyntaxTree Constant::operator^(const Constant& constant2) const
{
	return (constant2.m_parent->isNegation() ?
		AbstractSyntaxTree(Constant(1)) / Constant(pow(m_val, constant2.m_val)) :
		AbstractSyntaxTree(Constant(pow(m_val, constant2.m_val))));
}

Napis Constant::toNapis() const
{
	return Napis(m_val);
}
