#include "StateMachineComponent.h"
#include "Actor.h"
#include "PathfindComponent.h"
#include "SeekComponent.h"
#include "WanderComponent.h"
#include "Transform2D.h"
#include "Ghost.h"
#include "Player.h"
#include "MazeScene.h"

void StateMachineComponent::start()
{
	Component::start();

	m_wanderComponent = getOwner()->getComponent<WanderComponent>();
	m_wanderForce = m_wanderComponent->getSteeringForce();

	m_pathfindComponent = getOwner()->getComponent<PathfindComponent>();
	
	m_currentState = IDLE;
}

void StateMachineComponent::update(float deltaTime)
{
	Component::update(deltaTime);

	MathLibrary::Vector2 targetPos = m_seekComponent->getTarget()->getTransform()->getWorldPosition();
	MathLibrary::Vector2 ownerPos = getOwner()->getTransform()->getWorldPosition();
	float distanceFromTarget = (targetPos - ownerPos).getMagnitude();//Gets the distance from the target 

	bool targetInRange = distanceFromTarget <= m_seekRange;//Gets a bull by doing operation on same line

	switch (m_currentState)
	{
	case IDLE:
		m_pathfindComponent->setEnabled(false);
		m_wanderComponent->setSteeringForce(0);

		if (!targetInRange)
			setCurrentState(WANDER);

		break;
	case WANDER:
		m_wanderComponent->setSteeringForce(m_wanderForce);
		m_pathfindComponent->setEnabled(false);

		if (targetInRange)
			setCurrentState(TRACK);

		break;
	case TRACK:
		m_wanderComponent->setSteeringForce(0);
		m_pathfindComponent->setEnabled(true);
		m_pathfindComponent->setTarget();

		if (!targetInRange)
			setCurrentState(WANDER);

		break;
	}
}

