#pragma once
#ifndef  _TILE_
#define  _TILE_

#include "Label.h"
#include "NavigationObject.h"
#include "NeighbourTile.h"
#include "TileStatus.h"

class Tile : public NavigationObject
{
public:
	//Constructor
	Tile();

	//Destructor
	~Tile();
	//Life-Cycle Functions
	void draw() override;
	void update() override;
	void clean() override;

	Tile* getNeighbourTile(NeighbourTile position);
	void setNeighbourTile(NeighbourTile position, Tile* tile);

	float getTileCost() const;
	void setTileCost(float cost);

	float getTileStatus()const;
	void setTileStatus(float status);

	void addLabels();
	void setLabelsEnabled(bool state);

	bool isObstacle = false;

	//glm::vec2 getGridPosition() const;
	//void setGridPosition(float col, float row);

private:
	float m_cost;
	float m_status;
	Label* m_costLabel;
	Label* m_statusLabel;

	Tile* m_neighbours[NUM_OF_NEIGHBOUR_TILES];

	//glm::vec2 m_gridPosition;
};
#endif