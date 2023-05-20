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
    // Implement further
    currentSize++;
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
        newTable = rehash(newM);
        newM = find_next_prime(newM);
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
}


#include "iostream"
using std::cout;
using std::endl;
void Map::printHashTableAndBitmap() const
{
    for (int i = 0; i < m; ++i)
    {
        cout << "[" << i << "] =  ";
        for(int j = 0; j < hop; j++)
        {
            cout << table[i].bitMap[j];
        }
        cout << " | ";
        cout << " k: " << table[i].element.first << "  v: " << table[i].element.second << endl;
    }
    cout << endl << endl;
}

int Map::hashFunction(int key, int h) const {
    if (key >= 0)
        return (key + h) % m;
    else
        return (m + (key % m) + h) % m;
}