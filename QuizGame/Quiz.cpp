#include <iostream>
struct QuestionStruct
{
	std::string questionText = "A";
	std::string answers[4] = { "Abrica","Babrica","Cabrica","Dabrica" };
	bool correctAnswer[4] = { false, false, false, false };
};

int translateInputToIndex(char inputTranslate)
{
	switch (inputTranslate) {
	case '1': case 'a': case 'A':
		return 0;
	case '2': case 'b': case 'B':
		return 1;
	case '3': case 'c': case 'C':
		return 2;
	case '4': case 'd': case 'D':
		return 3;
	default:
		return -1; 
	}
}
bool checkAnswer(int userAnswerInput, QuestionStruct currentQuestion)
{
	if (currentQuestion.correctAnswer[userAnswerInput])
	{
		std::cout << "welldone!!" << std::endl;
		return true;
	}
	else {
		std::cout << "nice try" << std::endl;
		return false;
	}
}
int takeAnswer()
{
	char input;
	std::cout << "enter answer: ";
	std::cin >> input;
	int returnValue = translateInputToIndex(input);
	return returnValue;
}
void printAnswer(int index, QuestionStruct prntAnsQuestionVar) {
	if (index >= 4)
	{
		return;
	}
	std::cout << char(index + 65) << ") ";
	std::cout << prntAnsQuestionVar.answers[index] << std::endl;
	printAnswer(index + 1, prntAnsQuestionVar);

}
bool printSingleQuestion(QuestionStruct currentQuestion)
{
	int userAnswer;
	std::cout << currentQuestion.questionText << std::endl;
	printAnswer(0, currentQuestion);
	userAnswer = takeAnswer();
	return (checkAnswer(userAnswer, currentQuestion));
}

void QuizFunction(QuestionStruct Questions[], int quantityOfQuestions, int index, int score)
{
	if (index >= quantityOfQuestions)
	{
		std::cout << "quiz is done." << std::endl << "your score is: " << score << "/" << index << std::endl;
		return;
	}
	if (printSingleQuestion(Questions[index]))
	{
		score++;
	}
	QuizFunction(Questions, quantityOfQuestions, index + 1, score);
}

int main()
{
	const int quantityOfQuestions = 5;
	int startingIndex = 0;
	int startingScore = 0;
	QuestionStruct myQuestions[quantityOfQuestions] = {
		{
			"The original PlayStation was born from a failed partnership with which company?",
			{"Sega", "Philips", "Nintendo", "Atari"},
			{false, false, true, false}
		},
		{
			"Which video game was the first ever to be played in space?",
			{"Pong", "Tetris", "Super Mario Land", "The Legend of Zelda"},
			{false, true, false, false}
		},
		{
			"Which console was first to include online connectivity built into the hardware?",
			{"Dreamcast", "PS2", "Xbox", "GameCube"},
			{true, false, false, false}
		},
		{
			"What was the original intended name of the Xbox?",
			{"DirectX Box", "X-Console", "Game Box", "PlayBox"},
			{true, false, false, false}
		},
		{
			"Which N64 game was the first to implement a fully 3D camera the player could control?",
			{"Super Mario 64", "Ocarina of Time", "GoldenEye 007", "Star Fox 64"},
			{true, false, false, false}
		}
	};

	QuizFunction(myQuestions, quantityOfQuestions, startingIndex, startingScore);


	return 0;
}