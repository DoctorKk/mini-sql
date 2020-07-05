#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main() {
		ifstream in("./student.txt");
		string temp;
		getline(in, temp);
		cout << temp << endl;

}
