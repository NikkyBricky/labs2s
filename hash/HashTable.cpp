#include "HashTable.h"

#include <functional>
#include <utility>

size_t HashTable::hash_function(const KeyType &key) const {
    const int p = 31;
    const int m = 1e9 + 9; 
    long long hash_value = 0;
    long long p_pow = 1;
    
    for (char c : key) {
	long long char_code = (c - 'a' + 1);
        hash_value = (hash_value + char_code * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    
    return hash_value;
}

HashTable::HashTable(size_t size) noexcept
    : _capacity(size ? size : 1), _filled(0), table(_capacity) {}

HashTable::~HashTable() {}

void HashTable::insert(const KeyType &key, const ValueType &value)
{
    auto hash = hash_function(key);
    auto &list = table[hash % _capacity];

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
        size_t newSize = table.size() * 2;
        size_t filled = 0;
        std::vector<std::list<std::pair<KeyType, ValueType>>> newTable(newSize);

        for (size_t i = 0; i < table.size(); ++i)
        {
            for (auto it = table[i].cbegin(); it != table[i].cend(); ++it)
            {
                size_t newHash = hash_function(it->first);
                auto &newList = newTable[newHash % newSize];
                if (newList.empty())
                {
                    ++filled;
                }
                newList.push_back(*it);
            }
        }

        table = std::move(newTable);
        _capacity *= 2;
        _filled = filled;
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
