#include "Map.h"
#include "MapIterator.h"
#include <exception>
using namespace std;

// Best case Θ(1)
// Worst case Θ(m)
// Average case Θ(m)
// General case O(m)
MapIterator::MapIterator(const Map& d) : map(d)
{
    currentPosition = 0;
    while (map.table[currentPosition].element.first == NULL_TKEY && currentPosition < map.m)
        currentPosition++;
}

// Best case Θ(1)
// Worst case Θ(m)
// Average case Θ(m)
// General case O(m)
void MapIterator::first()
{
    currentPosition	= 0;
    while (map.table[currentPosition].element.first == NULL_TKEY && currentPosition < map.m)
        currentPosition++;
}

// Best case Θ(1)
// Worst case Θ(m)
// Average case Θ(m)
// General case O(m)
void MapIterator::next()
{
    if (!valid())
        throw std::exception();
    currentPosition++;
    while (map.table[currentPosition].element.first == NULL_TKEY && currentPosition < map.m)
    {
        currentPosition++;
    }
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
TElem MapIterator::getCurrent()
{
    if (valid())
	    return map.table[currentPosition].element;
    throw std::exception();
}

// Best case Θ(1)
// Worst case Θ(1)
// Average case Θ(1)
// General case Θ(1)
bool MapIterator::valid() const
{
    if (currentPosition < map.m && map.table[currentPosition].element.first != NULL_TKEY)
        return true;
	return false;
}



