#ifndef _JOTTO_H
#define _JOTTO_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include "randgen.h"

#include <io.h> 
#include <fcntl.h>

using namespace std;

class Jotto {
public:
	Jotto();
	Jotto(string filename);

	int GetScore();
	bool Win();
	int GetNumOfJots();
	string GetRandomWord();
	void SetRandomWord();
	wstring To_wstring(string str);
	void GetInput();
	void ResetRounds();
	bool OutOfTurns();
	bool PlayAgain();
	void drawSheet();
	void FillRow();
	void ShowWord();

private:
	HANDLE console;
	int score;
	int scoring[20] = { 100,95,90,85,80,75,70,65,60,55,50,45,40,40,40,40,35,35,35,35 };
	string arr_words[4000];
	int arr_words_length = 0;
	string randomWord = "";
	int gameRound = 0;
	string guessedWord = "";
	RandGen r;
};

Jotto::Jotto() {
	_setmode(_fileno(stdout), _O_U16TEXT); // fixes unicode
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	int i;
	for (i = 0; i <= 3999; i++) {
		arr_words[i] = "";
	}
	randomWord = "";
}
Jotto::Jotto(string filename) {
	_setmode(_fileno(stdout), _O_U16TEXT); // fixes unicode
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	int i;
	for (i = 0; i <= 3999; i++) {
		arr_words[i] = "";
	}
	randomWord = "";


	ifstream infile;
	infile.open(filename, ios::in);
	char word[6];
	i = 0;

	if (infile) {
		while (!infile.eof()) {
			infile.getline(word, 20);
			arr_words[i] = word;
			i++;
		}
		i--;
		arr_words_length = i;
	}
	else
	{
		wcout << "Error opening file.\n";
	}
	infile.close();

}

int Jotto::GetScore() {
	return score;
}
bool Jotto::Win() {
	for (int i = 0; i < 5; i++)
	{
		if ((randomWord[i] != guessedWord[i] && randomWord[i] != (char)((int)(guessedWord[i]) - 32))) {
			return false;
		}
	}
	score += scoring[gameRound - 1];
	return true;
}
int Jotto::GetNumOfJots() {
	int i, c, d;
	int jots = 0;
	string word = randomWord;
	for (i = 0; i <= 4; i++) {
		for (c = 0; c <= 4; c++) {
			if ((word[c] == guessedWord[i] || word[c] == (char)((int)(guessedWord[i]) - 32))) {

				for (d = c; d <= 3; d++) {
					word[d] = word[d + 1];
				}
				word[4] = ' ';

				jots++;
				c = 5;
			}
		}
	}
	return jots;
}
string Jotto::GetRandomWord() {
	return randomWord;
}
void Jotto::SetRandomWord() {
	randomWord = arr_words[r.RandInt(0, arr_words_length)];
	randomWord = arr_words[r.RandInt(0, arr_words_length)];
}
void Jotto::drawSheet() {
	wcout << L" Score:        Seceret Word               " << endl;
	wcout << L"  0000   ╔═══╗╔═══╗╔═══╗╔═══╗╔═══╗        " << endl;
	wcout << L"         ║ * ║║ * ║║ * ║║ * ║║ * ║        " << endl;
	wcout << L"         ╚═══╝╚═══╝╚═══╝╚═══╝╚═══╝        " << endl;
	wcout << L"┌───────┐                         ┌──────┐" << endl;
	wcout << L"│ Score │     Your Test Words     │ Jots │" << endl;
	wcout << L"│ ↓ ↓ ↓ │     ↓      ↓      ↓     │ ↓  ↓ │" << endl;
	wcout << L"└───────┘                         └──────┘" << endl;
	for (int i = 0; i < 20; i++)
	{
		wcout << L"┌───────┐" << L"┌╌╌╌┐┌╌╌╌┐┌╌╌╌┐┌╌╌╌┐┌╌╌╌┐┌──────┐" << endl;
		wcout << L"│  " << right << setfill(L' ') << setw(3) << scoring[i] << L"  │" << L"╎   ╎╎   ╎╎   ╎╎   ╎╎   ╎│      │" << endl;
		wcout << L"└───────┘" << L"└╌╌╌┘└╌╌╌┘└╌╌╌┘└╌╌╌┘└╌╌╌┘└──────┘" << endl;
	}
	SetConsoleCursorPosition(console, { 2, 1 });
	wcout << right << setfill(L'0') << setw(4) << score;
}
void Jotto::FillRow() {
	if (gameRound > 19) return;
	short row = short(gameRound * 3 + 9);
	int paddingLeft = 11;

	for (int i = 0; i < 5; i++)
	{
		SetConsoleCursorPosition(console, { short(i * 5 + paddingLeft), row });
		wcout << guessedWord[i];
	}
	SetConsoleCursorPosition(console, { 37, row });
	wcout << GetNumOfJots();
	gameRound++;
}
wstring Jotto::To_wstring(string str) {
	wstring str2(str.length(), L' '); // Make room for characters
	// Copy string to wstring.
	copy(str.begin(), str.end(), str2.begin());
	return str2;
}
void Jotto::GetInput() {
	short row = short(gameRound * 3 + 9);
	do {
		SetConsoleCursorPosition(console, { 42, row });
		wcout << "Enter Your Guess:                            ";
		SetConsoleCursorPosition(console, { 60, row });
		cin >> guessedWord;
		if (strlen(guessedWord.c_str()) == 5) {
			break;
		}
		else {
			if (guessedWord == "cheats") {
				ShowWord();
			}
			else if (guessedWord == "skip"){
				gameRound = 20;
				break;
			}
			else {
				SetConsoleCursorPosition(console, { 42, row - 1 });
				wcout << "Must use 5 letters";
			}
		}
	} while (true);
	SetConsoleCursorPosition(console, { 42, row - 1 });
	wcout << "                     ";
}
void Jotto::ResetRounds() {
	gameRound = 0;
}
bool Jotto::PlayAgain() {
	short row = short((gameRound - 1) * 3 + 9);
	char ans;
	SetConsoleCursorPosition(console, { 42, row + 1 });
	wcout << "Play Again? (y/n) ";
	cin >> ans;
	system("CLS");
	if (ans == 'y' || ans == 'Y') {
		return true;
	}
	else {
		return false;
	}
}
bool Jotto::OutOfTurns() {
	short row = short(gameRound * 3 + 9);
	if (gameRound > 19) {
		SetConsoleCursorPosition(console, { 42, row });
		wcout << "No more turns! The word was " << To_wstring(randomWord);
		return true;
	}
	else {
		return false;
	}
}
void Jotto::ShowWord() {
	for (int i = 0; i < 5; i++)
	{
		SetConsoleCursorPosition(console, { short(i * 5 + 11), 2 });
		wcout << randomWord[i];
	}
}

#endif





