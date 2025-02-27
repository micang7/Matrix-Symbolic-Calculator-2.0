#pragma once
#include "Operation.h"

class UnaryOperation : public Operation
{
public:
	UnaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	bool simplify(AbstractSyntaxTree& ast) override;

	bool evaluate(AbstractSyntaxTree& ast, UnaryOperation* opr) override;

	bool evaluate_part1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	bool isNegation() const override;

	Napis toNapis() const override;
};

