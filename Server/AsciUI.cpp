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
	int layout[15][15];
	Entity* entities = map->getEntities();
	Player* players = (Player*)entities;
	for(int i = 0 ; i < 10 ;i++) {
	/*
		if(players[i]){
			layout[(players[i]->getPosition())->x][(players[i]->getPosition())->y] = 1;
		}
		*/
		
	}

	string out_map = "+";
	for(int j = 0; j < 15; j++) {
			out_map.append("---+");
	}
	out_map.append("\n");
	for(int i = 0; i < 15; i++) {
		out_map.append("|");
		for(int j = 0; j < 15; j++) {
			out_map.append("    ");
		}
		out_map.append("|");
		out_map.append("\n+");
		if(i == 14) {
			for(int j = 0; j < 15; j++) {
				out_map.append("---+");
			}
		}else {
			for(int j = 0; j < 15; j++) {
				out_map.append("   +");
			}
		}
		out_map.append("\n");

	}

	/*
		+---+   +   +   +   +
		| @  XXX   
		+   +   +   +   +   +
		|XXX XXX
		+   +   +   +   +   +

	*/

	cout << out_map << endl;

		
	return 0;
}

