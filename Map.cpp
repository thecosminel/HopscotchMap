#include "Map.h"
#include "MapIterator.h"


Map::Map()
{
    this->hop = 3;
    this->m = 5;
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
    while (true)
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
        resize();
    }
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

void Map::resize()
{
    int mCopy = m;
    int newM;
    TElem* newTable = nullptr;
    // Create new
    do
    {
        newM = find_next_prime(mCopy);
        newTable = rehash(newM);
    } while (newTable == nullptr);

    delete[] table;
    table = newTable;
    this->m = newM;
}

TElem *Map::rehash(int newM) const
{
    auto newTable = new TElem[newM];
    for (int i = 0; i < newM; ++i)
    {
        newTable[i] = NULL_TELEM;
    }
    TElem elementToRehash;
    // Rehash elements one by one
    for (int index = 0; index < m; ++index)
    {
        bool added;
        elementToRehash = table[index];
        TKey k = elementToRehash.first;
        if (k != NULL_TKEY)
        {
            added = false;
            for (int i = 0; i < hop; ++i)
            {
                int positionInTable = hashFunction(k + i);
                TElem currentTElem= newTable[positionInTable];
                if (currentTElem.first == NULL_TKEY)
                {
                    newTable[positionInTable] = elementToRehash;
                    added = true;
                    break;
                }
            }
            if (!added)
            {
                // Try moving the other positions in hop range
                for (int i = 0; i < hop; ++i)
                {
                    int currentElemToMovePosition = hashFunction(k + i);
                    TElem currentElemToMove =newTable[currentElemToMovePosition];
                    for (int j = 0; j < hop; ++j)
                    {
                        int targetPosition = hashFunction(currentElemToMove.first + j);
                        TElem targetElement = newTable[targetPosition];
                        if (targetElement.first == NULL_TKEY)
                        {
                            newTable[targetPosition] = currentElemToMove;
                            newTable[currentElemToMovePosition] = elementToRehash;
                            added = true;
                            break;
                        }
                    }
                }
            }
            // Add failed, needs resize again
            if (!added)
            {
                delete[] newTable;
                return nullptr;
            }
        }
    }
    return newTable;
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