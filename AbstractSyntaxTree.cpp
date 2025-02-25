#include "AbstractSyntaxTree.h"

#include "Parser.h"
#include "TreeNode.h"
#include "Matrix.h"
#include "Constant.h"
#include "Variable.h"
#include "UnaryOperation.h"
#include "BinaryOperation.h"
#include <iostream>

void AbstractSyntaxTree::printSubtree(TreeNode* subtreeRoot, TreeNode* markNode, const char* markColor, bool hasSibling, const Napis& padding) const
{
	if (!subtreeRoot) return;

	Napis subtreeRootNapis = subtreeRoot->toNapisJustNode();

	if (subtreeRoot == m_root) {
		printSubtree(subtreeRoot->m_rightChild, markNode, markColor, false,
			padding + (subtreeRootNapis.getLen() + 1) * Napis(' '));

		if (subtreeRoot == markNode) {
			std::cout << ' ' << markColor << subtreeRootNapis << TREE_RESET_COLOR;
			if (!subtreeRoot->m_leftChild) std::cout << "\033[30m.\033[0m";
		}
		else std::cout << ' ' << subtreeRootNapis;
		if (subtreeRoot->m_leftChild) {
			if (subtreeRoot->m_rightChild)
				std::cout << ' ' << TREE_COLOR << TREE_BPARENT << TREE_RESET_COLOR << std::endl;
			else std::cout << ' ' << TREE_COLOR << TREE_UPARENT << TREE_RESET_COLOR;
		}
		else std::cout << std::endl;

		printSubtree(subtreeRoot->m_leftChild, markNode, markColor, subtreeRoot->m_rightChild,
			padding + (subtreeRootNapis.getLen() + 1) * Napis(' '));
	}
	else {
		if (subtreeRoot->m_parent->m_leftChild == subtreeRoot) {
			printSubtree(subtreeRoot->m_rightChild, markNode, markColor, false,
				padding + TREE_COLOR + TREE_PIPE + TREE_RESET_COLOR +
				(subtreeRootNapis.getLen() + 2) * Napis(' '));

			if (hasSibling) std::cout << padding << TREE_COLOR << TREE_LEFT << TREE_RESET_COLOR;
			if (subtreeRoot == markNode) {
				std::cout << ' ' << markColor << subtreeRootNapis << TREE_RESET_COLOR;
				if (!subtreeRoot->m_leftChild) std::cout << "\033[30m.\033[0m";
			}
			else std::cout << ' ' << subtreeRootNapis;
			if (subtreeRoot->m_leftChild) {
				if (subtreeRoot->m_rightChild)
					std::cout << ' ' << TREE_COLOR << TREE_BPARENT << TREE_RESET_COLOR << std::endl;
				else std::cout << ' ' << TREE_COLOR << TREE_UPARENT << TREE_RESET_COLOR;
			}
			else std::cout << std::endl;
			
			printSubtree(subtreeRoot->m_leftChild, markNode, markColor, subtreeRoot->m_rightChild,
				padding + (subtreeRootNapis.getLen() + 3) * Napis(' '));
		}
		else {
			printSubtree(subtreeRoot->m_rightChild, markNode, markColor, false,
				padding + (subtreeRootNapis.getLen() + 3) * Napis(' '));

			std::cout << padding << TREE_COLOR << TREE_RIGHT << TREE_RESET_COLOR << ' ';
			if (subtreeRoot == markNode) {
				std::cout << ' ' << markColor << subtreeRootNapis << TREE_RESET_COLOR;
				if (!subtreeRoot->m_leftChild) std::cout << "\033[30m.\033[0m";
			}
			else std::cout << ' ' << subtreeRootNapis;
			if (subtreeRoot->m_leftChild) {
				if (subtreeRoot->m_rightChild)
					std::cout << ' ' << TREE_COLOR << TREE_BPARENT << TREE_RESET_COLOR << std::endl;
				else std::cout << ' ' << TREE_COLOR << TREE_UPARENT << TREE_RESET_COLOR;
			}
			else std::cout << std::endl;

			printSubtree(subtreeRoot->m_leftChild, markNode, markColor, subtreeRoot->m_rightChild,
				padding + TREE_COLOR + TREE_PIPE + TREE_RESET_COLOR +
				(subtreeRootNapis.getLen() + 2) * Napis(' '));
		}
	}
}

AbstractSyntaxTree::AbstractSyntaxTree(const Napis& infixExpression)
{
	if (!CYK(infixExpression)) throw std::invalid_argument("Error: AST: Invalid expression!");

	Napis expression = infixToPrefix(infixExpression);

	m_root = nullptr;
	TreeNode* current = m_root;
	Napis constant;
	int i = 0, n = expression.getLen();

	while (i < n) {
		char chr = expression[i];
		if (chr == '{') {
			int startIndex = i, balance = 1, rows = 0, cols = 0, cols_in_row = 0;
			Napis matrix;

			while (balance) {
				i++;
				chr = expression[i];

				if (chr == '{') {
					if (balance > 1) matrix += '{';
					balance++;
				}
				else if (chr == '}' || chr == ',') {
					if (balance == 2) {
						cols_in_row++;
						matrix += ';';
						if (chr == '}') {
							if (cols == 0) cols = cols_in_row;
							else if (cols != cols_in_row) throw std::invalid_argument("Error: AST: Matrix is not rectangular!");
							cols_in_row = 0;
							rows++;
						}
					}
					else if (balance > 2) matrix += chr;
					if (chr == '}') balance--;
				}
				else matrix += chr;
			}
			current = addChild(current, Matrix(rows, cols, matrix))->nextFreeNode();
			i++;
		}
		else if (std::isdigit(chr)) {
			constant += chr;
		}
		else if (chr == ' ') {
			current = addChild(current, Constant(constant.toInt()))->nextFreeNode();
			constant = "";
		}
		else if (std::isalpha(chr)) {
			current = addChild(current, Variable(chr))->nextFreeNode();
			i++;
		}
		else if (chr == '~') {
			current = addChild(current, UnaryOperation("-"))->nextFreeNode();
			i++;
		}
		else {
			current = addChild(current, BinaryOperation(chr))->nextFreeNode();
			i++;
		}
		i++;
	}
	if (constant.getLen() > 0) addChild(current, Constant(constant.toInt()));

#ifdef AST_SIMPLIFY
	printTree();
	simplify();
	printTree();
#else
	printTree();
#endif
}

AbstractSyntaxTree::~AbstractSyntaxTree()
{
	removeSubtree(m_root);
}

TreeNode* AbstractSyntaxTree::addChild(TreeNode* parent, const TreeNode& child)
{
	TreeNode* copy = child.clone();

	if (!parent) return m_root = copy;

	copy->m_parent = parent;

	if (parent->m_leftChild)
		parent->m_rightChild = copy;
	else
		parent->m_leftChild = copy;

	return copy;
}

void AbstractSyntaxTree::addParent(TreeNode* node, const TreeNode& parent)
{
	if (!node) return;

	TreeNode* copy = parent.clone();

	if (node->m_parent) {
		copy->m_parent = node->m_parent;
		if (node->m_parent->m_leftChild == node)
			node->m_parent->m_leftChild = copy;
		else
			node->m_parent->m_rightChild = copy;
	}
	else m_root = copy;

	node->m_parent = copy;
	copy->m_leftChild = node;
}

void AbstractSyntaxTree::addSubtree(TreeNode* parent, const TreeNode* subtreeRoot)
{
	if (!subtreeRoot) return;

	TreeNode* copy = subtreeRoot->clone();

	if (parent) {
		copy->m_parent = parent;
		if (parent->m_leftChild)
			parent->m_rightChild = copy;
		else
			parent->m_leftChild = copy;
	}
	else m_root = copy;

	if (subtreeRoot->m_leftChild) addSubtree(copy, subtreeRoot->m_leftChild);
	if (subtreeRoot->m_rightChild) addSubtree(copy, subtreeRoot->m_rightChild);
}

void AbstractSyntaxTree::removeUnaryNode(TreeNode* node)
{
	if (!node) return;

	node->m_leftChild->m_parent = node->m_parent;

	if (node->m_parent) {
		if (node->m_parent->m_leftChild == node)
			node->m_parent->m_leftChild = node->m_leftChild;
		else
			node->m_parent->m_rightChild = node->m_leftChild;
	}
	else m_root = node->m_leftChild;

	delete node;
}

void AbstractSyntaxTree::removeBinaryNodeAndLeftSubtree(TreeNode* node)
{
	if (!node) return;

	node->m_rightChild->m_parent = node->m_parent;;

	if (node->m_parent) {
		if (node->m_parent->m_leftChild == node)
			node->m_parent->m_leftChild = node->m_rightChild;
		else
			node->m_parent->m_rightChild = node->m_rightChild;
	}
	else m_root = node->m_rightChild;

	removeSubtree(node->m_leftChild);
	delete node;
}

void AbstractSyntaxTree::removeBinaryNodeAndRightSubtree(TreeNode* node)
{
	if (!node) return;

	node->m_leftChild->m_parent = node->m_parent;;

	if (node->m_parent) {
		if (node->m_parent->m_leftChild == node)
			node->m_parent->m_leftChild = node->m_leftChild;
		else
			node->m_parent->m_rightChild = node->m_leftChild;
	}
	else m_root = node->m_leftChild;

	removeSubtree(node->m_rightChild);
	delete node;
}

void AbstractSyntaxTree::removeSubtree(TreeNode* subtreeRoot)
{
	if (!subtreeRoot) return;

	if (subtreeRoot->m_leftChild) removeSubtree(subtreeRoot->m_leftChild);
	if (subtreeRoot->m_rightChild) removeSubtree(subtreeRoot->m_rightChild);

	if (subtreeRoot->m_parent) {
		if (subtreeRoot->m_parent->m_leftChild == subtreeRoot)
			subtreeRoot->m_parent->m_leftChild = nullptr;
		else if (subtreeRoot->m_parent->m_rightChild == subtreeRoot)
			subtreeRoot->m_parent->m_rightChild = nullptr;
	}
	delete subtreeRoot;
}

void AbstractSyntaxTree::swapSubtrees(TreeNode* subtree1Root, TreeNode* subtree2Root)
{
	if (!subtree1Root || !subtree2Root || subtree1Root == subtree2Root) return;

	TreeNode* temp = subtree2Root->m_parent;

	subtree2Root->m_parent = subtree1Root->m_parent;

	if (subtree1Root->m_parent) {
		if (subtree1Root->m_parent->m_leftChild == subtree1Root)
			subtree1Root->m_parent->m_leftChild = subtree2Root;
		else
			subtree1Root->m_parent->m_rightChild = subtree2Root;
	}
	else m_root = subtree2Root;

	subtree1Root->m_parent = temp;

	if (temp) {
		if (temp->m_leftChild == subtree2Root)
			temp->m_leftChild = subtree1Root;
		else
			temp->m_rightChild = subtree1Root;
	}
	else m_root = subtree1Root;
}

void AbstractSyntaxTree::simplify()
{
	m_root->simplify(*this);
}

void AbstractSyntaxTree::printTree(TreeNode* markNode, const char* markColor, TreeNode* root) const
{
	printSubtree(m_root, markNode, markColor);
	std::cout << std::endl;
}

Napis AbstractSyntaxTree::toNapis()
{
	return m_root->toNapis();
}

Napis AbstractSyntaxTree::toNapisExpand()
{
	return m_root->toNapisExpand();
}
