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
			padding + (subtreeRootNapis.getLen() + 2) * Napis(' '));

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
			padding + (subtreeRootNapis.getLen() + 2) * Napis(' '));
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
				std::cout << markColor << subtreeRootNapis << TREE_RESET_COLOR;
				if (!subtreeRoot->m_leftChild) std::cout << "\033[30m.\033[0m";
			}
			else std::cout << subtreeRootNapis;
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

AbstractSyntaxTree::AbstractSyntaxTree()
{
	m_root = nullptr;
}

AbstractSyntaxTree::AbstractSyntaxTree(const Napis& infixExpression)
{
#ifdef AST_VISUALISE
	std::cout << "Input expression: " << infixExpression << std::endl << std::endl;
#endif

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

#ifdef AST_VISUALISE
	printTree();
#endif

#ifdef AST_SIMPLIFY
	evaluate();
#ifdef AST_VISUALISE
	printTree();
	std::cout << "Result:" << std::endl << toNapisExpand() << std::endl << std::endl;
#endif
#endif
}

AbstractSyntaxTree::AbstractSyntaxTree(const TreeNode& operand)
{
	m_root = operand.clone();
}

AbstractSyntaxTree::AbstractSyntaxTree(const AbstractSyntaxTree& original)
{
	addSubtree(nullptr, original.m_root);
}

AbstractSyntaxTree& AbstractSyntaxTree::operator=(const AbstractSyntaxTree& original)
{
	if (this != &original) {
		removeSubtree(m_root);
		addSubtree(nullptr, original.m_root);
	}
	return *this;
}

AbstractSyntaxTree::AbstractSyntaxTree(AbstractSyntaxTree&& original) noexcept
{
	m_root = original.m_root;
	original.m_root = nullptr;
}

AbstractSyntaxTree& AbstractSyntaxTree::operator=(AbstractSyntaxTree&& original) noexcept
{
	if (this != &original) {
		removeSubtree(m_root);
		m_root = original.m_root;
		original.m_root = nullptr;
	}
	return *this;
}

AbstractSyntaxTree::~AbstractSyntaxTree()
{
	removeSubtree(m_root);
}

TreeNode* AbstractSyntaxTree::getRoot() const
{
	return m_root;
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

TreeNode* AbstractSyntaxTree::addChild(TreeNode* parent, TreeNode&& child)
{
	TreeNode* copy = child.shift();

	if (!parent) return m_root = copy;

	copy->m_parent = parent;

	if (parent->m_leftChild)
		parent->m_rightChild = copy;
	else
		parent->m_leftChild = copy;

	return copy;
}

void AbstractSyntaxTree::removeOneChildNode(TreeNode* node)
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

void AbstractSyntaxTree::swapChildren(TreeNode* parent)
{
	if (!parent) return;

	TreeNode* left = parent->m_leftChild;
	parent->m_leftChild = parent->m_rightChild;
	parent->m_rightChild = left;
}

TreeNode* AbstractSyntaxTree::reverseRelationship(TreeNode* parent, reverseRelationshipMode mode)
{
	switch (mode) {
	case LEFT_CHILD_LEFT_SUBTREE:
	{
		TreeNode* grandparent = parent->m_parent;
		TreeNode* left = parent->m_leftChild;

		left->m_parent = grandparent;
		if (grandparent) {
			if (grandparent->m_leftChild == parent)
				grandparent->m_leftChild = left;
			else
				grandparent->m_rightChild = left;
		}
		else m_root = left;

		parent->m_leftChild = left->m_rightChild;
		left->m_rightChild->m_parent = parent;

		left->m_rightChild = parent;
		parent->m_parent = left;

		return left;
	}
	case LEFT_CHILD_RIGHT_SUBTREE:
	{
		TreeNode* grandparent = parent->m_parent;
		TreeNode* left = parent->m_leftChild;

		left->m_parent = grandparent;
		if (grandparent) {
			if (grandparent->m_leftChild == parent)
				grandparent->m_leftChild = left;
			else
				grandparent->m_rightChild = left;
		}
		else m_root = left;

		parent->m_leftChild = left->m_leftChild;
		left->m_leftChild->m_parent = parent;

		left->m_leftChild = parent;
		parent->m_parent = left;

		return left;
	}
	case RIGHT_CHILD_LEFT_SUBTREE:
	{
		TreeNode* grandparent = parent->m_parent;
		TreeNode* right = parent->m_rightChild;

		right->m_parent = grandparent;
		if (grandparent) {
			if (grandparent->m_leftChild == parent)
				grandparent->m_leftChild = right;
			else
				grandparent->m_rightChild = right;
		}
		else m_root = right;

		parent->m_rightChild = right->m_rightChild;
		right->m_rightChild->m_parent = parent;

		right->m_rightChild = parent;
		parent->m_parent = right;

		return right;
	}
	case RIGHT_CHILD_RIGHT_SUBTREE:
	{
		TreeNode* grandparent = parent->m_parent;
		TreeNode* right = parent->m_rightChild;

		right->m_parent = grandparent;
		if (grandparent) {
			if (grandparent->m_leftChild == parent)
				grandparent->m_leftChild = right;
			else
				grandparent->m_rightChild = right;
		}
		else m_root = right;

		parent->m_rightChild = right->m_leftChild;
		right->m_leftChild->m_parent = parent;

		right->m_leftChild = parent;
		parent->m_parent = right;

		return right;
	}
	default:
		return parent;
	}
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

void AbstractSyntaxTree::pushSubtree(TreeNode* parent, TreeNode* subtreeRoot)
{
	if (!subtreeRoot) return;

	TreeNode* copy = subtreeRoot->shift();

	if (parent) {
		copy->m_parent = parent;

		if (parent->m_leftChild)
			parent->m_rightChild = copy;
		else
			parent->m_leftChild = copy;
	}
	else m_root = copy;

	if (subtreeRoot->m_leftChild) pushSubtree(copy, subtreeRoot->m_leftChild);
	if (subtreeRoot->m_rightChild) pushSubtree(copy, subtreeRoot->m_rightChild);
}

void AbstractSyntaxTree::replaceSubtree(TreeNode* oldSubtreeRoot, const AbstractSyntaxTree& newSubtree)
{
	TreeNode* newSubtreeRoot = newSubtree.getRoot();
	if (!newSubtreeRoot) return;

	TreeNode* parent = oldSubtreeRoot->m_parent;

	removeSubtree(oldSubtreeRoot);
	addSubtree(parent, newSubtreeRoot);
}

void AbstractSyntaxTree::replaceSubtree(TreeNode* oldSubtreeRoot, AbstractSyntaxTree&& newSubtree)
{
	TreeNode* newSubtreeRoot = newSubtree.getRoot();
	if (!newSubtreeRoot) return;

	TreeNode* parent = oldSubtreeRoot->m_parent;

	removeSubtree(oldSubtreeRoot);
	pushSubtree(parent, newSubtreeRoot);
}

void AbstractSyntaxTree::swapExclusiveSubtrees(TreeNode* subtree1Root, TreeNode* subtree2Root)
{
	if (!subtree1Root || !subtree2Root || subtree1Root == subtree2Root) return;

	TreeNode* parent1 = subtree1Root->m_parent;

	subtree1Root->m_parent = subtree2Root->m_parent;

	if (subtree2Root->m_parent) {
		if (subtree2Root->m_parent->m_leftChild == subtree2Root)
			subtree2Root->m_parent->m_leftChild = subtree1Root;
		else
			subtree2Root->m_parent->m_rightChild = subtree1Root;
	}
	else m_root = subtree1Root;

	subtree2Root->m_parent = parent1;

	if (parent1) {
		if (parent1->m_leftChild == subtree1Root)
			parent1->m_leftChild = subtree2Root;
		else
			parent1->m_rightChild = subtree2Root;
	}
	else m_root = subtree2Root;
}

void AbstractSyntaxTree::removeSubtree(TreeNode* subtreeRoot)
{
	if (!subtreeRoot) return;

	if (subtreeRoot->m_leftChild) removeSubtree(subtreeRoot->m_leftChild);
	if (subtreeRoot->m_rightChild) removeSubtree(subtreeRoot->m_rightChild);

	if (subtreeRoot->m_parent) {
		if (subtreeRoot->m_parent->m_leftChild == subtreeRoot)
			subtreeRoot->m_parent->m_leftChild = nullptr;
		else
			subtreeRoot->m_parent->m_rightChild = nullptr;
	}
	delete subtreeRoot;
}

void AbstractSyntaxTree::evaluate()
{
	m_root->arrange(*this);
	m_root->evaluate(*this);
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

AbstractSyntaxTree operator~(AbstractSyntaxTree&& ast)
{
	AbstractSyntaxTree result;

	if (ast.m_root->isNegation())
		result.pushSubtree(nullptr, ast.m_root->m_leftChild);
	else
		result.pushSubtree(result.addChild(nullptr, UnaryOperation('-')), ast.m_root);

	ast.m_root = nullptr;
	
	return result;
}

AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('+'));

	result.pushSubtree(result.m_root, ast1.m_root);
	result.pushSubtree(result.m_root, ast2.m_root);

	ast1.m_root = nullptr;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('-'));

	result.pushSubtree(result.m_root, ast1.m_root);
	result.pushSubtree(result.m_root, ast2.m_root);

	ast1.m_root = nullptr;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('*'));

	result.pushSubtree(result.m_root, ast1.m_root);
	result.pushSubtree(result.m_root, ast2.m_root);

	ast1.m_root = nullptr;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('/'));

	result.pushSubtree(result.m_root, ast1.m_root);
	result.pushSubtree(result.m_root, ast2.m_root);

	ast1.m_root = nullptr;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('^'));

	result.pushSubtree(result.m_root, ast1.m_root);
	result.pushSubtree(result.m_root, ast2.m_root);

	ast1.m_root = nullptr;
	ast2.m_root = nullptr;

	return result;
}
