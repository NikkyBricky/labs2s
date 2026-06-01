#include "HashTable.h"

#include <cstdint>
#include <utility>

#define SIX_SEVEN 67

static size_t strhash(const KeyType &key)
{
    size_t hash = 0;
    for (size_t i = 0; i < key.size(); ++i)
    {
        hash = hash * SIX_SEVEN + key[i];
    }
    return hash;
}

static size_t rehash(std::vector<std::list<std::pair<KeyType, ValueType>>> &table)
{
    size_t newSize = table.size() * 2;
    size_t filled = 0;
    std::vector<std::list<std::pair<KeyType, ValueType>>> newTable(newSize);

    for (size_t i = 0; i < table.size(); ++i)
    {
        for (auto it = table[i].cbegin(); it != table[i].cend(); ++it)
        {
            size_t hash = strhash(it->first);
            auto &bucket = newTable[hash % newSize];
            if (bucket.empty())
            {
                ++filled;
            }
            bucket.push_back(*it);
        }
    }

    table = std::move(newTable);
    return filled;
}

size_t HashTable::hash_function(const KeyType &key) const
{
    return strhash(key);
}

HashTable::HashTable(size_t size) noexcept
    : _capacity(size ? size : 1), _filled(0), table(_capacity)
{
}

HashTable::~HashTable()
{
}

void HashTable::insert(const KeyType &key, const ValueType &value)
{
    auto &list = table[hash_function(key) % _capacity];

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (it->first == key)
        {
            it->second = value;
            return;
        }
    }

    list.push_back(std::pair<KeyType, ValueType>(key, value));
    if (list.size() == 1)
    {
        ++_filled;
    }

    if (getLoadFactor() > 0.75)
    {
        _filled = rehash(table);
        _capacity *= 2;
    }
}

bool HashTable::find(const KeyType &key, ValueType &value) const
{
    const auto &list = table[hash_function(key) % _capacity];
    for (auto it = list.cbegin(); it != list.cend(); ++it)
    {
        if (it->first == key)
        {
            value = it->second;
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key)
{
    auto &list = table[hash_function(key) % _capacity];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (it->first == key)
        {
            list.erase(it);
            if (list.empty())
            {
                --_filled;
            }
            return;
        }
    }
}

ValueType &HashTable::operator[](const KeyType &key)
{
    auto &list = table[hash_function(key) % _capacity];
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (it->first == key)
        {
            return it->second;
        }
    }

    insert(key, 0);
    return (*this)[key];
}

double HashTable::getLoadFactor()
{
    return (double)_filled / _capacity;
}
