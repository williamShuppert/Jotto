#ifndef _TOPTEN_H
#define _TOPTEN_H

#include <string>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <fstream>

using namespace std;

class TopTen {
public:
	TopTen();
	TopTen(string filename);

	bool IsTopTen(int points);
	void AddScore(int points, string name);
	void drawTopTen();
	void SetMaxNameLength(int length);
	void SetRequiredNameLength(int length);
	void SaveTxtFile();
	string GetName();
private:
	// name
	int requiredNameLength = 0;
	int maxNameLength = 3;
	// list
	int paddingMiddle = 3;
	int scoreLength = 5;
	wchar_t scoreFill = L' ';

	string toptenFilename;
	wstring to_wstring(string str);
	string name[10];
	string score[10];
};

TopTen::TopTen() {
	_setmode(_fileno(stdout), _O_U16TEXT); // fixes stupid unicode
	for (int i = 0; i < 10; i++)
	{
		name[i] = "---";
		score[i] = "---";
	}
}
TopTen::TopTen(string filename) {
	_setmode(_fileno(stdout), _O_U16TEXT); // fixes stupid unicode
	toptenFilename = filename;
	ifstream infile(filename);
	string str;
	int i = 0;
	if (infile) {
		while (getline(infile, str)) {
			if (i <= 9) {
				name[i] = str;
			}
			else if (i <= 19) {
				score[i - 10] = str;
			}
			i++;
		}
		infile.close();
	}
	else
	{
		wcout << "Error opening file.\n";
	}
}

// top ten chart looks
void TopTen::drawTopTen() {
	for (int i = 0; i < 10; i++)
	{
		wcout << right << setfill(L' ') << setw(maxNameLength) << to_wstring(name[i]) << setfill(L' ') << setw(paddingMiddle) << "" << left << setfill(scoreFill) << setw(scoreLength) << to_wstring(score[i]) << endl;
	}
}
void TopTen::SetMaxNameLength(int length) {
	maxNameLength = length;
	requiredNameLength = 0;
}
void TopTen::SetRequiredNameLength(int length) {
	maxNameLength = length;
	requiredNameLength = length;
}

string TopTen::GetName() {
	string playerName;
	do {
		wcout << "Must have " << requiredNameLength << " letters." << endl;
		wcout << "Enter Name: "; cin >> playerName;
		system("CLS");
	} while (strlen(playerName.c_str()) != requiredNameLength);
	return playerName;
}
void TopTen::AddScore(int points, string alias)
{
	for (int i = 0; i < 10; i++)
	{
		if (score[i] == "---" || points > stoi(score[i])) {

			for (int x = 9; x > i; x--)
			{
				score[x] = score[x - 1];
				name[x] = name[x - 1];
			}
			score[i] = to_string(points);
			name[i] = alias;
			break;
		}
	}
	SaveTxtFile();
}
bool TopTen::IsTopTen(int points) {
	if (points > stoi(score[9])) {
		return true;
	}
	return false;
}
void TopTen::SaveTxtFile() {
	std::ofstream ofs;
	ofs.open(toptenFilename, std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < 2; i++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (i == 1) {
				ofs << score[x] << endl;
			}
			else {
				ofs << name[x] << endl;
			}
		}
	}
	ofs.close();

}
wstring TopTen::to_wstring(string str) {
	wstring str2(str.length(), L' '); // Make room for characters
	// Copy string to wstring.
	copy(str.begin(), str.end(), str2.begin());
	return str2;
}


#endif

