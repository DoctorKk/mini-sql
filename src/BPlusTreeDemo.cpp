#include"BPlusTree.cpp"

using namespace std;

int main() {
	BPlusTree<int>* test = new BPlusTree<int>;
	test -> ClearTree();
	test -> Insert(make_pair(56, 80));
    test -> Insert(make_pair(89, 90));
    test -> Insert(make_pair(8, 70));
    test -> Insert(make_pair(6, 99));
    test -> Insert(make_pair(7, 77));
    test-> PrintTree();
    cout << endl;
    cout << test -> Search(90);
    test -> Delete(70);
}
