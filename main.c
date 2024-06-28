#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_ROWS 100
#define MAX_COLS 100

void printPos(Position p)
{
	printf("Row: %d, Column: %d\n", p.row, p.col);
}

int comparePos(Position p1, Position p2)
{
	return p1.row == p2.row && p1.col == p2.col;
}

void readMaze(char *filename, char maze[MAX_ROWS][MAX_COLS], int *rows, int *cols)
{
	FILE *file = fopen("maze.txt", "r");
	if (!file)
	{
		perror("Error opening file");
		exit(1);
	}
	char line[MAX_COLS];
	while (fgets(line, MAX_COLS, file))
	{
		for (int j = 0; line[j] != '\n' && line[j] != '\0'; j++)
		{
			maze[*rows][j] = line[j];
		}
		(*rows)++;
	}
	*cols = strlen(line) - 1;
	fclose(file);
}

void printMaze(char maze[MAX_ROWS][MAX_COLS], int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

Position getStartPosition(char maze[MAX_ROWS][MAX_COLS], int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (maze[i][j] == 'S')
			{
				Position startPos;
				startPos.row = i;
				startPos.col = j;
				return startPos;
			}
		}
	}

	return (Position){-1, -1};
}

Position getEndPosition(char maze[MAX_ROWS][MAX_COLS], int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (maze[i][j] == 'E')
			{
				Position endPos;
				endPos.row = i;
				endPos.col = j;
				return endPos;
			}
		}
	}

	return (Position){-1, -1};
}

int dfs(char maze[MAX_ROWS][MAX_COLS], int rows, int cols, int visited[rows][cols], Position pos, Position end)
{
	int row = pos.row;
	int col = pos.col;

	if (row < 0 || row >= rows || col < 0 || col >= cols || visited[row][col] || maze[row][col] == '#')
	{
		return 0;
	}

	if (comparePos(pos, end))
	{
		maze[row][col] = '1';
		return 1;
	}
	visited[row][col] = 1;

	int dRows[4] = {-1, 0, 1, 0};
	int dCols[4] = {0, -1, 0, 1};

	for (int i = 0; i < 4; i++)
	{
		int nRow = row + dRows[i];
		int nCol = col + dCols[i];
		Position newPos = {nRow, nCol};

		if (dfs(maze, rows, cols, visited, newPos, end))
		{
			maze[row][col] = '1';
			return 1;
		}
	}

	visited[row][col] = 0;
	return 0;
}

void bfs(char maze[MAX_ROWS][MAX_COLS], int rows, int cols, int visited[rows][cols], Position pos, Position end)
{
	Queue q;
	initializeQueue(&q);
	qPush(&q, pos);

	int dRows[4] = {-1, 0, 1, 0};
	int dCols[4] = {0, -1, 0, 1};

	visited[pos.row][pos.col] = 1;
	Position predecessors[rows][cols];


	for(int i = 0; i<rows; i++){
		for(int j = 0; j<cols; j++){
			predecessors[i][j] = (Position){-1,-1};
		}
	}

	int flag = 0;

	while (!isQueueEmpty(&q))
	{
		Position curPos = qPop(&q);
		int row = curPos.row;
		int col = curPos.col;
		if (comparePos(curPos, end))
		{
			flag = 1;
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			int nRow = row + dRows[i];
			int nCol = col + dCols[i];
			Position newPos = {nRow, nCol};
			if (nRow >= 0 && nRow < rows && nCol >= 0 && nCol < cols && !visited[nRow][nCol] && maze[nRow][nCol] != '#')
			{
				visited[nRow][nCol] = 1;
				qPush(&q, (Position){nRow, nCol});
				predecessors[nRow][nCol] = curPos;
			}
		}
	}

	if (flag == 1)
	{
		printf("Found an end\n");
		char path[MAX_ROWS][MAX_COLS];

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				path[i][j] = maze[i][j];
			}				
		}
		
		Position curr = end;
		while(!comparePos(curr, pos)){
			path[curr.row][curr.col] = '1';
			curr = predecessors[curr.row][curr.col];
		}

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				printf("%c ", path[i][j]);
			}
			printf("\n");				
		}
	}
	else
	{
		printf("Couldn't find an end\n");
	}
}

int main()
{
	char maze[MAX_ROWS][MAX_COLS];
	int rows = 0;
	int cols = 0;
	readMaze("maze.txt", maze, &rows, &cols);
	printf("-------------MAZE--------------\n");
	printMaze(maze, rows, cols);
	Position start = getStartPosition(maze, rows, cols);
	if (start.row == -1 || start.col == -1)
	{
		perror("No start symbol");
		exit(1);
	}

	Position end = getEndPosition(maze, rows, cols);
	if (end.row == -1 || end.col == -1)
	{
		perror("No start symbol");
		exit(1);
	}

	int visited[rows][cols];

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			visited[i][j] = 0;
		}
	}

	printf("\n\nTRAVERSAL:\n");

	bfs(maze, rows, cols, visited, start, end);
	// if (dfs(maze, rows, cols, visited, start, end))
	// {
	// 	printf("Found the end\n");

	// 	printMaze(maze, rows, cols);
	// }
	// else
	// {
	// 	printf("Couldn't find the end\n");
	// }
}