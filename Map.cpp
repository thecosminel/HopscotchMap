#include "Map.h"
#include "MapIterator.h"

Map::Map()
{
    this->h = 4;
    this->m = find_next_prime(h);
    arr = new Entry[m];
    for (int i = 0; i < m; ++i)
    {
        arr[i] = Entry();
    }
}

Map::~Map() {
	//TODO - Implementation
}

TValue Map::add(TKey c, TValue v){
	//TODO - Implementation
	return NULL_TVALUE;
}

TValue Map::search(TKey c) const{
	//TODO - Implementation
	return NULL_TVALUE;
}

TValue Map::remove(TKey c){
	//TODO - Implementation
	return NULL_TVALUE;
}


int Map::size() const {
	//TODO - Implementation
	return 0;
}

bool Map::isEmpty() const{
	//TODO - Implementation
	return false;
}

MapIterator Map::iterator() const {
	return MapIterator(*this);
}

#include "iostream"
using std::cout, std::endl;
void Map::printHashTable() const
{
    for (int i = 0; i < m; ++i)
    {
        cout << "[" << i << "] = " << arr[i].key << endl;
    }
}



