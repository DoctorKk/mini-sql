#include"BPlusTree.cpp"

using namespace std;

void test1(BPlusTree<int>* test) {
    test -> ClearTree();
    test -> Insert(make_pair(6, 99));
    test -> Insert(make_pair(7, 77));
    test -> Insert(make_pair(2, 35));
    test -> Insert(make_pair(4, 89));
    test -> Insert(make_pair(2, 1));
    //test-> PrintTree();
    cout << endl;
    cout << test -> Search(89);
    test -> Delete(89);
    delete test;

}

void test2(BPlusTree<string>* test) {
    test -> ClearTree();
    test -> Insert(make_pair(6, "shit"));
    test -> Insert(make_pair(7, "fuck"));
    test -> Insert(make_pair(2, "joker"));
    test -> Insert(make_pair(4, "ceshi"));
    test -> Insert(make_pair(2, "intel"));
    test -> Insert(make_pair(9, "amd"));
    test -> Insert(make_pair(11, "apple"));
    test -> Insert(make_pair(28, "doge"));
    test -> Insert(make_pair(47, "shishi"));
    cout << endl;
    cout << test -> Search("apple");
    //test -> PrintTree();
    test -> Delete("fuck");
    delete test;

}

int main() {
    //BPlusTree<int>* test = new BPlusTree<int>;
    //test1(test);
    BPlusTree<string>* test = new BPlusTree<string>;
    test2(test);
}

