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
	std::cout << "Input expression: " << infixExpression << std::endl << std::endl;

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
	std::cout << "Result:" << std::endl << toNapisExpand() << std::endl << std::endl;
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
	if (!parent) return m_root = &child;

	child.m_parent = parent;

	if (parent->m_leftChild)
		parent->m_rightChild = &child;
	else
		parent->m_leftChild = &child;

	return &child;
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
	if (parent) {
		newSubtreeRoot->m_parent = parent;

		if (parent->m_leftChild)
			parent->m_rightChild = newSubtreeRoot;
		else
			parent->m_leftChild = newSubtreeRoot;
	}
	else m_root = newSubtreeRoot;
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

AbstractSyntaxTree operator~(AbstractSyntaxTree&& ast)
{
	AbstractSyntaxTree result(UnaryOperation('-'));

	result.m_root->m_leftChild = ast.m_root;
	ast.m_root->m_parent = result.m_root;
	ast.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator+(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('+'));

	result.m_root->m_leftChild = ast1.m_root;
	ast1.m_root->m_parent = result.m_root;
	ast1.m_root = nullptr;

	result.m_root->m_rightChild = ast2.m_root;
	ast2.m_root->m_parent = result.m_root;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator-(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('-'));

	result.m_root->m_leftChild = ast1.m_root;
	ast1.m_root->m_parent = result.m_root;
	ast1.m_root = nullptr;

	result.m_root->m_rightChild = ast2.m_root;
	ast2.m_root->m_parent = result.m_root;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator*(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('*'));

	result.m_root->m_leftChild = ast1.m_root;
	ast1.m_root->m_parent = result.m_root;
	ast1.m_root = nullptr;

	result.m_root->m_rightChild = ast2.m_root;
	ast2.m_root->m_parent = result.m_root;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator/(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('/'));

	result.m_root->m_leftChild = ast1.m_root;
	ast1.m_root->m_parent = result.m_root;
	ast1.m_root = nullptr;

	result.m_root->m_rightChild = ast2.m_root;
	ast2.m_root->m_parent = result.m_root;
	ast2.m_root = nullptr;

	return result;
}

AbstractSyntaxTree operator^(AbstractSyntaxTree&& ast1, AbstractSyntaxTree&& ast2)
{
	AbstractSyntaxTree result(BinaryOperation('^'));

	result.m_root->m_leftChild = ast1.m_root;
	ast1.m_root->m_parent = result.m_root;
	ast1.m_root = nullptr;

	result.m_root->m_rightChild = ast2.m_root;
	ast2.m_root->m_parent = result.m_root;
	ast2.m_root = nullptr;

	return result;
}