#pragma once
#include <vector>
#include <stack>
#include <random>
#include <time.h>

struct Coord
{
	int x, y;
	Coord(int coordX = 0, int coordY = 0) :x(coordX), y(coordY) {}
};
bool operator==(const Coord& lhs, const Coord& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

class Maze
{
private:
	int m_nbRow, m_nbColumn;
	std::vector<bool> m_matWallH, m_matWallV;
	std::stack<Coord> m_bcktStack;
	std::vector<Coord> m_coordChecked;
	std::vector<bool> m_matCell;//This is a duplication of the data that are in m_coordChecked, I use it because checking if (i,j) are in m_coordChecked has a complexity of O(N) whereas checking if m_path[n] = true has a complexity of O(1)
	Coord m_currentCoord;
	Coord m_frontCoord;



	std::vector<Coord> possibleCoord(Coord c)
	{
		std::vector<Coord> validCoord;
		std::vector<Coord> tempCoord = { Coord{c.x - 1,c.y },Coord{c.x + 1,c.y},Coord{c.x,c.y - 1},Coord{c.x,c.y + 1} };
		for (auto c : tempCoord) {
			if (c.x >= 0 && c.y >= 0 && c.y < m_nbRow && c.x < m_nbColumn && std::find(m_coordChecked.begin(), m_coordChecked.end(), c) == m_coordChecked.end())
				validCoord.push_back(c);
		}
		return validCoord;
	}
	void removeWall(Coord c1, Coord c2)
	{
		if (c1.x == c2.x)
		{
			int i = min(c1.y, c2.y);
			int j = c1.x;
			m_matWallH[i * (m_nbColumn)+j] = false;
		}
		else if (c1.y == c2.y)
		{
			int i = c1.y;
			int j = min(c1.x, c2.x);
			m_matWallV[i * (m_nbColumn - 1) + j] = false;
		}
	}

public:
	Maze(int _nbRow, int _nbColumn) : m_nbRow(_nbRow), m_nbColumn(_nbColumn)
	{
		srand(time(NULL));
		m_matWallH = std::vector<bool>((m_nbRow - 1) * m_nbColumn, true);
		m_matWallV = std::vector<bool>(m_nbRow * (m_nbColumn - 1), true);
		m_matCell = std::vector<bool>(m_nbRow * m_nbColumn, false);
		m_currentCoord = { rand() % m_nbColumn,rand() % m_nbRow };
		m_coordChecked.push_back(m_currentCoord);
		m_bcktStack.push(m_currentCoord);
	}

	bool backTrackingIteration()
	{
		if (!m_bcktStack.empty())
		{
			m_currentCoord = m_bcktStack.top();
			m_frontCoord = m_currentCoord;
			m_bcktStack.pop();
			std::vector<Coord> validCoord = possibleCoord(m_currentCoord);
			if (!validCoord.empty())
			{
				m_bcktStack.push(m_currentCoord);
				Coord tempCoord = validCoord[rand() % validCoord.size()];
				m_frontCoord = tempCoord;
				removeWall(m_currentCoord, tempCoord);
				m_coordChecked.push_back(tempCoord);
				m_matCell[tempCoord.y * m_nbColumn + tempCoord.x] = true;
				m_bcktStack.push(tempCoord);
			}
			m_matCell[m_frontCoord.y * m_nbColumn + m_frontCoord.x] = true;
			return true;
		}
		m_frontCoord = { -1,-1 };
		return false;
	}

	bool getMatH(int i,int j)
	{
		//if (i >= 0 && j >= 0 && i < nbRow - 1 && j < nbColumn)
			return m_matWallH[i * m_nbColumn + j];
	}
	void setMatH(int i,int j,bool value)
	{
		//if (i >= 0 && j >= 0 && i < nbRow - 1 && j < nbColumn)
			m_matWallH[i * m_nbColumn + j] = value;
	}
	bool getMatV(int i, int j)
	{
		//if (i >= 0 && j >= 0 && i < nbRow && j < nbColumn - 1)
			return m_matWallV[i * (m_nbColumn - 1) + j];
	}
	void setMatV(int i,int j,bool value)
	{
		//if (i >= 0 && j >= 0 && i < nbRow && j < nbColumn - 1)
			m_matWallV[i * (m_nbColumn - 1) + j] = value;
	}

	std::vector<bool> getScreenMat(int dimCell) {
		int width = m_nbColumn * (dimCell + 1) + 1;
		int height = m_nbRow * (dimCell + 1) + 1;
		std::vector<bool> screenMat(width* height,1);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if ((i == 0 || i == height - 1 || j == 0 || j == width - 1) || (i % (dimCell + 1) == 0 && j % (dimCell + 1) == 0))//Border and point between 4 cells
					screenMat[i * width + j] = 1;
				else if (i % (dimCell + 1) == 0)//Horizontal wall
				{
					//screenMat[i * width + j] = (i/(dimCell+1))%2==0;
					screenMat[i * width + j] = getMatH(i / (dimCell + 1)-1, j / (dimCell + 1));
				}
				else if (j % (dimCell + 1) == 0)//Vertical wall
				{
					//screenMat[i * width + j] = true;
					screenMat[i * width + j] = getMatV(i / (dimCell + 1), j / (dimCell + 1) - 1);
				}
				else//All other points are cells
				{
					screenMat[i * width + j] = 0;
				}
			}
		}
		return screenMat;
	}


	bool isChecked(int i, int j)
	{
		return m_matCell[i * m_nbColumn + j];
	}

	bool isCurrentCell(int i,int j)
	{
		if (m_frontCoord == Coord{ j,i })
			return true;
		return false;
	}

	


	

};