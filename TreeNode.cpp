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
	return false;
}

Napis TreeNode::toNapisExpand() const
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