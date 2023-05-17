#include "ExtendedTest.h"
#include "ShortTest.h"

#include "Map.h"

#include <iostream>
using namespace std;
#include "cassert"

int main() {
	testAll();
	testAllExtended();
	cout << "That's all!" << endl;
//	system("pause");


//    Map m;
//    assert(m.isEmpty() == true);
//    assert(m.size() == 0); //add elements
//    assert(m.add(5,5)==NULL_TVALUE);
//    assert(m.add(1,111)==NULL_TVALUE);
//    assert(m.add(10,110)==NULL_TVALUE);
//    assert(m.add(7,7)==NULL_TVALUE);
//    assert(m.add(1,1)==111);
//    assert(m.add(10,10)==110);
//    m.printHashTable();
//    assert(m.add(-3,-3)==NULL_TVALUE);
//    m.printHashTable();
////    assert(m.size() == 5);
////    assert(m.search(10) == 10);
////    assert(m.search(16) == NULL_TVALUE);
////    assert(m.remove(1) == 1);
////    assert(m.remove(6) == NULL_TVALUE);
////    assert(m.size() == 4);
	return 0;
}


