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

struct Bucket{
    TElem element;
    int* bitMap{};
    Bucket() : element(NULL_TELEM){}
    explicit Bucket(int h){
        element = NULL_TELEM;
        bitMap = new int[h];
        for (int i = 0; i < h; ++i)
            bitMap[i] = 0;
    }
    ~Bucket() {
        delete[] bitMap;
    }
};

class Map {
	//DO NOT CHANGE THIS PART
	friend class MapIterator;

private:
    Bucket* table;
    int m; // Capacity
    int currentSize; // Size
    int hop;

    int hashFunction(int key, int h) const;
    void resize();
    bool rehash(Bucket* newTable, int oldM);
    bool addWithoutResize(Bucket* newTable, TKey k, TValue v);

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

    void printHashTableAndBitmap() const;

};



