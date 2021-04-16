#include "State.h"

State::State():m_action(nullptr) {}

State::~State(){} 

std::vector<Transition*> State::getTransitions()
{
    return m_transitions;
}

Action* State::getAction()
{
    return m_action;
}

void State::addTransition(Transition* transition)
{
    m_transitions.push_back(transition);
}

void State::setAction(Action* action)
{
    m_action = action;
}