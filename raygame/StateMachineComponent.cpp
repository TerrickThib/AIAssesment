#include "StateMachineComponent.h"
#include "Actor.h"
#include "PathfindComponent.h"
#include "WanderComponent.h"
#include "Transform2D.h"

void StateMachineComponent::start()
{
	Component::start();

	m_wanderComponent = getOwner()->getComponent<WanderComponent>();
	m_pathfindComponent = getOwner()->getComponent<PathfindComponent>();
}

void StateMachineComponent::update(float deltaTime)
{
	switch (m_currentState)
	{
	case IDLE:
		
	}
}

