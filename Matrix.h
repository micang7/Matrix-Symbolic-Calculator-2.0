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

	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated = false) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated = false) override;

	Napis toNapis() const override;
	Napis toNapisExpand() const override;
};