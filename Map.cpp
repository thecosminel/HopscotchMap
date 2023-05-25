#include "Map.h"
#include "MapIterator.h"


Map::Map()
{
    this->hop = 3;
    this->m = 5;
    this->currentSize = 0;
    table = new Bucket[m];
    for (int i = 0; i < m; ++i)
    {
        table[i] = Bucket(hop);
    }
}

Map::~Map() {
	delete[] table;
}

TValue Map::add(TKey k, TValue v)
{
    int expectedPosition = hashFunction(k, 0);
    for (int i = 0; i < hop; ++i)
    {
        int position = hashFunction(k, i);
        if (table[position].element == NULL_TELEM)
        {
            table[position].element.first = k;
            table[position].element.second = v;
            table[expectedPosition].bitMap[i] = 1;
            currentSize++;
            return NULL_TVALUE;
        }
        else if (table[position].element.first == k)
        {
            TValue oldValue = table[position].element.second;
            table[position].element.second = v;
            return oldValue;
        }
    }
    // Try moving
    while (true)
    {
        int* bitMap = table[expectedPosition].bitMap;
        for (int i = 0; i < hop; ++i)
        {
            if (bitMap[i] == 0)
            {
                int currentPosition = hashFunction(k,i);
                TElem currentElem = table[currentPosition].element;
                TKey currentKey = currentElem.first;
                int expectedCurrentElemPosition = hashFunction(currentKey, 0);
                for (int j = 0; j < hop; ++j)
                {
                    int newPosition = hashFunction(currentKey, j);
                    if (table[newPosition].element == NULL_TELEM)
                    {
                        table[newPosition].element = currentElem;
                        // Find elem to move position in bitmap
                        int elemToMovePostionInBitMap = NULL_TKEY;
                        for (int l = 0; l < hop; ++l)
                        {
                            if (hashFunction(currentKey, l) == currentPosition)
                            {
                                elemToMovePostionInBitMap = l;
                                break;
                            }
                        }
                        table[expectedCurrentElemPosition].bitMap[elemToMovePostionInBitMap] = 0;
                        table[expectedCurrentElemPosition].bitMap[j] = 1;
                        // Put new
                        bitMap[i] = 1;
                        table[currentPosition].element.first = k;
                        table[currentPosition].element.second = v;
                        currentSize++;
                        return NULL_TVALUE;
                    }
                }
            }
        }
        // If moving failed, rehash is needed;
        resize();
    }
    return NULL_TVALUE;
}

TValue Map::search(TKey k) const
{
    int expectedPosition = hashFunction(k, 0);
    for (int i = 0; i < hop; ++i)
    {
        if (table[expectedPosition].bitMap[i] == 1)
        {
            int position = hashFunction(k, i);
            if (k == table[position].element.first)
                return table[position].element.second;
        }
    }
	return NULL_TVALUE;
}

TValue Map::remove(TKey k)
{
    int expectedPosition = hashFunction(k, 0);
    for (int i = 0; i < hop; ++i)
    {
        if (table[expectedPosition].bitMap[i] == 1)
        {
            int position = hashFunction(k, i);
            if (k == table[position].element.first)
            {
                TValue removedValue = table[position].element.second;
                table[position].element = NULL_TELEM;
                table[expectedPosition].bitMap[i] = 0;
                currentSize--;
                return removedValue;
            }
        }
    }
    return NULL_TVALUE;
}


int Map::size() const
{
    return currentSize;
}

bool Map::isEmpty() const
{
	if (currentSize == 0)
        return true;
	return false;
}

MapIterator Map::iterator() const
{
	return MapIterator(*this);
}

void Map::resize()
{
    int newM = 2*m;
    Bucket* newTable;
    // Increment and rehash until a compatible m is found
    do {
        newM = find_next_prime(newM);
        newTable = rehash(newM);
    } while (newTable != nullptr);

    delete[] table;
}

Bucket* Map::rehash(int newM) const
{
    auto* newTable = new Bucket[newM];
    newTable = new Bucket[m];
    for (int i = 0; i < m; ++i)
    {
        newTable[i] = Bucket(hop);
    }

    return nullptr;
}


#include "iostream"
using std::cout;
using std::endl;
void Map::printHashTableAndBitmap() const
{
    for (int i = 0; i < m; ++i)
    {
        Bucket currentBucket = table[i];
        cout << "[" << i << "] =  ";
        for(int j = 0; j < hop; j++)
        {
            cout << currentBucket.bitMap[j];
        }
        cout << " | ";
        cout << " k: " << currentBucket.element.first << "  v: " << currentBucket.element.second << endl;
    }
    cout << endl << endl;
}

int Map::hashFunction(int key, int h) const {
    if (key >= 0)
        return (key + h) % m;
    else
        return (m + (key % m) + h) % m;
}