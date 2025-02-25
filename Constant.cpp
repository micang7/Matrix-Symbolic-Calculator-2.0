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

bool Constant::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Constant::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return opr->m_rightChild->simplify_R(ast, opr, this);
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	Napis operation = opr->getOpr();
	AbstractSyntaxTree result;
	if (operation == '+')
		result = *left + *this;
	else if (operation == '-')
		result = *left - *this;
	else if (operation == '*')
		result = *left * *this;
	else if (operation == '/')
		result = *left / *this;
	else if (operation == '^')
		result = *left ^ *this;
	ast.addSubtree(ast.removeSubtree(opr), result.getRoot());
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Constant::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
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
