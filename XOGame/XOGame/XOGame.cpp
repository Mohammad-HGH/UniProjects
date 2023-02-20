// XOGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<int> changer(vector<int> mainVec, int first, int second);
class XOGame {
private:
	int table[3][3][3]{
		{{0,0,0}, {0,0,0}, {0,0,0}},
		{{0,0,0}, {0,0,0}, {0,0,0}},
		{{0,0,0}, {0,0,0}, {0,0,0}}
	};
	int lastTable[3][3]{
		{0,0,0},
		{0,0,0},
		{0,0,0}
	};
	void gameSet() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				bool flag = false;
				for (int k = 2; k >= 0; k--)
				{
					if (table[i][j][k] == 1)
					{
						lastTable[i][j] = 1;
						flag = true;
						break;
					}
					else if (table[i][j][k] == 2)
					{
						lastTable[i][j] = 2;
						flag = true;
						break;
					}
				}
				if (!flag)
					lastTable[i][j] = 0;
			}
		}
	}
	void winChecker() {
		for (int i = 0; i < 3; i++) {
			bool flag = false;
			if (lastTable[i][0] == 1 && lastTable[i][1] == 1 && lastTable[i][2] == 1)
				flag = true;

			if (!flag)
				continue;
			else {
				cout << "RED WON!!!";
				system("pause");
				exit(0);
			}

		}
		for (int j = 0; j < 3; j++) {
			bool flag = false;
			if (lastTable[0][j] == 1 && lastTable[1][j] == 1 && lastTable[2][j] == 1)
				flag = true;

			if (!flag)
				continue;
			else {
				cout << "RED WON!!!";
				system("pause");
				exit(0);
			}
		}

		bool flag = false;
		if (lastTable[0][0] == 1 && lastTable[1][1] == 1 && lastTable[2][2] == 1)
			flag = true;
		else if (lastTable[2][0] == 1 && lastTable[1][1] == 1 && lastTable[0][2] == 1)
			flag = true;
		if (flag) {
			cout << "RED WON!!!";
			system("pause");
			exit(0);
		}
		for (int i = 0; i < 3; i++) {
			bool flag = false;
			if (lastTable[i][0] == 2 && lastTable[i][1] == 2 && lastTable[i][2] == 2)
				flag = true;

			if (!flag)
				continue;
			else {
				cout << "BLUE WON!!!";
				system("pause");
				exit(0);
			}

		}
		for (int j = 0; j < 3; j++) {
			bool flag = false;
			if (lastTable[0][j] == 2 && lastTable[1][j] == 2 && lastTable[2][j] == 2)
				flag = true;

			if (!flag)
				continue;
			else {
				cout << "BLUE WON!!!";
				system("pause");
				exit(0);
			}
		}
		flag = false;
		if (lastTable[0][0] == 2 && lastTable[1][1] == 2 && lastTable[2][2] == 2)
			flag = true;
		else if (lastTable[2][0] == 2 && lastTable[1][1] == 2 && lastTable[0][2] == 2)
			flag = true;
		if (flag) {
			cout << "BLUE WON!!!";
			system("pause");
			exit(0);
		}
	}
	void displayTable() {
		system("cls");
		gameSet();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (lastTable[i][j] == 1)
					cout << "R\t";
				else if (lastTable[i][j] == 2)
					cout << "B\t";
				else
					cout << "0\t";
			}
			cout << endl;
		}
	}
	void errorCondition() {
		cout << "you have moved wrongly, you lost your turn\n";
	}
	void errorCondition(string error) {
		cout << "you have moved wrongly, you lost your turn(" << error << ").\n";
	}
	void tableSetter(int x, int y, int priority, string player) {
		if (player == "RED") {
			bool flag = false;
			for (int i = 3; i > priority; i--)
				if (table[x][y][i] == 1 || table[x][y][i] == 2)
					flag = true;

			if (!flag)
				table[x][y][priority] = 1;
			else
			{
				errorCondition();
			}
		}
		else if (player == "BLUE")
		{
			bool flag = false;
			for (int i = 2; i >= priority; i--)
				if (table[x][y][i] == 1 || table[x][y][i] == 2)
					flag = true;

			if (!flag)
				table[x][y][priority] = 2;
		}
		else
			errorCondition();
	}
	vector <int> redPieces = { 1, 1, 2, 2, 3, 3 };
	vector <int> bluePieces = { 1, 1, 2, 2, 3, 3 };
public:
	void starter() {
		int x, y, priority;
		int x2, y2;
		XOGame XO;
		cout << "Choose your case, BLUE!:\n ";
		cout << "1. use new pieces\n";
		cout << "2. move your pieces\n";
		int c; //Case
		cin >> c;
		if (c == 1) {
			cout << "set your position and your piece, BLUE\n";
			cin >> y >> x >> priority;
			XOSet(x, y, priority, "BLUE");

		}
		else if (c == 2) {
			cout << "type the change(first X first, then first Y, then secondary X, then secondary Y, and at last priority .\n ";
			cin >> y >> x >> y2 >> x2 >> priority;
			XOChanger(x, y, x2, y2, priority, "BLUE");

		}
		cout << "Choose your case, RED!:\n ";
		cout << "1. use new pieces\n";
		cout << "2. move your pieces\n";
		cin >> c;
		if (c == 1) {
			cout << "set your position and your piece, RED";
			cin >> y >> x >> priority;
			XOSet(x, y, priority, "RED");

		}
		else if (c == 2) {
			cout << "type the position of your piece and then the new pos and the priority of the piece.\n ";
			cin >> y >> x >> y2 >> x2 >> priority;
			XOChanger(x, y, x2, y2, priority, "RED");

		}
	}
	void XOGamestart() {
		int x, y, priority;
		XOGame XO;
		displayTable();
		cout << "set your position and your piece, BLUE\n";
		cin >> y >> x >> priority;
		XOSet(x, y, priority, "BLUE");

		cout << "set your position and your piece, RED\n";
		cin >> y >> x >> priority;
		XOSet(x, y, priority, "RED");

	}
	int XOSet(int x, int y, int priority, string player) {
		bool flag = false;
		if (player == "RED") {
			for (int i = 0; i < redPieces.size(); i++) {
				if (redPieces[i] == priority) {
					if (table[x][y][priority - 1] == 0) {
						table[x][y][priority - 1] = 1;

					}
					else {
						errorCondition();
						return 0;
					}
					redPieces = changer(redPieces, i, redPieces.size() - 1);
					redPieces.pop_back();
					flag = true;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < bluePieces.size(); i++) {
				if (bluePieces[i] == priority) {
					if (table[x][y][priority - 1] == 0) {
						table[x][y][priority - 1] = 2;

					}
					else {
						errorCondition();
						return 0;
					}
					bluePieces = changer(redPieces, i, redPieces.size() - 1);
					bluePieces.pop_back();
					flag = true;
					break;
				}

			}
		}
		if (!flag)
			errorCondition("No more piece with this priority!");

		tableSetter(x, y, priority - 1, player);
		displayTable();
		winChecker();
	}
	int XOChanger(int x, int y, int x2, int y2, int priority, string player) {
		bool flag = false;
		for (int i = 2; i > priority - 1; i--)
			if (table[x][y][i] != 0)
				flag = true;
		if (!flag)
		{
			table[x2][y2][priority - 1] = table[x][y][priority - 1];
			table[x][y][priority - 1] = 0;
		}
		else
		{
			errorCondition();
			return 0;
		}
		tableSetter(x2, y2, priority, player);
		displayTable();
		winChecker();
	}
};


int main() {
	XOGame XO;
	XO.XOGamestart();
	cout << endl;
	while (true) {
		XO.starter();
	}
}
vector<int> changer(vector<int> mainVec, int first, int second) {
	int temp;
	temp = mainVec[first];
	mainVec[first] = mainVec[second];
	mainVec[second] = temp;
	return  mainVec;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
