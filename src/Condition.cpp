#include "Condition.h"

Condition::Condition(bool condition)
{
    SetCondition(condition);
}

Condition::~Condition(){}

bool Condition::Test()
{
    return m_condition;
}

void Condition::SetCondition(bool condition)
{
    m_condition = condition;
}
