#include"conio.h"
#include"stdio.h"
#include"string.h"
#include"iostream"
#include <fstream>
#include <atlstr.h>
#include "../SuperMarioBros3/Utils.cpp"
using namespace std;

#define MAX_GAME_LINE 1024
#define CELL_WIDTH	150
#define CELL_HEIGHT	150

void CreateGridFile(int idScene);
void GetMapSize(string scenePath, int& mapWidth, int& mapHeight);
string GetScenePath(int idScene);

void WriteGridFile(string scenePath, int RowsOfGrid, int columnsOfGrid);


int main()
{
	int idScene;
	cout << "Id Scene: ";
	cin >> idScene;
	CreateGridFile(idScene);
	return 0;
}

void CreateGridFile(int idScene)
{
	int mapWidth = 0;
	int mapHeight = 0;
	string scenePath = GetScenePath(idScene);
	cout << "Scene path: " << scenePath << "\n";


	GetMapSize(scenePath, mapWidth, mapHeight);
	cout << "Map Width: " << mapWidth << ", Map Height: " << mapHeight << "\n";
	if (mapWidth == 0 || mapHeight == 0)
		return;

	int colsOfGrid = (int)(mapWidth / CELL_WIDTH +1);
	int rowsOfGrid = (int)(mapHeight / CELL_HEIGHT +1);

	WriteGridFile(scenePath, colsOfGrid, rowsOfGrid);
}

void GetMapSize(string scenePath, int &mapWidth, int &mapHeight)
{

	ifstream f(ToLPCWSTR( scenePath));
	char str[MAX_GAME_LINE];
	bool IsSectionMAP = false;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);
		if (line == "[MAP]")
		{
			IsSectionMAP = true;
			continue;
		}
		if (!IsSectionMAP)
			continue;
		if (line[0] == '#')
			continue;

		vector<string> tokens = split(line);

		if (tokens.size() < 9)
			return;

		int tileWidth = atoi(tokens[1].c_str());
		int tileHeight = atoi(tokens[2].c_str());

		int tRMap = atoi(tokens[5].c_str());
		int tCMap = atoi(tokens[6].c_str());

		mapWidth = tileWidth * tCMap;
		mapHeight = tileHeight * tRMap;

		break;
	}
	f.close();
}

string GetScenePath(int idScene)
{
	string scenePath = "";
	ifstream f;
	f.open(L"..\\SuperMarioBros3\\globalData\\mario-sample.txt");
	char str[MAX_GAME_LINE];
	bool IsSectionScene = false;


	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line == "[SETTINGS]")
		{
			IsSectionScene = true;
			continue;
		}

		if (!IsSectionScene)
			continue;

		if (line[0] == '#') continue;	// skip comment lines	

		//get scene path
		vector<string> tokens = split(line);

		if (tokens.size() < 2) 
			continue;
		int id = atoi(tokens[0].c_str());
		if (id == idScene)
		{
			scenePath = "..\\SuperMarioBros3\\" + tokens[1];
			return scenePath;
		}
	}
	f.close();
	return scenePath;
}

void WriteGridFile(string scenePath, int colsOfGrid, int rowsOfGrid)
{
	ofstream gridFile;
	ifstream sceneFile;
	string sceneName = split(split(scenePath, "\\").back(), ".").at(0);

	string gridFilePath = "..\\SuperMarioBros3\\grid\\" + sceneName + "_Grid.txt";
	
	gridFile.open(gridFilePath);
	sceneFile.open(scenePath);
	
	char str[MAX_GAME_LINE];
	bool IsSectionObjects = false;

	gridFile << rowsOfGrid << "\t" << colsOfGrid << "\n";
	while (sceneFile.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line == "[OBJECTS]")
		{
			IsSectionObjects = true;
			continue;
		}

		if (!IsSectionObjects)
			continue;

		if (line[0] == '#')
		{
			gridFile << line<<"\n";
			continue;
		}

		vector<string> tokens = split(line);

		if (tokens.size() < 3)
			continue;

		gridFile << line << "\t";

		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());

		int colsOfCell = (int)(x / CELL_WIDTH);
		int rowsOfCell = (int)(y / CELL_HEIGHT);

		if (colsOfCell >= colsOfGrid)
			colsOfCell = colsOfGrid - 1;
		if (rowsOfCell >= rowsOfGrid)
			rowsOfCell = rowsOfGrid - 1;
		
		gridFile << rowsOfCell << "\t" << colsOfCell << "\n";

		//cout << line<<"\t" << colsOfCell<< rowsOfCell<< "\n";
	}
	gridFile.close();
	sceneFile.close();

}