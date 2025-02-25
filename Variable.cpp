#include "Variable.h"

#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "BinaryOperation.h"
#include "Constant.h"
#include "Matrix.h"

Variable::Variable(char name)
{
	m_name = name;
}

TreeNode* Variable::clone() const
{
	return new Variable(*this);
}

bool Variable::simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Variable::simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return opr->m_rightChild->simplify_R(ast, opr, this);
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool Variable::simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

Napis Variable::toNapis() const
{
	return m_name;
}
