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

void UnaryOperation::simplify(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	if (m_opr == '-' && m_leftChild->isNegation()) {
		TreeNode* next = m_leftChild->m_leftChild;
		ast.removeOneChildNode(m_leftChild);
		ast.removeOneChildNode(this);
		next->simplify(ast);
	}
	else {
		m_leftChild->simplify(ast);
		ast.printTree(this);
		ast.replaceSubtree(this, m_leftChild->evaluate(m_opr));
	}
	
#else
	// double negation reduction
	if (m_opr == '-' && m_leftChild->isNegation()) {
		TreeNode* next = m_leftChild->m_leftChild;
		ast.removeOneChildNode(m_leftChild);
		ast.removeOneChildNode(this);
		next->simplify(ast);
		return;
	}

	m_leftChild->simplify(ast);
	ast.replaceSubtree(this, m_leftChild->evaluate(m_opr));
#endif
}

AbstractSyntaxTree UnaryOperation::evaluate(const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->evaluate(opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::evaluate1(const Napis& opr, TreeNode* right) const
{
	if (m_opr == '-') return m_leftChild->evaluate1(opr, right);
	return right->evaluate2(*this, opr);
}

AbstractSyntaxTree UnaryOperation::evaluate2(const Constant& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->evaluate2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::evaluate2(const Matrix& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->evaluate2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::evaluate2(const Variable& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->evaluate2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::evaluate2(const BinaryOperation& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->evaluate2(left, opr);
	return AbstractSyntaxTree();
}

AbstractSyntaxTree UnaryOperation::evaluate2(const UnaryOperation& left, const Napis& opr) const
{
	if (m_opr == '-') return m_leftChild->evaluate2(left, opr);
	return AbstractSyntaxTree();
}

bool UnaryOperation::isNegation() const
{
	return m_opr == '-';
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
