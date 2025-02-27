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

void BinaryOperation::reorganise(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	m_leftChild->reorganise(ast);
	ast.printTree(this);
	m_rightChild->reorganise(ast);
#else
	m_leftChild->reorganise(ast);
	m_rightChild->reorganise(ast);
#endif
}

void BinaryOperation::evaluate(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
	m_leftChild->evaluate(ast);
	ast.printTree(this, TREE_NODE_COLOR2);
	m_rightChild->evaluate(ast);
	ast.printTree(this, TREE_NODE_COLOR2);
	ast.replaceSubtree(this, m_leftChild->compute1(m_opr, m_rightChild));
#else
	m_leftChild->simplify(ast);
	m_rightChild->simplify(ast);
	ast.replaceSubtree(this, m_leftChild->evaluate1(m_opr, m_rightChild));
#endif
}

AbstractSyntaxTree BinaryOperation::compute1(const Napis& opr, TreeNode* right) const
{
	return right->compute2(*this, opr);
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
