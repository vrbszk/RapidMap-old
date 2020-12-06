#pragma once
#include <memory>
#include <stack>

class StateMachine;

#include "State.hpp"

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void AddState(StatePtr newState, bool isReplasing = true);
	void RemoveState();
	bool IsEmpty();

	void ProcessStateChanges();

	StatePtr& GetActiveState();

private:
	std::stack<StatePtr> states;
	StatePtr newState;

	bool isRemoving;
	bool isAdding;
	bool isReplacing;
};