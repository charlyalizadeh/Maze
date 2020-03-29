#pragma once
#include "olcConsoleGameEngine.h"
#include <vector>
#include "Maze.h"
#include <string>





class Engine : public olcConsoleGameEngine
{
public:
	Engine(int nbRow, int nbColumn, int dimCell = 3, short pathColor = FG_WHITE | BG_BLACK, short currentCellColor = FG_RED | BG_BLACK , short backgroundColor = FG_BLACK | BG_BLACK) : m_nbRow(nbRow), m_nbColumn(nbColumn), m_dimCell(dimCell), m_pathColor(pathColor), m_currentCellColor(currentCellColor), m_backgroundColor(backgroundColor), m_maze(nbRow, nbColumn)
	{

		int width = nbColumn * (dimCell + 1) + 1;
		int height = nbRow * (dimCell + 1) + 1;
		m_colorCells = std::vector<short>(m_nbRow * m_nbColumn, backgroundColor);
		player = { 1,1 };
		m_colorPixel = std::vector<short>(width * height, 0);
		olcConsoleGameEngine::ConstructConsole(width, height, 8, 8);
	}
private:

	//Generation variables
	int m_dimCell, m_nbRow, m_nbColumn;
	Maze m_maze;
	std::vector<short> m_colorCells;
	short m_pathColor, m_currentCellColor, m_backgroundColor;

	//Game variables
	float timeLastMove;
	Coord player;
	std::vector<bool> m_mazeMap;//a 2D dimensional array (theorically not in pratic) which contains true if (i,j) is wall, else false.
	std::vector<short> m_colorPixel;

	//Generation methods
	bool generateMaze() 
	{
		bool generate = m_maze.backTrackingIteration();
		updateColor();
		return generate;
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
				Fill(indexX, indexY, indexX + m_dimCell, indexY + m_dimCell, 9608, m_colorCells[i * m_nbColumn + j]);
				indexX += m_dimCell + 1;
			}
			indexY += m_dimCell + 1;
		}

		indexX = m_dimCell + 1;
		indexY = 1;
		for (int i = 0; i < m_nbRow; i++)
		{
			indexX = m_dimCell + 1;
			for (int j = 0; j < m_nbColumn - 1; j++)
			{
				if (m_maze.getMatV(i, j))
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
				if (m_maze.getMatH(i, j))
					DrawLine(indexX, indexY, indexX + m_dimCell - 1, indexY, 9608, m_backgroundColor);
				else
					DrawLine(indexX, indexY, indexX + m_dimCell - 1, indexY, 9608, m_pathColor);
				indexX += m_dimCell + 1;
			}
			indexY += m_dimCell + 1;
		}
	}
	void updateColor()
	{
		for (int i = 0; i < m_nbRow; i++)
		{
			for (int j = 0; j < m_nbColumn; j++)
			{
				if (m_maze.isCurrentCell(i, j))
					m_colorCells[i * m_nbColumn + j] = m_currentCellColor;
				else if (m_maze.isChecked(i, j))
					m_colorCells[i * m_nbColumn + j] = m_pathColor;
				else
					m_colorCells[i * m_nbColumn + j] = m_backgroundColor;
			}
		}
	}


	//Game methods
	void play() {
		if (timeLastMove > 0.05)
		{
			if ((GetKey(VK_UP).bPressed || GetKey(VK_UP).bHeld) && isValidMove(VK_UP))
				player.y--;
			if ((GetKey(VK_DOWN).bPressed || GetKey(VK_DOWN).bHeld) && isValidMove(VK_DOWN))
				player.y++;
			if ((GetKey(VK_LEFT).bPressed || GetKey(VK_LEFT).bHeld) && isValidMove(VK_LEFT))
				player.x--;
			if ((GetKey(VK_RIGHT).bPressed || GetKey(VK_RIGHT).bHeld) && isValidMove(VK_RIGHT))
				player.x++;
			timeLastMove = 0;
		}
		drawMazeMap();
		drawPlayer();
	}
	bool isValidMove(short key)
	{
		bool isValid = true;
		switch(key)
		{
		case VK_UP:
			for (int i = player.x; i < player.x + m_dimCell && isValid; i++)
			{
				if (m_mazeMap[(player.y - 1) * ScreenWidth() + i])
					isValid = false;
			}
			break;
		case VK_LEFT:
			for (int i = player.y; i < player.y + m_dimCell && isValid; i++)
			{
				if (m_mazeMap[i * ScreenWidth() + (player.x - 1)])
					isValid = false;
			}
			break;
		case VK_RIGHT:
			for (int i = player.y; i < player.y + m_dimCell && isValid; i++)
			{
				if (m_mazeMap[i * ScreenWidth() + (player.x+m_dimCell)])
					isValid = false;
			}
			break;
		case VK_DOWN:
			for (int i = player.x; i < player.x + m_dimCell && isValid; i++)
			{
				if (m_mazeMap[(player.y + m_dimCell) * ScreenWidth() + i])
					isValid = false;
			}
			break;
		}
		return isValid;
	}
	void drawPlayer()
	{
		for (int i = player.y; i < player.y + m_dimCell; i++)
		{
			for (int j = player.x; j < player.x + m_dimCell; j++)
			{
				m_colorPixel[i * ScreenWidth() + j] = FG_YELLOW;
			}
		}
		Fill(player.x, player.y, player.x + m_dimCell, player.y + m_dimCell, 9608, FG_DARK_GREEN);
	}
	void drawMazeMap()
	{
		for(int i = 0;i<ScreenHeight();i++)
		{
			for(int j = 0;j<ScreenWidth();j++)
			{
				if (m_colorPixel[i * ScreenWidth() + j] == FG_YELLOW)
					Draw(j, i, 9608, FG_YELLOW);
				else if (m_mazeMap[i * ScreenWidth() + j])
					Draw(j, i, 9608, FG_BLACK);
				else
					Draw(j, i, 9608, FG_WHITE);
			}
		}
	}

protected:
	bool OnUserCreate() 
	{		
		timeLastMove = -1;
		return true;
	}
	bool OnUserUpdate(float fEslapsedTime)
	{
		if(timeLastMove!=-1)
			timeLastMove += fEslapsedTime;
		Fill(0, 0, ScreenWidth(), ScreenHeight(), 9608, m_backgroundColor);
		if (generateMaze())
			drawMaze();
		else
		{
			if (timeLastMove == -1)
			{
				m_mazeMap = m_maze.getScreenMat(m_dimCell);
				timeLastMove = 0;
			}
			play();
		}
		
		return true;
	}
};


