#pragma once
#include "Component.h"

enum State
{
	IDLE,
	WANDER,
	TRACK
};

class WanderComponent;
class PathfindComponent;
class SeekComponent;

class StateMachineComponent : 
	public Component
{
public:
	void start() override;

	State getCurrentState() { return m_currentState; }
	void setCurrentState(State state) { m_currentState = state; }

	void update(float deltaTime) override;

private:
	State m_currentState;
	WanderComponent* m_wanderComponent;
	PathfindComponent* m_pathfindComponent;
	SeekComponent* m_seekComponent;
	float m_wanderForce;
	float m_seekRange = 300;
	float m_seekForce;
};

