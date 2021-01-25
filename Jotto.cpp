// Jotto.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Jotto: a dumb word game
// Will Shuppert

#include "pch.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
// jotto
#include "Jotto.h"
// top ten
#include "TopTen.h"
// for unicode
#include <fcntl.h>
#include <io.h>

using namespace std;

int main()
{
	TopTen topten("topTen.txt");
	topten.SetRequiredNameLength(3);
	topten.drawTopTen();
	system("pause");
	system("CLS");
	Jotto jotto("jotto.txt");
	char ans;
	do {
		jotto.ResetRounds();
		jotto.drawSheet();
		jotto.SetRandomWord();
		do {
			jotto.GetInput();
			jotto.FillRow();
			if (jotto.Win())	break;
			if (jotto.OutOfTurns())	break;
		} while (true);
		jotto.ShowWord();
		if (jotto.PlayAgain() == false)	break;
	} while (true);
	system("CLS");
	if (topten.IsTopTen(jotto.GetScore()))	topten.AddScore(jotto.GetScore(), topten.GetName());
	topten.drawTopTen();
	system("pause");
}

