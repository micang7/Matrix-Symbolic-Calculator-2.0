#include "UnaryOperation.h"

#include "AbstractSyntaxTree.h"
#include "BinaryOperation.h"
#include "Constant.h"
#include "Matrix.h"
#include "Variable.h"

UnaryOperation::UnaryOperation(const Napis& opr) : Operation(opr) {}

TreeNode* UnaryOperation::clone() const
{
	return new UnaryOperation(*this);
}

TreeNode* UnaryOperation::nextFreeNode()
{
	if (!m_leftChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool UnaryOperation::simplify(AbstractSyntaxTree& ast)
{
	if (m_leftChild->simplify(ast)) return true;
}

bool UnaryOperation::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated)
{
	return false;
}

bool UnaryOperation::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated)
{
	return false;
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated)
{
	return false;
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated)
{
	return false;
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated)
{
	return false;
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated)
{
	return false;
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated)
{
	return false;
}

Napis UnaryOperation::toNapis() const
{
	bool inner_parentheses = m_leftChild->isNegationSignificant();
	bool outer_parentheses = (m_parent->isNegationSignificant() && this == m_parent->m_rightChild);
	return
		(outer_parentheses ? "(" : "") +
		m_opr +
		(inner_parentheses ? "(" : "") +
		m_leftChild->toNapis() +
		(inner_parentheses ? ")" : "") +
		(outer_parentheses ? ")" : "");
}
