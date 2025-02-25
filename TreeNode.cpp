#include "TreeNode.h"

#include "AbstractSyntaxTree.h"

TreeNode::TreeNode()
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

bool TreeNode::simplify(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
#endif
	return false;
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

bool TreeNode::isNegationSignificant() const
{
	return false;
}