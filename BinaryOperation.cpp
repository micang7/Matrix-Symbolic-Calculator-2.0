#include "BinaryOperation.h"

#include "Parser.h"

BinaryOperation::BinaryOperation(const Napis& opr) : Operation(opr) {}

TreeNode* BinaryOperation::clone() const
{
	return new BinaryOperation(*this);
}

TreeNode* BinaryOperation::nextFreeNode()
{
	if (!m_leftChild || !m_rightChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool BinaryOperation::simplify(AbstractSyntaxTree& ast)
{
	if (m_leftChild->simplify(ast)) return true;
	if (m_rightChild->simplify(ast)) return true;
}

Napis BinaryOperation::toNapis() const
{
	bool left_parentheses = m_leftChild->lowerPrecedenceThan(m_opr);
	bool right_parentheses = (m_rightChild->lowerPrecedenceThan(m_opr) ||
		(m_opr == '-' && m_rightChild->m_rightChild) ||
		(m_opr == '/' && m_rightChild->m_rightChild));
	return
		(left_parentheses ? "(" : "") +
		m_leftChild->toNapis() +
		(left_parentheses ? ")" : "") +
		m_opr +
		(right_parentheses ? "(" : "") +
		m_rightChild->toNapis() +
		(right_parentheses ? ")" : "");
}

bool BinaryOperation::lowerPrecedenceThan(const Napis& opr2) const
{
	return precedence(m_opr.getStr()[0]) < precedence(opr2.getStr()[0]);
}

bool BinaryOperation::isNegationSignificant() const
{
	return m_opr == '+' || m_opr == '-';
}
