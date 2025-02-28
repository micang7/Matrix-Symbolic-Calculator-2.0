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

	Matrix(const Matrix& original);

	Matrix(Matrix&& original) noexcept;

	~Matrix();

	TreeNode* clone() const override;
	TreeNode* shift() override;

	AbstractSyntaxTree compute1(const Napis& opr, TreeNode* right) const override;

	Napis toNapis() const override;
	Napis toNapisExpand() const override;
};