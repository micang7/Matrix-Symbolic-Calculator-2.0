#pragma once
#include "TreeNode.h"
#include "Napis.h"

class AbstractSyntaxTree;

class Matrix : public TreeNode
{
	int m_rows;
	int m_cols;
	AbstractSyntaxTree*** m_matrix;
public:
	Matrix(const Napis& expression);
	Matrix(int rows, int cols, const Napis& expression);

	TreeNode* clone() const override;

	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	Napis toNapis() const override;
	Napis toNapisExpand() const override;
};