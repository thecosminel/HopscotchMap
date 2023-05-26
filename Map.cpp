#include "Map.h"
#include "MapIterator.h"


// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
int Map::hashFunction(int key, int h) const
{
    if (key >= 0)
        return (key + h) % m;
    else
        return (m + (key % m) + h) % m;
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
// That's because in constructor m=5, which is a constant
Map::Map()
{
    this->m = 5;
    this->hopRange = 3;
    this->currentSize = 0;
    table = new Bucket[m];
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
Map::~Map()
{
	delete[] table;
}

// Best case Θ(1)
// Worst case Θ(n+m)
// Average case Θ(1)
// General case O(n+m)
TValue Map::add(TKey k, TValue v)
{
    while (true)
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
        resizeUp(); // Θ(m+n)
    }
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
TValue Map::search(TKey k) const
{
    int expectedPosition = hashFunction(k, 0);
    int currentBitMap = table[expectedPosition].bitMap;
    for (int hop = 0; hop < hopRange; ++hop)
    {

        if ((1 & (currentBitMap >> hop)) == 1)
        {
            int position = hashFunction(k, hop);
            if (k == table[position].element.first)
                return table[position].element.second;
        }
    }
	return NULL_TVALUE;
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
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

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
int Map::size() const
{
    return currentSize;
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
bool Map::isEmpty() const
{
	if (currentSize == 0)
        return true;
	return false;
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
MapIterator Map::iterator() const
{
	return MapIterator(*this);
}

// Best case Θ(m+n)
// Worst case Θ(m+n)
// Average case Θ(m+n)
// General case Θ(m+n)
void Map::resizeUp()
{
    int oldM = m;
    m = 2*m;
    auto* oldTable = table;
    table = nullptr;
    // Increment and rehash until a compatible m is found
    bool rehashResult;
    do {
        delete[] table;
        m = find_next_prime(m); //Θ(1)
        table = new Bucket[m]; //Θ(m)
        rehashResult = rehash(oldTable, oldM); //Θ(n)
    } while (!rehashResult);

    delete[] oldTable;
}

// Best case Θ(n)
// Worst case Θ(n)
// Average case Θ(n)
// General case Θ(n)
bool Map::rehash(Bucket* oldTable, int oldM)
{
    for (int i = 0; i < oldM; ++i) //Θ(n)
    {
        TElem currentElement = oldTable[i].element;
        if (currentElement != NULL_TELEM)
        {
            bool result = addWithoutResize(currentElement.first, currentElement.second); // Θ(1)
            if (!result)
            {
                return false;
            }
        }
    }
    return true;
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
bool Map::addWithoutResize(TKey k, TValue v)
{
    //Θ(1)
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
    }

    // Try moving Θ(1)
    short unsigned int bitMap = table[hashPosition].bitMap;
    for (int hop = 0; hop < hopRange; ++hop) //Θ(1)
    {
        if ((bitMap & (1 << hop)) == 0)
        {
            int currentPosition = hashFunction(k, hop);
            TElem currentElem = table[currentPosition].element;
            TKey currentKey = currentElem.first;
            int expectedCurrentElemPosition = hashFunction(currentKey, 0);
            for (int j = 0; j < hopRange; ++j) // Θ(1)
            {
                int newPosition = hashFunction(currentKey, j);
                if (table[newPosition].element == NULL_TELEM)
                {
                    table[newPosition].element = currentElem;
                    // Find elem to move position in bitmap
                    int elemToMovePostionInBitMap = NULL_TKEY;
                    for (int l = 0; l < hopRange; ++l) //Θ(1)
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

Map::Map(const Map &map)
{
    this->m = map.m;
    this->hopRange = map.hopRange;
    this->currentSize = map.m;
    this->table = new Bucket[m];
    for (int i = 0; i < m; ++i)
    {
        this->table[m] = map.table[m];
    }
}

Map Map::operator=(const Map &map){
    if (this == &map)
        return *this;
    // Copy
    this->m = map.m;
    this->hopRange = map.hopRange;
    this->currentSize = map.m;
    this->table = new Bucket[m];
    for (int i = 0; i < m; ++i)
    {
        this->table[m] = map.table[m];
    }
    return *this;
}


