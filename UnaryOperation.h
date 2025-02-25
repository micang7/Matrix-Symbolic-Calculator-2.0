#pragma once
#include "Operation.h"

class UnaryOperation : public Operation
{
public:
	UnaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	bool simplify(AbstractSyntaxTree& ast) override;
	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	bool isNegation() const override;

	Napis toNapis() const override;
};

