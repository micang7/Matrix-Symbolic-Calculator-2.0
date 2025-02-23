#include "Operation.h"

#include <iostream>

Operation::Operation(const Napis& opr) : m_opr(opr) {}

const Napis& Operation::getOpr() const
{
	return m_opr;
}

void Operation::setOpr(Napis&& opr)
{
	m_opr = std::move(opr);
}
