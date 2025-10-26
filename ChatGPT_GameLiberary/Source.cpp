#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// ---------------------------- Data Model ----------------------------
struct Game {
	std::string name;
	int         quantity = 0;   // store as integer
	int         priceCents = 0; // store money as cents to avoid float errors
};

// ---------------------------- Small Utilities ----------------------------
static std::string toLowerCopy(std::string_view text) {
	std::string lowered(text);
	for (char& ch : lowered) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
	return lowered;
}

static std::string trimCopy(std::string_view text) {
	size_t first = 0, last = text.size();
	while (first < last && std::isspace(static_cast<unsigned char>(text[first]))) ++first;
	while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1]))) --last;
	return std::string(text.substr(first, last - first));
}

static bool isAffirmative(std::string_view input) {
	std::string lowered = toLowerCopy(input);
	return lowered == "y" || lowered == "yes" || lowered == "1" ||
		lowered == "true" || lowered == "ok" || lowered == "ofcourse" || lowered == "ofcource";
}

static bool parsePriceToCents(std::string_view text, int& outCents) {
	// Accept "12", "12.3", "12.34". Reject negatives and nonsense.
	std::string s = trimCopy(text);
	if (s.empty()) return false;
	bool seenDot = false;
	int cents = 0, fractional = 0, fracDigits = 0;

	for (char ch : s) {
		if (ch == '.') {
			if (seenDot) return false;
			seenDot = true;
			continue;
		}
		if (!std::isdigit(static_cast<unsigned char>(ch))) return false;
		if (!seenDot) {
			// dollars
			cents = cents * 10 + (ch - '0');
			if (cents > 2'000'000'000) return false;
		}
		else {
			// cents
			if (fracDigits < 2) {
				fractional = fractional * 10 + (ch - '0');
				++fracDigits;
			}
			else {
				// more than 2 fractional digits -> reject to keep format simple
				return false;
			}
		}
	}
	if (fracDigits == 1) fractional *= 10; // "12.3" -> "12.30"
	outCents = cents * 100 + fractional;
	return true;
}

static std::string centsToString(int cents) {
	int dollars = cents / 100;
	int rem = std::abs(cents % 100);
	std::ostringstream output;
	output << dollars << '.' << std::setw(2) << std::setfill('0') << rem;
	return output.str();
}

static bool parseIntSafe(std::string_view text, int& outValue) {
	std::string s = trimCopy(text);
	if (s.empty()) return false;
	long long value = 0;
	for (char ch : s) {
		if (!std::isdigit(static_cast<unsigned char>(ch))) return false;
		value = value * 10 + (ch - '0');
		if (value > std::numeric_limits<int>::max()) return false;
	}
	outValue = static_cast<int>(value);
	return true;
}

// ---------------------------- File I/O ----------------------------
static bool parseGameLine(std::string_view line, Game& outGame) {
	// Expected format: name:price:quantity
	size_t firstColon = line.find(':');
	if (firstColon == std::string_view::npos) return false;
	size_t secondColon = line.find(':', firstColon + 1);
	if (secondColon == std::string_view::npos) return false;

	std::string namePart = trimCopy(line.substr(0, firstColon));
	std::string pricePart = trimCopy(line.substr(firstColon + 1, secondColon - firstColon - 1));
	std::string quantityPart = trimCopy(line.substr(secondColon + 1));

	if (namePart.empty()) return false;

	int priceCents = 0;
	if (!parsePriceToCents(pricePart, priceCents)) return false;

	int quantityValue = 0;
	if (!parseIntSafe(quantityPart, quantityValue)) return false;

	outGame.name = std::move(namePart);
	outGame.priceCents = priceCents;
	outGame.quantity = quantityValue;
	return true;
}

static void loadLibraryFromFile(const std::string& filePath, std::vector<Game>& library) {
	std::ifstream inputFile(filePath);
	if (!inputFile.is_open()) return;

	std::string lineBuffer;
	while (std::getline(inputFile, lineBuffer)) {
		if (lineBuffer.empty()) continue;
		Game parsedGame;
		if (parseGameLine(lineBuffer, parsedGame)) {
			library.push_back(std::move(parsedGame));
		}
		else {
			// Ignore malformed lines to keep the program robust
		}
	}
}

static void saveLibraryToFile(const std::string& filePath, const std::vector<Game>& library) {
	std::ofstream outputFile(filePath, std::ios::trunc);
	if (!outputFile.is_open()) return;
	for (const Game& gameEntry : library) {
		outputFile << gameEntry.name << ':' << centsToString(gameEntry.priceCents) << ':' << gameEntry.quantity << '\n';
	}
}

// ---------------------------- Library Ops ----------------------------
static bool isDuplicateName(const std::vector<Game>& library, const std::string& candidateName) {
	return std::any_of(library.begin(), library.end(),
		[&](const Game& gameEntry) { return gameEntry.name == candidateName; });
}

static void printLibrary(const std::vector<Game>& library) {
	// 4 columns, 15 chars each, bordered
	const char borderChar = static_cast<char>(219);
	const int columns = 4;
	const int cellWidth = 15;
	const int roofLength = columns * cellWidth + columns + 1; // borders included

	auto printRoof = [&]() {
		for (int i = 0; i <= roofLength; ++i) std::cout << borderChar;
		std::cout << '\n';
		};

	auto centerCell = [&](std::string_view text) {
		std::string trimmed = (text.size() <= static_cast<size_t>(cellWidth)) ? std::string(text) : std::string(text.substr(0, cellWidth));
		int extra = cellWidth - static_cast<int>(trimmed.size());
		int leftPad = extra / 2;
		int rightPad = extra - leftPad;
		std::cout << std::string(leftPad, ' ') << trimmed << std::string(rightPad, ' ');
		};

	const size_t total = library.size();
	size_t rowStartIndex = 0;

	printRoof();
	while (rowStartIndex < total) {
		// Row 1: Names
		for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
			std::cout << borderChar;
			size_t itemIndex = rowStartIndex + static_cast<size_t>(columnIndex);
			if (itemIndex < total) centerCell(library[itemIndex].name);
			else                   centerCell("");
		}
		std::cout << borderChar << '\n';

		// Row 2: Prices
		for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
			std::cout << borderChar;
			size_t itemIndex = rowStartIndex + static_cast<size_t>(columnIndex);
			if (itemIndex < total) centerCell(centsToString(library[itemIndex].priceCents));
			else                   centerCell("");
		}
		std::cout << borderChar << '\n';

		// Row 3: Quantities
		for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
			std::cout << borderChar;
			size_t itemIndex = rowStartIndex + static_cast<size_t>(columnIndex);
			if (itemIndex < total) centerCell(std::to_string(library[itemIndex].quantity));
			else                   centerCell("");
		}
		std::cout << borderChar << '\n';

		printRoof();
		rowStartIndex += static_cast<size_t>(columns);
	}
}

// ---------------------------- Input Helpers ----------------------------
static void discardPendingNewline() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string readLine(const char* prompt) {
	std::cout << prompt;
	std::string line;
	std::getline(std::cin, line);
	return line;
}

// ---------------------------- Main ----------------------------
int main() {
	std::vector<Game> library;
	library.reserve(64); // small guess to reduce early reallocations

	// Load existing data (file name kept as in your original code)
	loadLibraryFromFile("Liberary.txt", library);

	// Show existing inventory
	printLibrary(library);

	std::cout << "HEY GAMER!! welcome to my humble game shop!\n"
		"You can store your games here, and I will sell them for you :)\n";

	// Ensure getline works even if something left a newline behind.
	if (std::cin.peek() == '\n') discardPendingNewline();

	while (true) {
		const std::string endDialogue =
			"alright in that case I will let you know when the games sell out!\n"
			"I definitely won't make a profit off of selling your games and never telling you about them!! :D\n";

		// Name (allow spaces)
		std::string gameName = readLine("What's the name of the game you want to sell?\n");
		gameName = trimCopy(gameName);
		if (gameName.empty()) {
			std::cout << "Empty name ignored.\n";
			continue;
		}
		if (isDuplicateName(library, gameName)) {
			std::string anotherGameAnswer = readLine("Whoops! You already registered this game. Do you have another game? (yes/no)\n");
			if (!isAffirmative(anotherGameAnswer)) {
				std::cout << endDialogue;
				break;
			}
			continue; // ask for another game
		}

		// Price
		int priceCents = 0;
		while (true) {
			std::string priceText = readLine("How much do you want me to sell it for? (e.g., 12.99)\n");
			if (parsePriceToCents(priceText, priceCents)) break;
			std::cout << "Please enter a valid price like 10, 10.5, or 10.50.\n";
		}

		// Quantity
		int quantityValue = 0;
		while (true) {
			std::string quantityText = readLine("How many copies do you want to sell?\n");
			if (parseIntSafe(quantityText, quantityValue) && quantityValue >= 0) break;
			std::cout << "Please enter a non-negative integer.\n";
		}

		// Commit new game
		library.push_back(Game{ gameName, quantityValue, priceCents });

		std::string moreGamesAnswer = readLine("Noted and stored! Any more games you wanna sell? (yes/no)\n");
		if (!isAffirmative(moreGamesAnswer)) {
			std::cout << endDialogue;
			break;
		}
	}

	std::cout << "Here is your library:\n";
	printLibrary(library);

	// Totals loop
	std::string totalsAnswer = readLine("\nWould you like to see the total price of a stack? (yes/no)\n");
	while (isAffirmative(totalsAnswer)) {
		if (library.empty()) {
			std::cout << "Library is empty.\n";
			break;
		}

		// Show index list with names to guide selection
		std::cout << "Choose an item by index (1.." << library.size() << "):\n";
		for (size_t index = 0; index < library.size(); ++index) {
			std::cout << "  " << (index + 1) << ") " << library[index].name << '\n';
		}

		std::string indexText = readLine("Index: ");
		int oneBasedIndex = 0;
		if (!parseIntSafe(indexText, oneBasedIndex) || oneBasedIndex < 1 || static_cast<size_t>(oneBasedIndex) > library.size()) {
			std::cout << "Invalid index.\n";
		}
		else {
			size_t zeroBasedIndex = static_cast<size_t>(oneBasedIndex - 1);
			const Game& selected = library[zeroBasedIndex];
			long long totalCents = static_cast<long long>(selected.priceCents) * static_cast<long long>(selected.quantity);
			std::cout << "We have " << selected.quantity << " copies of " << selected.name
				<< ". The total price is: " << centsToString(static_cast<int>(totalCents)) << "$\n";
		}

		totalsAnswer = readLine("\nWould you like to check another game set? (yes/no)\n");
	}

	// Save data back to file
	saveLibraryToFile("Liberary.txt", library);
	return 0;
}