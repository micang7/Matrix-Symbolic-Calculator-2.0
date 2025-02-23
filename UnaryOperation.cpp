#include "UnaryOperation.h"

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
