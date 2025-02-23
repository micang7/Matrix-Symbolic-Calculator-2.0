#pragma once
#include "TreeNode.h"
#include "Napis.h"

class Operation : public TreeNode
{
protected:
	Napis m_opr;
public:
	Operation(const Napis& opr);

	const Napis& getOpr() const;
	void setOpr(Napis&& opr);
};

