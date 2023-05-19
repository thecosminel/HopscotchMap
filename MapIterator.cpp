#include "Map.h"
#include "MapIterator.h"
#include <exception>
using namespace std;


MapIterator::MapIterator(const Map& d) : map(d)
{
	currentPosition = 0;
}


void MapIterator::first()
{
    currentPosition	= 0;
    while (map.table[currentPosition].element.first == NULL_TKEY && currentPosition < map.m)
        currentPosition++;
}


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


TElem MapIterator::getCurrent()
{
    if (valid())
	    return map.table[currentPosition].element;
    throw std::exception();
}


bool MapIterator::valid() const
{
    if (currentPosition < map.m && map.table[currentPosition].element.first != NULL_TKEY)
        return true;
	return false;
}



