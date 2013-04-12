#include "AsciUI.h"
#include <string>
#include <iostream>

using namespace System;
using namespace System::IO;
using namespace std;

AsciUI::AsciUI(void)
{
}


AsciUI::~AsciUI(void)
{
}

int update()
{
	system("cls");
	Console::WriteLine("hello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello therehello there");
	
	return 0;
}

int main()
{

	//StreamWriter^ writer = gcnew StreamWriter(args[2]);

	
		//Console::SetOut(writer);
		
		//Console.SetIn(new StreamReader(args[1]));
	
	string str;
	while(true) {
		cin >> str;
		update();
	}

//sw->Close();
}
