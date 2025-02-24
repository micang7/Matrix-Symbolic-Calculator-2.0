#pragma once
#include "Napis.h"
#include <string>

class TreeNode;

class AbstractSyntaxTree
{
	TreeNode* m_root;

	void print_nodes(const std::string& padding, const std::string& edge, TreeNode* node, bool has_left_sibling) const;
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

	void printTree(TreeNode* root = nullptr) const;

	Napis toNapis();
	Napis toNapisExpand();
};