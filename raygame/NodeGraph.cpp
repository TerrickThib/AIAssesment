#include "NodeGraph.h"
#include <raylib.h>
#include <xlocinfo>

DynamicArray<NodeGraph::Node*> reconstructPath(NodeGraph::Node* start, NodeGraph::Node* end)
{
	DynamicArray<NodeGraph::Node*> path;
	NodeGraph::Node* currentNode = end;

	while (currentNode != start->previous)
	{
		currentNode->color = 0xFFFF00FF;
		path.insert(currentNode, 0);
		currentNode = currentNode->previous;
	}

	return path;
}

float diagonalDistance(NodeGraph::Node* node, NodeGraph::Node* goal, float cardinalCost, float diagonalCost)
{
	float displacementX = abs(node->position.x - goal->position.x);
	float displacementY = abs(node->position.y - goal->position.y);

	return cardinalCost * (displacementX + displacementY) + (diagonalCost - 2 * cardinalCost) * fmin(displacementX, displacementY);
}

void sortFScore(DynamicArray<NodeGraph::Node*>& nodes)
{
	NodeGraph::Node* key = nullptr;
	int j = 0;

	for (int i = 1; i < nodes.getLength(); i++) {
		key = nodes[i];
		j = i - 1;
		while (j >= 0 && nodes[j]->fScore > key->fScore) {
			nodes[j + 1] = nodes[j];
			j--;
		}

		nodes[j + 1] = key;
	}
}

DynamicArray<NodeGraph::Node*> NodeGraph::findPath(Node* start, Node* goal)
{
	//Makes all the scoes 0
	resetGraphScore(start);
	float hScore, gScore = 0;
	NodeGraph::Node* m_currentNode;

	DynamicArray<Node*> openList = DynamicArray<Node*>();
	DynamicArray<Node*> closedList = DynamicArray<Node*>();
	
	openList.addItem(start);
	m_currentNode = start;
	while (openList.getLength() != 0)
	{
		sortFScore(openList);

		m_currentNode = openList[0];
		if (m_currentNode == goal)
		{
			return reconstructPath(start, m_currentNode);
		}

		openList.remove(m_currentNode);
		closedList.addItem(m_currentNode);

		for (int i = 0; i < m_currentNode->edges.getLength(); i++)
		{
			if (m_currentNode->walkable == false)
				continue;
			
			if (!closedList.contains(m_currentNode->edges[i].target))//WHile neither list have the goal
			{
				gScore = m_currentNode->edges[i].cost + m_currentNode->gScore;//Makes the gscore equal to the current nodes gscore and current cost
				hScore = manhattanDistance(m_currentNode->edges[i].target, goal);
			}
			else
				continue;
			
			if (m_currentNode->edges[i].target->fScore > (gScore + hScore))
			{
				m_currentNode->edges[i].target->gScore = gScore;//Sets the current nodes gscore to be the curent nodes edges gscore
				m_currentNode->edges[i].target->hScore = hScore;//Sets the current nodes hscore to be the current nodes edges hscore
				m_currentNode->edges[i].target->fScore = gScore + hScore;//Sets the curent nodes fScore to be the gscore plus the hscore
				m_currentNode->edges[i].target->previous = m_currentNode;//Sets the current nodes targets previous to be current node
			}

			if (!openList.contains(m_currentNode->edges[i].target))
			{
				openList.addItem(m_currentNode->edges[i].target);
				m_currentNode->edges[i].target->gScore = gScore;//Sets the current nodes gscore to be the curent nodes edges gscore
				m_currentNode->edges[i].target->hScore = hScore;//Sets the current nodes hscore to be the current nodes edges hscore
				m_currentNode->edges[i].target->fScore = gScore + hScore;//Sets the curent nodes fScore to be the gscore plus the hscore
				m_currentNode->edges[i].target->previous = m_currentNode;//Sets the current nodes targets previous to be current node
			}
		}
		
	}
	return reconstructPath(start, goal);
}

void NodeGraph::drawGraph(Node* start)
{
	DynamicArray<Node*> drawnList = DynamicArray<Node*>();
	drawConnectedNodes(start, drawnList);
}

void NodeGraph::drawNode(Node* node, float size)
{
	static char buffer[10];
	sprintf_s(buffer, "%.0f", node->gScore);

	//Draw the circle for the outline
	DrawCircle((int)node->position.x, (int)node->position.y, size + 1, GetColor(node->color));
	//Draw the inner circle
	DrawCircle((int)node->position.x, (int)node->position.y, size, GetColor(node->color));
	//Draw the text
	DrawText(buffer, (int)node->position.x, (int)node->position.y, .8f, BLACK);
}

void NodeGraph::drawConnectedNodes(Node* node, DynamicArray<Node*>& drawnList)
{
	drawnList.addItem(node);
	if (node->walkable)
		drawNode(node, 8);

	for (int i = 0; i < node->edges.getLength(); i++)
	{
		Edge e = node->edges[i];
		////Draw the Edge
		//DrawLine((int)node->position.x, (int)node->position.y, (int)e.target->position.x, (int)e.target->position.y, WHITE);
		////Draw the cost
		//MathLibrary::Vector2 costPos = { (node->position.x + e.target->position.x) / 2, (node->position.y + e.target->position.y) / 2 };
		//static char buffer[10];
		//sprintf_s(buffer, "%.0f", e.cost);
		//DrawText(buffer, (int)costPos.x, (int)costPos.y, 16, RAYWHITE);
		//Draw the target node
		if (!drawnList.contains(e.target)) {
			drawConnectedNodes(e.target, drawnList);
		}
	}
}

void NodeGraph::resetGraphScore(Node * start)
{
	DynamicArray<Node*> resetList = DynamicArray<Node*>();
	resetConnectedNodes(start, resetList);
}

void NodeGraph::resetConnectedNodes(Node* node, DynamicArray<Node*>& resetList)
{
	resetList.addItem(node);

	for (int i = 0; i < node->edges.getLength(); i++)
	{
		node->edges[i].target->gScore = 0;
		node->edges[i].target->hScore = 0;
		node->edges[i].target->fScore = 0;
		node->edges[i].target->color = 0xFFFFFFFF;

		//Draw the target node
		if (!resetList.contains(node->edges[i].target)) {
			resetConnectedNodes(node->edges[i].target, resetList);
		}
	}
}

float NodeGraph::manhattanDistance(NodeGraph::Node* node, NodeGraph::Node* goal)
{
	return abs(node->position.x - goal->position.x) + abs(node->position.y - goal->position.y);
}