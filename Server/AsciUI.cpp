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

int AsciUI::render(Player* players)
{
	
	system("cls");
	int layout[15][15];

	for(int i = 0 ; i < 4 ;i++) {
		layout[(int)(players[i].getPosition()).y][(int)(players[i].getPosition()).x] = i;
		
	}

	string out_map = "+";
	for(int j = 0; j < 15; j++) {
			out_map.append("---+");
	}
	out_map.append("\n");
	for(int i = 0; i < 15; i++) {
		out_map.append("|");
		for(int j = 0; j < 15; j++) {
			if(layout[i][j] == 0) {
				out_map.append(" 11 ");
			} else if(layout[i][j] == 1){
				out_map.append(" 22 ");
			} else if(layout[i][j] == 2){
				out_map.append(" 33 ");
			}else if(layout[i][j] == 3){
				out_map.append(" 44 ");
			} else {
				out_map.append("    ");
			}


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
		out_map.append("Player1 health: " + std::to_string( long long(players[0].getHealth() ) ) + "\n") ;
		out_map.append("Player2 health: "  + std::to_string( long long(players[1].getHealth() ) ) + "\n" );
		out_map.append("Player3 health: " + std::to_string( long long(players[2].getHealth() ) ) + "\n");
		out_map.append("Player4 health: " + std::to_string( long long(players[3].getHealth() ) ) + "\n");
    out_map.append(players[0].getString());
	cout << out_map << endl;

		
	return 0;

}

int AsciUI::update(Map* map)
{
	system("cls");
	int layout[15][15];
//	Entity* entities = map->getEntities();
//	Player* players = (Player*)entities;
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

