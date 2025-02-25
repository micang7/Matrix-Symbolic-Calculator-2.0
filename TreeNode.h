#pragma once
#include "Napis.h"

class AbstractSyntaxTree;
class UnaryOperation;
class BinaryOperation;
class Constant;
class Matrix;
class Variable;

class TreeNode
{
public:
	TreeNode* m_parent;
	TreeNode* m_leftChild;
	TreeNode* m_rightChild;

	TreeNode();
	TreeNode(const TreeNode& original);
	TreeNode(TreeNode&& original) noexcept;
	virtual ~TreeNode();

	virtual TreeNode* clone() const = 0;
	virtual TreeNode* nextFreeNode();

	virtual bool simplify(AbstractSyntaxTree& ast);
	virtual bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr) = 0;
	virtual bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) = 0;

	virtual bool isNegation() const;

	AbstractSyntaxTree operator~() const;
	
	virtual Napis toNapis() const = 0;
	virtual Napis toNapisExpand() const;
	virtual Napis toNapisJustNode() const;

	virtual bool lowerPrecedenceThan(const Napis& opr2) const;
	virtual bool equalPrecedenceAs(const Napis& opr2) const;
	virtual bool isNegationSignificant() const;
};