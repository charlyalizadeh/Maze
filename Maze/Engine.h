#pragma once
#include "olcConsoleGameEngine.h"
#include <vector>
#include <stack>
#include <random>
#include <time.h>

struct Coord
{
	int x, y;
	Coord(int coordX = 0,int coordY = 0):x(coordX),y(coordY){}
};
bool operator==(const Coord& lhs, const Coord& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

class Engine : public olcConsoleGameEngine
{
public:
	Engine(int nbRow,int nbColumn,int dimCell = 3,short pathColor = FG_WHITE, short currentCellColor = FG_RED, short backgroundColor = FG_BLACK) : m_nbRow(nbRow), m_nbColumn(nbColumn), m_dimCell(dimCell), m_pathColor(pathColor),m_currentCellColor(currentCellColor),m_backgroundColor(backgroundColor)
	{
		srand(time(NULL));
		int width = nbColumn * (dimCell + 1) + 1;
		int height = nbRow * (dimCell + 1) + 1;
		m_matWallH = std::vector<bool>((m_nbRow - 1) * m_nbColumn, true);
		m_matWallV = std::vector<bool>(m_nbRow * (m_nbColumn - 1), true);
		m_colorCells = std::vector<short>(m_nbRow * m_nbColumn, backgroundColor);
		currentCoord = { rand() % m_nbColumn,rand() % m_nbRow };
		start = false;
		olcConsoleGameEngine::ConstructConsole(width, height, 8, 8);
	}
private:
	int m_dimCell, m_nbRow, m_nbColumn;
	std::vector<bool> m_matWallH, m_matWallV;
	std::vector<short> m_colorCells;
	std::stack<Coord> m_bcktStack;
	std::vector<Coord> m_coordChecked;
	Coord currentCoord;
	short m_pathColor, m_currentCellColor, m_backgroundColor;
	bool start;

	std::vector<Coord> possibleCoord(Coord c)
	{
		std::vector<Coord> validCoord;
		std::vector<Coord> tempCoord = { Coord{c.x - 1,c.y },Coord{c.x + 1,c.y},Coord{c.x,c.y - 1},Coord{c.x,c.y + 1} };
		for (auto c : tempCoord) {
			bool negativity = c.x >= 0 && c.y >= 0;
			bool limitSup = c.y < m_nbRow && c.x < m_nbColumn;
			bool notChecked = std::find(m_coordChecked.begin(), m_coordChecked.end(), c) == m_coordChecked.end();
			if (c.x >= 0 && c.y >= 0 && c.y < m_nbRow && c.x < m_nbColumn && std::find(m_coordChecked.begin(), m_coordChecked.end(), c) == m_coordChecked.end())
				validCoord.push_back(c);
		}
		return validCoord;
	}
	void backTracking(Coord startCoord)
	{
		Coord currentCoord = startCoord;
		m_coordChecked.push_back(currentCoord);
		m_bcktStack.push(currentCoord);
		while(!m_bcktStack.empty())
		{
			currentCoord = m_bcktStack.top();
			m_bcktStack.pop();
			std::vector<Coord> validCoord = possibleCoord(currentCoord);
			if(!validCoord.empty())
			{
				m_bcktStack.push(currentCoord);
				Coord tempCoord = validCoord[rand()%(validCoord.size())];
				removeWall(currentCoord, tempCoord);
				m_coordChecked.push_back(tempCoord);
				m_bcktStack.push(tempCoord);
			}
		}
	}
	
	void removeWall(Coord c1,Coord c2)
	{
		if(c1.x==c2.x)
		{
			int i = min(c1.y, c2.y);
			int j = c1.x;
			m_matWallH[i * (m_nbColumn) + j] = false;
		}
		else if(c1.y==c2.y)
		{
			int i = c1.y;
			int j = min(c1.x, c2.x);
			m_matWallV[i * (m_nbColumn - 1)+j] = false;
		}
	}
	void drawMaze()
	{
		int indexX = 1;
		int indexY = 1;
		for (int i = 0; i < m_nbRow; i++)
		{
			indexX = 1;
			for (int j = 0; j < m_nbColumn; j++)
			{
				Fill(indexX, indexY,indexX+m_dimCell,indexY+m_dimCell, 9608, m_colorCells[i * m_nbColumn + j]);
				indexX += m_dimCell+1;
			}
			indexY += m_dimCell+1;
		}

		indexX = m_dimCell + 1;
		indexY = 1;
		for (int i = 0; i < m_nbRow; i++)
		{
			indexX = m_dimCell + 1;
			for (int j = 0; j < m_nbColumn - 1; j++)
			{
				if (m_matWallV[i * (m_nbColumn-1) + j])
					DrawLine(indexX, indexY, indexX, indexY + m_dimCell - 1, 9608, m_backgroundColor);
				else
					DrawLine(indexX, indexY, indexX, indexY + m_dimCell - 1, 9608, m_pathColor);
				indexX += m_dimCell + 1;
			}
			indexY += m_dimCell + 1;
		}


		indexX = 1;
		indexY = m_dimCell + 1;
		for (int i = 0; i < m_nbRow - 1; i++)
		{
			indexX = 1;
			for (int j = 0; j < m_nbColumn; j++)
			{
				if (m_matWallH[i * m_nbColumn + j])
					DrawLine(indexX, indexY, indexX + m_dimCell-1, indexY, 9608, m_backgroundColor);
				else
					DrawLine(indexX, indexY, indexX + m_dimCell - 1, indexY, 9608, m_pathColor);
				indexX += m_dimCell + 1;
			}
			indexY += m_dimCell + 1;
		}
	}
	void backTrackIteration()
	{
		if (!m_bcktStack.empty())
		{
			currentCoord = m_bcktStack.top();
			m_colorCells[currentCoord.y * m_nbColumn + currentCoord.x] = m_pathColor;
			m_bcktStack.pop();
			std::vector<Coord> validCoord = possibleCoord(currentCoord);
			if (!validCoord.empty())
			{
				m_bcktStack.push(currentCoord);
				Coord tempCoord = validCoord[rand() % validCoord.size()];
				removeWall(currentCoord, tempCoord);
				m_coordChecked.push_back(tempCoord);
				m_colorCells[tempCoord.y * m_nbColumn + tempCoord.x] = m_currentCellColor;
				m_bcktStack.push(tempCoord);
			}
			else
			{
				m_colorCells[currentCoord.y * m_nbColumn + currentCoord.x] = m_currentCellColor;
			}
		}

		drawMaze();
		m_colorCells[currentCoord.y * m_nbColumn + currentCoord.x] = m_pathColor;
	}

protected:
	bool OnUserCreate() 
	{
		//backTracking({0,0});
		srand(time(NULL));
		m_coordChecked.push_back(currentCoord);
		m_bcktStack.push(currentCoord);
		return true;
	}
	bool OnUserUpdate(float fEslapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), 9608, m_backgroundColor);
		if (!start && GetKey(VK_SPACE).bPressed)
			start = true;
		if(start)
			backTrackIteration();
		
		return true;
	}
};


