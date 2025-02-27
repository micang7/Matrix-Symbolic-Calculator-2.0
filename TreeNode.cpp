#include "TreeNode.h"

#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"

TreeNode::TreeNode()
{
	m_parent = nullptr;
	m_leftChild = nullptr;
	m_rightChild = nullptr;
}

TreeNode::TreeNode(const TreeNode& original)
{
	m_parent = nullptr;
	m_leftChild = nullptr;
	m_rightChild = nullptr;
}

TreeNode::TreeNode(TreeNode&& original) noexcept
{
	m_parent = nullptr;
	m_leftChild = nullptr;
	m_rightChild = nullptr;
}

TreeNode::~TreeNode()
{
	m_parent = nullptr;
	m_leftChild = nullptr;
	m_rightChild = nullptr;
}

TreeNode* TreeNode::nextFreeNode()
{
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool TreeNode::isNegation() const
{
	return false;
}

void TreeNode::reorganise(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
}

void TreeNode::evaluate(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
#endif
}

AbstractSyntaxTree TreeNode::compute(const Napis& opr) const
{
	return AbstractSyntaxTree();
}

AbstractSyntaxTree TreeNode::compute2(const Constant& left, const Napis& opr) const
{
	return AbstractSyntaxTree();
}

AbstractSyntaxTree TreeNode::compute2(const Matrix& left, const Napis& opr) const
{
	return AbstractSyntaxTree();
}

AbstractSyntaxTree TreeNode::compute2(const Variable& left, const Napis& opr) const
{
	return AbstractSyntaxTree();
}

AbstractSyntaxTree TreeNode::compute2(const BinaryOperation& left, const Napis& opr) const
{
	return AbstractSyntaxTree();
}

AbstractSyntaxTree TreeNode::compute2(const UnaryOperation& left, const Napis& opr) const
{
	return AbstractSyntaxTree();
}

Napis TreeNode::toNapisExpand() const
{
	return toNapis();
}

Napis TreeNode::toNapisJustNode() const
{
	return toNapis();
}

bool TreeNode::lowerPrecedenceThan(const Napis& opr2) const
{
	return false;
}

bool TreeNode::equalPrecedenceAs(const Napis& opr2) const
{
	return false;
}

bool TreeNode::isNegationSignificant() const
{
	return false;
}