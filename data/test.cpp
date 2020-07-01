#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
		ifstream in("teacher");
		if (!in)
				cout << "shit" << endl;
		in.seekg(0, in.end);
		int length = in.tellg();
		in.seekg(0, in.beg);
		cout << length << endl;
}
