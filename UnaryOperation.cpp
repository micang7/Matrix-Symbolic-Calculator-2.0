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
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
	if (m_leftChild->simplify(ast)) return true;
	ast.printTree(this, TREE_NODE_COLOR2);
	bool result = m_leftChild->simplify_L(ast, this, m_opr == '-');
	ast.printTree(this, TREE_NODE_COLOR2);
	return result;
#else
	if (m_leftChild->simplify(ast)) return true;
	return m_leftChild->simplify_L(ast, this, m_opr == '-');
#endif
}

bool UnaryOperation::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		if (leftIsNegated) {
			ast.removeUnaryNode(m_parent);
			ast.removeUnaryNode(this);
			return false;
		}
		bool result = m_leftChild->simplify_L(ast, opr, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') {
		if (leftIsNegated) {
			ast.removeUnaryNode(m_parent);
			ast.removeUnaryNode(this);
			return false;
		}
		return m_leftChild->simplify_L(ast, opr, true);
	}
	return false;
#endif
}

bool UnaryOperation::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		bool result = m_leftChild->simplify_L(ast, opr, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') return m_leftChild->simplify_L(ast, opr, true);
	return false;
#endif
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		bool result = m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') return m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
	return false;
#endif
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		bool result = m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') return m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
	return false;
#endif
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		bool result = m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') return m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
	return false;
#endif
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		bool result = m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') return m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
	return false;
#endif
}

bool UnaryOperation::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-') {
		bool result = m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
		ast.printTree(this);
		return result;
	}
	return false;
#else
	if (m_opr == '-') return m_leftChild->simplify_R(ast, opr, leftIsNegated, left, true);
	return false;
#endif
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
