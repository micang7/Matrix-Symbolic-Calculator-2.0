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
	virtual ~TreeNode();

	virtual TreeNode* clone() const = 0;
	virtual TreeNode* nextFreeNode();

	virtual bool simplify(AbstractSyntaxTree& ast);
	virtual bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated = false) = 0;
	virtual bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated = false) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated = false) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated = false) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated = false) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated = false) = 0;
	virtual bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated = false) = 0;

	virtual Napis toNapis() const = 0;
	virtual Napis toNapisExpand() const;
	virtual Napis toNapisJustNode() const;

	virtual bool lowerPrecedenceThan(const Napis& opr2) const;
	virtual bool isNegationSignificant() const;
};