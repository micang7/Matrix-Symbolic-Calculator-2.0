#include "Matrix.h"

#include "Parser.h"
#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "BinaryOperation.h"
#include "Constant.h"
#include "Variable.h"
#include <iostream>

Matrix::Matrix(const Napis& expression)
{
	if (!CYK(expression)) throw std::invalid_argument("Error: Matrix: Invalid expression!");

	m_rows = m_cols = 0;
	int i = 0, balance = 1, cols_in_row = 0;
	Napis matrix;

	while (balance) {
		i++;
		char chr = expression[i];

		if (chr == '{') {
			if (balance > 1) matrix += '{';
			balance++;
		}
		else if (chr == '}' || chr == ',') {
			if (balance == 2) {
				cols_in_row++;
				matrix += ';';
				if (chr == '}') {
					if (m_cols == 0) m_cols = cols_in_row;
					else if (m_cols != cols_in_row) throw std::invalid_argument("Error: Matrix: Matrix is not rectangular!");
					cols_in_row = 0;
					m_rows++;
				}
			}
			else if (balance > 2) matrix += chr;
			if (chr == '}') balance--;
		}
		else matrix += chr;
	}
	i = 0;
	Napis element;

	m_matrix = new AbstractSyntaxTree * *[m_rows];
	for (int r = 0; r < m_rows; r++) {
		m_matrix[r] = new AbstractSyntaxTree * [m_cols];
		for (int c = 0; c < m_cols; c++) {
			element = "";
			while (matrix[i] != ';') {
				element += matrix[i];
				i++;
			}
			i++;
			m_matrix[r][c] = new AbstractSyntaxTree(element);
		}
	}
}

Matrix::Matrix(int rows, int cols, const Napis& expression)
{
	m_rows = rows;
	m_cols = cols;

	int i = 0;
	Napis element;

	m_matrix = new AbstractSyntaxTree * *[m_rows];
	for (int r = 0; r < m_rows; r++) {
		m_matrix[r] = new AbstractSyntaxTree * [m_cols];
		for (int c = 0; c < m_cols; c++) {
			element = "";
			while (expression[i] != ';') {
				element += expression[i];
				i++;
			}
			i++;
			m_matrix[r][c] = new AbstractSyntaxTree(element);
		}
	}
}

TreeNode* Matrix::clone() const
{
	return new Matrix(*this);
}

bool Matrix::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Matrix::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return opr->m_rightChild->simplify_R(ast, opr, this);
}

bool Matrix::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Matrix::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Matrix::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Matrix::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Matrix::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

Napis Matrix::toNapis() const
{
	if (m_rows == 0 || m_cols == 0) return "";

	Napis expression = "{";
	for (int i = 0; i < m_rows - 1; i++) {
		expression += "{";
		for (int j = 0; j < m_cols - 1; j++) {
			expression += m_matrix[i][j]->toNapis();
			expression += ",";
		}
		expression += m_matrix[i][m_cols - 1]->toNapis();
		expression += "},";
	}
	expression += "{";
	for (int j = 0; j < m_cols - 1; j++) {
		expression += m_matrix[m_rows - 1][j]->toNapis();
		expression += ",";
	}
	expression += m_matrix[m_rows - 1][m_cols - 1]->toNapis();
	expression += "}}";

	return expression;
}

Napis Matrix::toNapisExpand() const
{
	if (m_rows == 0 || m_cols == 0) return "";

	int* maxWidth = new int[m_cols] {0};
	int len;

	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			len = m_matrix[i][j]->toNapis().getLen();
			if (len > maxWidth[j]) maxWidth[j] = len;
		}
	}
	Napis element;
	Napis separator = "  ";
	Napis expression = "";
	for (int i = 0; i < m_rows - 1; i++) {
		for (int j = 0; j < m_cols - 1; j++) {
			element = m_matrix[i][j]->toNapis();
			expression += element;
			for (int k = 0; k < maxWidth[j] - element.getLen(); k++)
				expression += " ";
			expression += separator;
		}
		expression += m_matrix[i][m_cols - 1]->toNapis();
		expression += '\n';
	}
	for (int j = 0; j < m_cols - 1; j++) {
		element = m_matrix[m_rows - 1][j]->toNapis();
		expression += element;
		for (int k = 0; k < maxWidth[j] - element.getLen(); k++)
			expression += " ";
		expression += separator;
	}
	expression += m_matrix[m_rows - 1][m_cols - 1]->toNapis();

	delete[] maxWidth;

	return expression;
}
