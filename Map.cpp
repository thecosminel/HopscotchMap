#include "Map.h"
#include "MapIterator.h"


Map::Map()
{
    this->hop = 5;
    this->m = 13;
    table = new TElem [m];
    for (int i = 0; i < m; ++i)
    {
        table[i] = NULL_TELEM;
    }
}

Map::~Map() {
	delete[] table;
}

TValue Map::add(TKey k, TValue v)
{
    TElem newElement;
    newElement.first = k;
    newElement.second = v;
    // If an empty position is found
    for (int i = 0; i < hop; ++i)
    {
        int positionInTable = hashFunction(k + i);
        TElem currentTElem= table[positionInTable];
        if (currentTElem.first == NULL_TKEY)
        {
            table[positionInTable] = newElement;
            return NULL_TVALUE;
        }
        else if (currentTElem.first == k)
        {
            TValue oldValue = currentTElem.second;
            table[positionInTable].second = v;
            return oldValue;
        }
    }
    // Try moving the other positions in hop range
    for (int i = 0; i < hop; ++i)
    {
        int currentElemToMovePosition = hashFunction(k + i);
        TElem currentElemToMove = table[currentElemToMovePosition];
        for (int j = 0; j < hop; ++j)
        {
            int targetPosition = hashFunction(currentElemToMove.first + j);
            TElem targetElement = table[targetPosition];
            if (targetElement.first == NULL_TKEY)
            {
                table[targetPosition] = currentElemToMove;
                table[currentElemToMovePosition] = newElement;
                return NULL_TVALUE;
            }
        }
    }
    // If no position was found
    resizeAndRehash();
    // Try again after rehash
    return add(k, v);
}

TValue Map::search(TKey k) const
{
    for (int i = 0; i <= hop; ++i)
    {
        int positionInTable = hashFunction(k+i);
        if (table[positionInTable].first == k)
        {
            TValue returnValue = table[positionInTable].second;
            return returnValue;
        }
    }
	return NULL_TVALUE;
}

TValue Map::remove(TKey k)
{
    TElem removedEntry = NULL_TELEM;
    int positionInTable = hashFunction(k);
    for (int i = 0; i <= hop; ++i)
    {
        if (table[positionInTable].first != NULL_TKEY)
        {
            removedEntry = table[positionInTable];
            table[positionInTable] = NULL_TELEM;
        }
    }
	return removedEntry.second;
}


int Map::size() const
{
    int size = 0;
    for (int i = 0; i < m; ++i)
    {
        if (table[i].first != NULL_TKEY)
            size++;
    }
	return size;
}

bool Map::isEmpty() const
{
	if (size() == 0)
        return true;
	return false;
}

MapIterator Map::iterator() const
{
	return MapIterator(*this);
}

void Map::resizeAndRehash()
{
    // Save old
    int oldM = m;
    TElem * oldTable = table;
    // Create new
    this->m = find_next_prime(m);
    table = new TElem [m];
    for (int i = 0; i < m; ++i)
    {
        table[i] = NULL_TELEM;
    }
    // Rehash
    for (int i = 0; i < oldM; ++i)
    {
        TElem currentTElem = oldTable[i];
        if (currentTElem.first != NULL_TKEY)
        {
            if(!addWithoutRehash(currentTElem.first, currentTElem.second))
            {
                m = oldM;
                delete[] table;
                table = oldTable;
                resizeAndRehash();
                return;
            }
        }
    }
    // Finish
    delete[] oldTable;
}

#include "iostream"
using std::cout;
using std::endl;
void Map::printHashTable() const
{
    for (int i = 0; i < m; ++i)
    {
        cout << "[" << i << "] = k: " << table[i].first << "  v: " << table[i].second << endl;
    }
    cout << endl << endl;
}

int Map::hashFunction(int key) const {
    if (key >= 0)
        return key % m;
    else
        return (-key) % m;
}

bool Map::addWithoutRehash(TKey k, TValue v)
{
    TElem newElement;
    newElement.first = k;
    newElement.second = v;
    // If an empty position is found
    for (int i = 0; i < hop; ++i)
    {
        int positionInTable = hashFunction(k + i);
        TElem currentTElem= table[positionInTable];
        if (currentTElem.first == NULL_TKEY)
        {
            table[positionInTable] = newElement;
            return NULL_TVALUE;
            return true;
        }
        else if (currentTElem.first == k)
        {
            TValue oldValue = currentTElem.second;
            table[positionInTable].second = v;
            return oldValue;
            return true;
        }
    }
    // Try moving the other positions in hop range
    for (int i = 0; i < hop; ++i)
    {
        int currentElemToMovePosition = hashFunction(k + i);
        TElem currentElemToMove = table[currentElemToMovePosition];
        for (int j = 0; j < hop; ++j)
        {
            int targetPosition = hashFunction(currentElemToMove.first + j);
            TElem targetElement = table[targetPosition];
            if (targetElement.first == NULL_TKEY)
            {
                table[targetPosition] = currentElemToMove;
                table[currentElemToMovePosition] = newElement;
                return NULL_TVALUE;
                return true;
            }
        }
    }
    // If no position was found
    return false;
}



