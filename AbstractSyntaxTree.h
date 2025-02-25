#pragma once
#include "AST_Settings.h"
#include "Napis.h"

class TreeNode;

class AbstractSyntaxTree
{
	TreeNode* m_root;

	void printSubtree(TreeNode* subtreeRoot, TreeNode* markNode = nullptr, const char* markColor = "", bool hasSibling = true, const Napis& padding = "") const;
public:
	AbstractSyntaxTree(const Napis& infixExpression);

	AbstractSyntaxTree();
	AbstractSyntaxTree(const TreeNode& operand);

	AbstractSyntaxTree(const AbstractSyntaxTree& original);
	AbstractSyntaxTree& operator=(const AbstractSyntaxTree& original);

	AbstractSyntaxTree(AbstractSyntaxTree&& original) noexcept;
	AbstractSyntaxTree& operator=(AbstractSyntaxTree&& original) noexcept;

	~AbstractSyntaxTree();

	TreeNode* getRoot() const;

	TreeNode* addChild(TreeNode* parent, const TreeNode& child);
	void addParent(TreeNode* node, const TreeNode& parent);
	void addSubtree(TreeNode* parent, const TreeNode* subtreeRoot);

	void removeUnaryNode(TreeNode* node);
	void removeBinaryNodeAndLeftSubtree(TreeNode* node);
	void removeBinaryNodeAndRightSubtree(TreeNode* node);
	TreeNode* removeSubtree(TreeNode* subtreeRoot);

	void swapSubtrees(TreeNode* subtree1Root, TreeNode* subtree2Root);

	void simplify();

	friend AbstractSyntaxTree operator~(AbstractSyntaxTree&& ast);
	friend AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);
	friend AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2);

	void printTree(TreeNode* markNode = nullptr, const char* markColor = TREE_NODE_COLOR1, TreeNode* root = nullptr) const;

	Napis toNapis();
	Napis toNapisExpand();
};