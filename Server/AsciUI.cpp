#include "AsciUI.h"
#include <string>
#include <iostream>

using namespace std;

AsciUI::AsciUI(void)
{
}


AsciUI::~AsciUI(void)
{
}

int AsciUI::update(Map* map)
{
	system("cls");
	int layout[25][25];
	Entity** entities = map->getEntities();
	for(int i = 0 ; i < 10 ;i++) {
		if(entities[i] != 0){
			//layout[entities[i]->position][entities[i]->position] = 1;
		}
	}

	string out_map;
	for(int j = 0; j < 25; j++) {
			out_map.append("—");
	}
	for(int i = 0; i < 25; i++) {
		out_map.append("|");
		for(int j = 0; j < 25; j++) {
			out_map.append(" |");
		}
		for(int j = 0; j < 25; j++) {
			out_map.append("—");
		}

	}

	cout << out_map << endl;

		
	return 0;
}

