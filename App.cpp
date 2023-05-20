#include "ExtendedTest.h"
#include "ShortTest.h"

#include "Map.h"

#include <iostream>
using namespace std;
#include "cassert"

int main() {
	testAll();
//	testAllExtended();
//	cout << "That's all!" << endl;
//	system("pause");

//    cout << "Test add" << endl;
//    Map m;

//    for (int i = 0; i < 10; i++) {
//        assert(m.add(i, i) == NULL_TVALUE);
//    }
//    assert(m.isEmpty() == false);
//    assert(m.size() == 10);
//    m.add(-10,-10);
//    m.add(-9,-9);
//    m.add(-8,-8);

    Map m;
    m.add(0,0);
    m.add(1, 1);
    m.add(2, 2);
    m.add(3, 3);
    m.printHashTableAndBitmap();
    m.add(10, 10);
    m.printHashTableAndBitmap();
	return 0;
}


