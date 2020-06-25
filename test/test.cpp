#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
		ifstream shit("./hello.txt");
		string temp;
		while (getline(shit, temp)) {
				cout << temp << endl;

		}

		ofstream tt("./hello.txt", ios::app);
		char* test = "dslfhsdhlf";
		tt << test;
}
