#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct game
{
	std::string Name = "EMPTYNAME";
	std::string Price = "EMPTYPRICE";
	std::string Quantity = "EMPTYQuantity";
};

//inventory//add game to inventory
//user enters name //no dupe// price quantity 
//display all after input
//select game to see totall price
//read from a file/write to a file why not

//---1. make the inventory 
//---2. let player add item
//---3. add function to detect dupe
//---4. print all games
//---5. add function to detect total price of each chosen game
//---6. write to file the games added
//---7. read from file
//---8. check file for available data to then feed the vectors
//---9. loop back done.
//10. polish


bool isDupe(std::vector<game>& inputedGame, std::string inputedName)
{
	//for (std::vector<game>::iterator iterator = inputedGame.begin(); iterator != inputedGame.end(); iterator++)
	for (int i = 0; i != inputedGame.size(); i++)
	{
		if (inputedName == inputedGame[i].Name)
		{
			return true;
		}
	}
	return false;
}
bool isYes(std::string& input)
{
	return input == "yes" || input == "1" || input == "ofcource";
}

void printLiberary(std::vector<game>& liberary);


int main()
{
	//first create games vector
	std::vector<game> games;

	//Take data from Liberary file-------------------------------------------
	std::ifstream inLib("Liberary.txt");
	if (inLib.is_open())
	{
		char buffer[48];
		game gameTemp;
		while (inLib.getline(buffer, 48))
		{
			std::string singleLine(buffer);
			
			int nameEnd = singleLine.find(':');
			int priceEnd = singleLine.find(':', nameEnd+1);
			std::string tempLine = singleLine;

			int length = nameEnd;
			std::string name = tempLine.substr(0, length);

			tempLine = singleLine;
			length = priceEnd - nameEnd - 1;

			std::string price = singleLine.substr(nameEnd+1, length);

			tempLine = singleLine;
			std::string quant = singleLine.substr(priceEnd+1);
			//fill game struct type:
			gameTemp.Name = name; 
			gameTemp.Price = price; 
			gameTemp.Quantity = quant;

			games.push_back(gameTemp);


		}
		inLib.close();
	}
	//Data in end-----------------------------------------------------------

	printLiberary(games);


	//game start -----------------------------------------------------------
	std::cout << "HEY GAMER!! welcome to my humble game shop!" << '\n';
	std::cout << "You can store your games here, and I will sell them for you :)" << '\n';
	while (true)
	{
		std::string input = "0";
		bool isGameDupe = false;
		game userGame;
		std::string endDialogue = "alright in that case I will let you know when the games sell out! '\n'I defintely won't make a profit off of selling your games and never telling you about them!! :D";

		std::cout << "What's the name of the game you want to sell? " << '\n';
		std::cin >> userGame.Name;
		if (isDupe(games, userGame.Name))
		{
			std::cout << "whoops! you have already registered this game! do you have another game? " << '\n';
			std::cin >> input;
			if (!isYes(input))
			{
				std::cout << endDialogue << '\n';
				break;
			}
		}
		std::cout << "and how much do want me to sell it for? " << '\n';
		std::cin >> userGame.Price;
		std::cout << "gotcha, how many copies of {" << userGame.Name << "}s do you want to sell for you?" << '\n';
		std::cin >> userGame.Quantity;
		games.push_back(userGame);
		std::cout << "noted and stored! any more games you wanna sell? (all lowercase please)" << '\n';
		std::cin >> input;
		if (isYes(input))
		{
			std::cout << "alright!" << '\n';
			continue;
		}
		else
		{
			std::cout << endDialogue << '\n';
			break;
		}
	}
	std::cout << "Here is your liberary btw!: " << '\n';
	printLiberary(games);
	
	std::cout << '\n' << '\n' << '\n';
	std::cout << "would you like to see the total price of each stack? " << '\n';
	std::string input;
	std::cin >> input;
	while (isYes(input))
	{
		std::cout << "which stack are you interested in?";
		int inputIndex;
		std::cin >> inputIndex;
		int trueIndx = inputIndex - 1;
		std::cout << "We have " << games[trueIndx].Quantity << " copies of " << games[trueIndx].Name << ". The total price is: " << '\n';
		int quantity = std::stoi(games[trueIndx].Quantity);
		float price = std::stof(games[trueIndx].Price);
		float totalPrice = price * quantity;
		std::string total = std::to_string(totalPrice);
		total.append("$");
		std::cout << total;
		std::cout << '\n' << '\n';
		std::cout << "would you like to check total price of another game set?" << '\n';
		std::cin >> input;
		if (isYes(input))
		{
			continue;
		}
		else
		{
			break;
		}
	}
	//game end -----------------------------------------------------------


	//save data ----------------------------------------------------------
	std::ofstream outLib("Liberary.txt");
	if (outLib.is_open())
	{
		for (std::vector<game>::iterator it = games.begin(); it != games.end(); it++)
		{
			outLib << (*it).Name << ":" << (*it).Price <<":" << (*it).Quantity;
			outLib << '\n';
		}
		outLib.close();
	}
	//save done ----------------------------------------------------------
}

void printLiberary(std::vector<game>& liberary)
{
	{
		char border = char(219);
		auto rowCount = (liberary.size() / 4) + 1;
		int indexFixer = 0;
		for (int i = 0; i != rowCount; i++)
		{
			std::string space15 = "               ";
			//COSMETIC print roof
			if (i == 0)
			{
				//64 is length of roof
				for (int y = 0; y <= 64; y++)
				{
					std::cout << border;

				}
				std::cout << '\n';
			}

			//print top row----- NAME
			for (int j = 0; j != 4; j++)
			{
				if (j == 0)
				{
					std::cout << border;
				}
				if ((indexFixer + j) < liberary.size())
				{
					std::string temp = space15;
					//7(SIZE/2) gives the spacing needed with 15 character limit
					int spacingNeeded = 7 - (liberary[indexFixer + j].Name.size() / 2);
					temp.replace(spacingNeeded, liberary[indexFixer + j].Name.size(), liberary[indexFixer + j].Name);
					std::cout << temp;
				}
				else
				{
					std::cout << space15;
				}

					std::cout << border;
			}
			//TOPROW DONE------- NAME DONE

			std::cout << '\n';

			//print mid row----- PRICE
			for (int j = 0; j != 4; j++)
			{
				if (j == 0)
				{
					std::cout << border;
				}
				if ((indexFixer + j) < liberary.size())
				{
					std::string temp = space15;
					//7(SIZE/2) gives the spacing needed with 15 character limit
					int spacingNeeded = 7 - (liberary[indexFixer + j].Price.size() / 2);
					temp.replace(spacingNeeded, liberary[indexFixer + j].Price.size(), liberary[indexFixer + j].Price);
					std::cout << temp;
				}
				else
				{
					std::cout << space15;
				}
				std::cout << border;
			}
			//MIDROW DONE------ PRICE DONE

			std::cout << '\n';

			//print bottom row- QUANT
			for (int j = 0; j != 4; j++)
			{
				if (j == 0)
				{
					std::cout << border;
				}
				if ((indexFixer + j) < liberary.size())
				{
					std::string temp = space15;
					//7(SIZE/2) gives the spacing needed with 15 character limit
					int spacingNeeded = 7 - (liberary[indexFixer + j].Quantity.size() / 2);
					temp.replace(spacingNeeded, liberary[indexFixer + j].Quantity.size(), liberary[indexFixer + j].Quantity);
					std::cout << temp;
				}
				else
				{
					std::cout << space15;
				}
				std::cout << border;
			}
			//BOTTOMROW DONE- QUANT DONE

			std::cout << '\n';

			//print floor/roof
			//64 is length of roof
			for (int y = 0; y <= 64; y++)
			{
				std::cout << border;
			}
			std::cout << '\n';

			//INDEX UP!
			indexFixer += 4;
		}
	}
}
