#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
		string test = "124321";
		stringstream fuck;
		fuck << test;
		int shit;
		fuck >> shit;
		cout << shit << endl;

}
