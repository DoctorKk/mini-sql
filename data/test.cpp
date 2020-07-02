#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
		ifstream in("teacher");
		if (!in)
				cout << "shit" << endl;
		string temp;
		while (!in.eof()) {
				getline(in, temp);
				cout << temp << flush;
		}
}
