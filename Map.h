#pragma once
#include <utility>
//DO NOT INCLUDE MAPITERATOR

#include "auxiliaryFunctions.h"

//DO NOT CHANGE THIS PART
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TKEY (-111111)
#define NULL_TVALUE (-111111)
#define NULL_TELEM std::pair<TKey, TValue>(-111111, -111111)
class MapIterator;

class Map {
	//DO NOT CHANGE THIS PART
	friend class MapIterator;

private:
    TElem * table;
    int m; // Capacity
    int hop;

    void resize();
    TElem* rehash(int newM) const;
    int hashFunction(int key) const;
public:

	// implicit constructor
	Map();

	// adds a pair (key,value) to the map
	//if the key already exists in the map, then the value associated to the key is replaced by the new value and the old value is returned
	//if the key does not exist, a new pair is added and the value null is returned
	TValue add(TKey k, TValue v);

	//searches for the key and returns the value associated with the key if the map contains the key or null: NULL_TVALUE otherwise
	TValue search(TKey k) const;

	//removes a key from the map and returns the value associated with the key if the key existed ot null: NULL_TVALUE otherwise
	TValue remove(TKey k);

	//returns the number of pairs (key,value) from the map
	int size() const;

	//checks whether the map is empty or not
	bool isEmpty() const;

	//returns an iterator for the map
	MapIterator iterator() const;

	// destructor
	~Map();

    void printHashTable() const;

};



