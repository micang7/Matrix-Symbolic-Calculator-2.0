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

TreeNode* UnaryOperation::shift()
{
	return new UnaryOperation(std::move(*this));
}

TreeNode* UnaryOperation::nextFreeNode()
{
	if (!m_leftChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool UnaryOperation::isNegation() const
{
	return m_opr == '-';
}

void UnaryOperation::reorganise(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	// double negation reduction
	if (m_opr == '-' && m_leftChild->isNegation()) {
		TreeNode* next = m_leftChild->m_leftChild;
		ast.removeOneChildNode(m_leftChild);
		ast.removeOneChildNode(this);
		next->reorganise(ast);
		return;
	}
	m_leftChild->reorganise(ast);
}

void UnaryOperation::evaluate(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
	m_leftChild->evaluate(ast);
	ast.printTree(this, TREE_NODE_COLOR2);
	ast.replaceSubtree(this, m_leftChild->compute(m_opr));
#else
	m_leftChild->evaluate(ast);
	ast.replaceSubtree(this, m_leftChild->result(m_opr));
#endif
}

AbstractSyntaxTree UnaryOperation::compute(const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->compute(opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::compute1(const Napis& opr, TreeNode* right) const
{
	if (m_opr == '-') return m_leftChild->compute1(opr, right);
	return right->compute2(*this, opr);
}

AbstractSyntaxTree UnaryOperation::compute2(const Constant& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->compute2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::compute2(const Matrix& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->compute2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::compute2(const Variable& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->compute2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::compute2(const BinaryOperation& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->compute2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::compute2(const UnaryOperation& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->compute2(left, opr);
	return AbstractSyntaxTree();
}

Napis UnaryOperation::toNapis() const
{
	bool inner_parentheses = m_leftChild->isNegationSignificant();
	bool outer_parentheses = (m_parent && m_parent->isNegationSignificant() &&
		this == m_parent->m_rightChild);
	return
		(outer_parentheses ? "(" : "") +
		m_opr +
		(inner_parentheses ? "(" : "") +
		m_leftChild->toNapis() +
		(inner_parentheses ? ")" : "") +
		(outer_parentheses ? ")" : "");
}
