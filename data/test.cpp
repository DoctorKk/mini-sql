#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
<<<<<<< Updated upstream
		ifstream in("teacher");
		if (!in)
				cout << "shit" << endl;
		string temp;
		while (!in.eof()) {
				getline(in, temp);
				cout << temp << flush;
		}
=======
		ofstream out("teacher", ios::out);
		string place("/", 10);
		char* temp = (char* )place.c_str();
		out << temp << endl;		
>>>>>>> Stashed changes
}
