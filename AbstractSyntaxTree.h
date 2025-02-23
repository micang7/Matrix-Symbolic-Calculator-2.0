#pragma once
#include "Napis.h"

class TreeNode;

class AbstractSyntaxTree
{
	TreeNode* m_root;
public:
	AbstractSyntaxTree(const Napis& infixExpression);

	~AbstractSyntaxTree();

	TreeNode* addChild(TreeNode* parent, const TreeNode& child);
	void addParent(TreeNode* node, const TreeNode& parent);
	void addSubtree(TreeNode* parent, const TreeNode* subtreeRoot);

	void removeUnaryNode(TreeNode* node);
	void removeBinaryNodeAndLeftSubtree(TreeNode* node);
	void removeBinaryNodeAndRightSubtree(TreeNode* node);
	void removeSubtree(TreeNode* subtreeRoot);

	void swapSubtrees(TreeNode* subtree1Root, TreeNode* subtree2Root);

	void simplify();

	Napis toNapis();
	Napis toNapisExpand();
};