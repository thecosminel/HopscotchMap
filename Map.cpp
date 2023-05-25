#include "Map.h"
#include "MapIterator.h"


//Remove at the end
#include "iostream"
using std::cout;
using std::endl;
// !!!


Map::Map()
{
    this->m = 5;
    this->hopRange = 3;
    this->currentSize = 0;
    table = new Bucket[m];
}

Map::~Map()
{
	delete[] table;
}

TValue Map::add(TKey k, TValue v)
{
    int hashPosition = hashFunction(k, 0);
    for (int hop = 0; hop < hopRange; ++hop)
    {
        int position = hashFunction(k, hop);
        if (table[position].element == NULL_TELEM)
        {
            table[position].element.first = k;
            table[position].element.second = v;
            table[hashPosition].bitMap |= (1 << hop);
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
        hashPosition = hashFunction(k, 0);
        short unsigned int bitMap = table[hashPosition].bitMap;
        for (int hop = 0; hop < hopRange; ++hop)
        {
            if ((bitMap & (1 << hop)) == 0)
            {
                int currentPosition = hashFunction(k, hop);
                TElem currentElem = table[currentPosition].element;
                TKey currentKey = currentElem.first;
                int expectedCurrentElemPosition = hashFunction(currentKey, 0);
                for (int j = 0; j < hopRange; ++j)
                {
                    int newPosition = hashFunction(currentKey, j);
                    if (table[newPosition].element == NULL_TELEM)
                    {
                        table[newPosition].element = currentElem;
                        // Find elem to move position in bitmap
                        int elemToMovePostionInBitMap = NULL_TKEY;
                        for (int l = 0; l < hopRange; ++l)
                        {
                            if (hashFunction(currentKey, l) == currentPosition)
                            {
                                elemToMovePostionInBitMap = l;
                                break;
                            }
                        }
                        table[expectedCurrentElemPosition].bitMap &= ~(1 << elemToMovePostionInBitMap); // Set bit to 0
                        table[expectedCurrentElemPosition].bitMap |= (1 << j); // Set bit to 1
                        // Put new
                        table[hashPosition].bitMap |= (1 << hop);
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
}

TValue Map::search(TKey k) const
{
    int expectedPosition = hashFunction(k, 0);
    int currentBitMap = table[expectedPosition].bitMap;
    for (int hop = 0; hop < hopRange; ++hop)
    {

        unsigned short int thing = (1 & (currentBitMap >> hop));
        if ((1 & (currentBitMap >> hop)) == 1)
        {
            int position = hashFunction(k, hop);
            if (k == table[position].element.first)
                return table[position].element.second;
        }
    }
	return NULL_TVALUE;
}

TValue Map::remove(TKey k)
{
    int expectedPosition = hashFunction(k, 0);
    int currentBitMap = table[expectedPosition].bitMap;
    for (int hop = 0; hop < hopRange; ++hop)
    {
        if ((1 & (currentBitMap >> hop)) == 1)
        {
            int position = hashFunction(k, hop);
            if (k == table[position].element.first)
            {
                TValue removedValue = table[position].element.second;
                table[position].element = NULL_TELEM;
                table[expectedPosition].bitMap &= ~(1 << hop);
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
    int oldM = m;
    m = 2*m;
    auto* oldTable = table;
    table = new Bucket[1];
    // Increment and rehash until a compatible m is found
    bool rehashResult;
    do {
        delete[] table;
        m = find_next_prime(m);
        table = new Bucket[m];
        rehashResult = rehash(oldTable, oldM);
    } while (!rehashResult);

    delete[] oldTable;
}

bool Map::rehash(Bucket* oldTable, int oldM)
{
    for (int i = 0; i < oldM; ++i)
    {
        TElem currentElement = oldTable[i].element;
        if (currentElement != NULL_TELEM)
        {
            bool result = addWithoutResize(currentElement.first, currentElement.second);
            if (!result)
            {
                return false;
            }
        }
    }
    return true;
}



void Map::printHashTableAndBitmap() const
{
    for (int i = 0; i < m; ++i)
    {
        Bucket currentBucket = table[i];
        cout << "[" << i << "] =  ";
        for(int j = 0; j < hopRange; j++)
        {
            cout << ((currentBucket.bitMap >>  j) & 1);
        }
        cout << " | ";
        cout << " k: " << currentBucket.element.first << "  v: " << currentBucket.element.second << endl;
    }
    cout << endl << endl;
}

int Map::hashFunction(int key, int h) const
{
    if (key >= 0)
        return (key + h) % m;
    else
        return (m + (key % m) + h) % m;
}

bool Map::addWithoutResize(TKey k, TValue v)
{
    int hashPosition = hashFunction(k, 0);
    for (int hop = 0; hop < hopRange; ++hop)
    {
        int position = hashFunction(k, hop);
        if (table[position].element == NULL_TELEM)
        {
            table[position].element.first = k;
            table[position].element.second = v;
            table[hashPosition].bitMap |= (1 << hop);
            return true;
        }
        else if (table[position].element.first == k)
        {
            table[position].element.second = v;
            return false;
        }
    }

    // Try moving
    short unsigned int bitMap = table[hashPosition].bitMap;
    for (int hop = 0; hop < hopRange; ++hop)
    {
        if ((bitMap & (1 << hop)) == 0)
        {
            int currentPosition = hashFunction(k, hop);
            TElem currentElem = table[currentPosition].element;
            TKey currentKey = currentElem.first;
            int expectedCurrentElemPosition = hashFunction(currentKey, 0);
            for (int j = 0; j < hopRange; ++j)
            {
                int newPosition = hashFunction(currentKey, j);
                if (table[newPosition].element == NULL_TELEM)
                {
                    table[newPosition].element = currentElem;
                    // Find elem to move position in bitmap
                    int elemToMovePostionInBitMap = NULL_TKEY;
                    for (int l = 0; l < hopRange; ++l)
                    {
                        if (hashFunction(currentKey, l) == currentPosition)
                        {
                            elemToMovePostionInBitMap = l;
                            break;
                        }
                    }
                    table[expectedCurrentElemPosition].bitMap &= ~(1 << elemToMovePostionInBitMap);
                    table[expectedCurrentElemPosition].bitMap |= (1 << j);
                    // Put new
                    table[hashPosition].bitMap |= (1 << hop);
                    table[currentPosition].element.first = k;
                    table[currentPosition].element.second = v;
                    return true;
                }
            }
        }
    }
    return false;
}


