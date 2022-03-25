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
	//Inalizes the wander component wander force and pathfind component
	Component::start();
	m_wanderComponent = getOwner()->getComponent<WanderComponent>();
	m_wanderForce = m_wanderComponent->getSteeringForce();
	m_pathfindComponent = getOwner()->getComponent<PathfindComponent>();
	
	m_currentState = IDLE;//Declares the State at the start of the programe
}

void StateMachineComponent::update(float deltaTime)
{
	Component::update(deltaTime);

	MathLibrary::Vector2 targetPos = m_pathfindComponent->getTarget()->getTransform()->getWorldPosition();
	MathLibrary::Vector2 ownerPos = getOwner()->getTransform()->getWorldPosition();
	float distanceFromTarget = (targetPos - ownerPos).getMagnitude();//Gets the distance from the target 

	bool targetInRange = distanceFromTarget <= m_seekRange;//Gets a bull by doing operation on same line

	switch (m_currentState)
	{//Begins on idle sets steering force to 0 and pathfind is enabled
	case IDLE:
		m_pathfindComponent->setEnabled(false);
		m_wanderComponent->setSteeringForce(0);

		if (!targetInRange)
			setCurrentState(WANDER);

		if (targetInRange)
			setCurrentState(TRACK);

		break;
		//Sets to wander is target is in ranged begin to Track
	case WANDER:
		m_wanderComponent->setSteeringForce(m_wanderForce);
		m_pathfindComponent->setEnabled(false);

		if (targetInRange)//If target is in range begin to track
			setCurrentState(TRACK);

		break;
		//Sets the State to Track and enables pathfinding 
	case TRACK:
		m_wanderComponent->setSteeringForce(0);
		m_pathfindComponent->setEnabled(true);

		if (!targetInRange)//If target isnt in range begin to wander
			setCurrentState(WANDER);

		break;
	}
}

