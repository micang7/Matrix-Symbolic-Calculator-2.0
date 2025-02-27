#include "BinaryOperation.h"

#include "Parser.h"
#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "Constant.h"
#include "Matrix.h"
#include "Variable.h"

BinaryOperation::BinaryOperation(const Napis& opr) : Operation(opr) {}

TreeNode* BinaryOperation::clone() const
{
	return new BinaryOperation(*this);
}

TreeNode* BinaryOperation::shift()
{
	return new BinaryOperation(std::move(*this));
}

TreeNode* BinaryOperation::nextFreeNode()
{
	if (!m_leftChild || !m_rightChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

void BinaryOperation::simplify(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	m_leftChild->simplify(ast);
	ast.printTree(this);
	m_rightChild->simplify(ast);
	ast.printTree(this);
	ast.replaceSubtree(this, m_leftChild->evaluate1(m_opr, m_rightChild));
#else
	m_leftChild->simplify(ast);
	m_rightChild->simplify(ast);
	ast.replaceSubtree(this, m_leftChild->evaluate1(m_opr, m_rightChild));
#endif
}

AbstractSyntaxTree BinaryOperation::evaluate1(const Napis& opr, TreeNode* right) const
{
	return right->evaluate2(*this, opr);
}

Napis BinaryOperation::toNapis() const
{
	bool left_parentheses = m_leftChild->lowerPrecedenceThan(m_opr);
	bool right_parentheses = (m_rightChild->lowerPrecedenceThan(m_opr) ||
		(m_opr == '-' && m_rightChild->equalPrecedenceAs(m_opr)) ||
		(m_opr == '/' && m_rightChild->equalPrecedenceAs(m_opr)));
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

bool BinaryOperation::equalPrecedenceAs(const Napis& opr2) const
{
	return precedence(m_opr.getStr()[0]) == precedence(opr2.getStr()[0]);
}

bool BinaryOperation::isNegationSignificant() const
{
	return m_opr == '+' || m_opr == '-';
}
