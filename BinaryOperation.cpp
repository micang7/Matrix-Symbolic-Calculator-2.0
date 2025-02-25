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

TreeNode* BinaryOperation::nextFreeNode()
{
	if (!m_leftChild || !m_rightChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool BinaryOperation::simplify(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
	if (m_leftChild->simplify(ast)) return true;
	ast.printTree(this, TREE_NODE_COLOR2);
	if (m_rightChild->simplify(ast)) return true;
	ast.printTree(this, TREE_NODE_COLOR2);
	return m_leftChild->simplify_L(ast, this);
#else
	if (m_leftChild->simplify(ast)) return true;
	if (m_rightChild->simplify(ast)) return true;
	return m_leftChild->simplify_L(ast, this);
#endif
}

bool BinaryOperation::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool BinaryOperation::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool BinaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool BinaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool BinaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool BinaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool BinaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
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
