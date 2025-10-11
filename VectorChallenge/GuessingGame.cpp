#include <iostream>
#include <vector>
inline int randNum(int max) {
	return (rand() % max) + 1;
}

int main()
{
	const int MAX_QUESTIONS = 20;
	const int SKIP_CODE = 11;
	const int EXIT_CODE = 12;

	std::vector<int> randNumbers;
	randNumbers.reserve(MAX_QUESTIONS + 2);

	int guesses = 0;
	int correct = 0;
	int skips = 0;
	int input;
	int index = 0;

	srand(time(0));
	for (int i = 0; i < MAX_QUESTIONS; i++)
	{
		randNumbers.push_back(randNum(10));
	}

	while (correct < MAX_QUESTIONS)
	{
		std::cout << "Guess the number or enter: " << SKIP_CODE << " to skip | " << EXIT_CODE << " to giveup ||| " << "number of guesses: " << guesses << std::endl;
		std::cin >> input;


		if (input == SKIP_CODE)
		{
			index++;
			skips++;
			continue;
		}
		else if (input == EXIT_CODE)
		{
			break;
		}

		if (index >= randNumbers.size())
		{
			index = 0;
		}

		guesses++;
		const int currentNumber = randNumbers[index];
		if (input == currentNumber)
		{
			std::cout << "correct!" << std::endl;
			correct++;
			randNumbers.erase(randNumbers.begin() + index);
			index++;
		}
		else 
		{ 
			std::cout << "incorrect!" << std::endl;
		}
	}

	std::cout << "out of " << guesses << "s guess you made " << correct << " correct guesses and you skipped " << skips << " times." << std::endl;
	return 0;
}