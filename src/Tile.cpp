#include "Tile.h"

#include <iomanip>
#include <sstream>

#include "Config.h"
#include "TextureManager.h"
#include "Util.h"

Tile::Tile() :m_cost(0.0f)
{
	//TextureManager::Instance()->load("../Assets/textures/tile.png","Tile");

	//auto size = TextureManager::Instance()->getTextureSize("Tile");
	//setWidth(size.x);
	//setHeight(size.y);
	setWidth(Config::TILE_SIZE);
	setHeight(Config::TILE_SIZE);
	//setTileCost(0.0f);

}

Tile::~Tile()
= default;


void Tile::draw()
{
	//TextureManager::Instance()->draw("Tile", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
	Util::DrawRect(getTransform()->position, getWidth(), getHeight());

}

void Tile::update()
{
}

void Tile::clean()
{
}

Tile* Tile::getNeighbourTile(const NeighbourTile position)
{
	return m_neighbours[position];
}

void Tile::setNeighbourTile(const NeighbourTile position, Tile * tile)
{
	m_neighbours[position] = tile;
}

float Tile::getTileCost() const
{
	return m_cost;
}

void Tile::setTileCost(const float  cost)
{
	m_cost = cost;

	//format string up to 1 decimal place
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << cost;
	const std::string cost_string = stream.str();

	m_costLabel->setText(cost_string);
}

float Tile::getTileStatus() const
{
	return m_status;
}

void Tile::setTileStatus(const float status)
{
	m_status = status;

	//format string up to 1 decimal place
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << status;
	const std::string cost_string = stream.str();

	m_statusLabel->setText(cost_string);

	//switch (status)
	//{
	//case UNVISITED:
	//	m_statusLabel->setText("--");
	//	break;
	//case OPEN:
	//	m_statusLabel->setText("O");
	//	break;
	//case CLOSED:
	//	m_statusLabel->setText("C");
	//	break;
	//case IMPASSABLE:
	//	m_statusLabel->setText("I");
	//	break;
	//case GOAL:
	//	m_statusLabel->setText("G");
	//	break;
	//case START:
	//	m_statusLabel->setText("S");
	//	break;
	//}
}

void Tile::addLabels()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	m_costLabel = new Label("", "Consolas", 12);
	m_costLabel->getTransform()->position = getTransform()->position + offset + glm::vec2(0.0f, -6.0f);
	getParent()->addChild(m_costLabel,1);
	m_costLabel->setEnabled(false);

	m_statusLabel = new Label("--", "Consolas", 12);
	m_statusLabel->getTransform()->position = getTransform()->position + offset + glm::vec2(0.0f, 6.0f);
	getParent()->addChild(m_statusLabel,1);
	m_statusLabel->setEnabled(false);
}

void Tile::setLabelsEnabled(const bool state)
{
	m_costLabel->setEnabled(state);
	m_statusLabel->setEnabled(state);
}
