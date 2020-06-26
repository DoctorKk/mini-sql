#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
    string Path("h");
	string Path2("ello.txt");
	Path += Path2;
	ofstream out(Path,ios::app);
	out << "tsetaesdf" << endl;
	out.close();
}
