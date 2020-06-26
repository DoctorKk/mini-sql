#include<iostream>
#include<fstream>
#include<string>
#include<cstdio>

using namespace std;

int main() {
    string Path("h");
	string Path2("ello.txt");
	Path += Path2;
	char* temp = "testfucking shit test\ntest\ntest\n";
	ofstream out(Path,ios::trunc);
	out << temp << endl;
	out.close();

	ifstream in(Path);
	string joke;
	char* chartemp;
	getline(in, joke);
	chartemp = (char*)joke.c_str();
	
	cout << chartemp << endl;
	cout << strlen(chartemp) << endl;
}
