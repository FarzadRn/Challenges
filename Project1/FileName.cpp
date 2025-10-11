#include <iostream>
#include <fstream>
int main()
{
	char Letters[50];
	//alphabet.txt
	std::ifstream aF("alphabet.txt");
	if (aF.is_open())
	{

		int index = 0;
		while (index < 26 && aF >> Letters[index])
		{
			std::cout << Letters[index] << '\n';
			index++;
		}
		std::cout << char(219);
		for (int i = 0; i != 26; i++)
		{
			Letters[i] = char(97 + i);
		}
		aF.close();

	}
	std::ofstream af("alphabet.txt");
	if (af.is_open())
	{
		for (int i = 0; i != 26; i++)
		{
			af << Letters[i] << '\n';
		}
		af.close();
	}
	if (aF.is_open())
	{

		int index = 0;
		while (index < 26 && aF >> Letters[index])
		{
			std::cout << Letters[index] << '\n';
			index++;
		}
		std::cout << char(97);
		aF.close();

	}
}