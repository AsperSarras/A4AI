#include "StateMachine.h"
#include "Transition.h"

StateMachine::StateMachine(){}

StateMachine::~StateMachine(){}

State* StateMachine::getCurrentState() const
{
    return m_currentState;
}

void StateMachine::setCurrentState(State* current_state)
{
    m_currentState = current_state;
}

void StateMachine::Update()
{
    /*Transition* triggeredTransition = nullptr;*/
    for (auto transition : getCurrentState()->getTransitions())
    {
        if (transition != nullptr && transition->getCondition()->Test()) // If condition of transition is true...
        {
            setCurrentState(transition->getTargetState());
            break;
        }
    }
    /*if (triggeredTransition != nullptr)
        setCurrentState(triggeredTransition->getTargetState());*/
    getCurrentState()->getAction()->Execute();
}
