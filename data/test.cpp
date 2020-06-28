#include<iostream>
#include<string>
#include<fstream>

using namespace std;

int main() {
		ifstream in("./test");
		in.seekg(0, ios::end);
		int length = in.tellg();
		in.seekg(0, ios::beg);
		char* test = new char[length];
		in.read(test, 4);
		cout << test << endl;
		delete[] test;

		test = new char[length];
		in.read(test, 2);
		cout << test << endl;
		
}
