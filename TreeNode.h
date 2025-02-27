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
	virtual TreeNode* shift() = 0;

	virtual TreeNode* nextFreeNode();

	virtual void simplify(AbstractSyntaxTree& ast);

	virtual AbstractSyntaxTree evaluate(const Napis& opr) const;
	
	virtual AbstractSyntaxTree evaluate1(const Napis& opr, TreeNode* right) const = 0;
	virtual AbstractSyntaxTree evaluate2(const Constant& left, const Napis& opr) const;
	virtual AbstractSyntaxTree evaluate2(const Matrix& left, const Napis& opr) const;
	virtual AbstractSyntaxTree evaluate2(const Variable& left, const Napis& opr) const;
	virtual AbstractSyntaxTree evaluate2(const BinaryOperation& left, const Napis& opr) const;
	virtual AbstractSyntaxTree evaluate2(const UnaryOperation& left, const Napis& opr) const;

	virtual bool isNegation() const;
	
	virtual Napis toNapis() const = 0;
	virtual Napis toNapisExpand() const;
	virtual Napis toNapisJustNode() const;

	virtual bool lowerPrecedenceThan(const Napis& opr2) const;
	virtual bool equalPrecedenceAs(const Napis& opr2) const;
	virtual bool isNegationSignificant() const;
};