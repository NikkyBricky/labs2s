#include <cstdint>
#include <functional>

#include "HashTable.h"

HashTable::HashTable(size_t size) noexcept
    : _capacity(static_cast<int32_t>(size == 0 ? 1 : size)),
      _filled(0),
      table(static_cast<size_t>(_capacity))
{
}

HashTable::~HashTable()
{
}

size_t HashTable::hash_function(const KeyType &key) const
{
    if (_capacity <= 0)
    {
        return 0;
    }

    return std::hash<KeyType>{}(key) % static_cast<size_t>(_capacity);
}

void HashTable::insert(const KeyType &key, const ValueType &value)
{
    if (_capacity <= 0)
    {
        _capacity = 1;
        table.assign(static_cast<size_t>(_capacity), {});
        _filled = 0;
    }

    auto rehash = [this](int32_t newCapacity)
    {
        std::vector<std::list<std::pair<KeyType, ValueType>>> oldTable = std::move(table);

        _capacity = newCapacity;
        table.assign(static_cast<size_t>(_capacity), {});
        _filled = 0;

        for (const auto &bucket : oldTable)
        {
            for (const auto &entry : bucket)
            {
                size_t index = hash_function(entry.first);
                auto &destBucket = table[index];

                if (destBucket.empty())
                {
                    ++_filled;
                }

                destBucket.push_back(entry);
            }
        }
    };

    size_t index = hash_function(key);
    auto &bucket = table[index];

    for (auto &entry : bucket)
    {
        if (entry.first == key)
        {
            entry.second = value;
            return;
        }
    }

    if (bucket.empty())
    {
        if (static_cast<double>(_filled + 1) / static_cast<double>(_capacity) > 0.75)
        {
            rehash(_capacity * 2);
            index = hash_function(key);
        }
    }

    auto &targetBucket = table[index];
    if (targetBucket.empty())
    {
        ++_filled;
    }

    targetBucket.emplace_back(key, value);
}

bool HashTable::find(const KeyType &key, ValueType &value) const
{
    if (_capacity <= 0)
    {
        return false;
    }

    size_t index = hash_function(key);
    const auto &bucket = table[index];

    for (const auto &entry : bucket)
    {
        if (entry.first == key)
        {
            value = entry.second;
            return true;
        }
    }

    return false;
}

void HashTable::remove(const KeyType &key)
{
    if (_capacity <= 0)
    {
        return;
    }

    size_t index = hash_function(key);
    auto &bucket = table[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
        if (it->first == key)
        {
            bucket.erase(it);

            if (bucket.empty())
            {
                --_filled;
            }

            return;
        }
    }
}

ValueType &HashTable::operator[](const KeyType &key)
{
    if (_capacity <= 0)
    {
        _capacity = 1;
        table.assign(static_cast<size_t>(_capacity), {});
        _filled = 0;
    }

    auto rehash = [this](int32_t newCapacity)
    {
        std::vector<std::list<std::pair<KeyType, ValueType>>> oldTable = std::move(table);

        _capacity = newCapacity;
        table.assign(static_cast<size_t>(_capacity), {});
        _filled = 0;

        for (const auto &bucket : oldTable)
        {
            for (const auto &entry : bucket)
            {
                size_t index = hash_function(entry.first);
                auto &destBucket = table[index];

                if (destBucket.empty())
                {
                    ++_filled;
                }

                destBucket.push_back(entry);
            }
        }
    };

    size_t index = hash_function(key);
    auto &bucket = table[index];

    for (auto &entry : bucket)
    {
        if (entry.first == key)
        {
            return entry.second;
        }
    }

    if (bucket.empty())
    {
        if (static_cast<double>(_filled + 1) / static_cast<double>(_capacity) > 0.75)
        {
            rehash(_capacity * 2);
            index = hash_function(key);
        }
    }

    auto &targetBucket = table[index];
    if (targetBucket.empty())
    {
        ++_filled;
    }

    targetBucket.emplace_back(key, ValueType{});
    return targetBucket.back().second;
}

double HashTable::getLoadFactor()
{
    if (_capacity <= 0)
    {
        return 0.0;
    }

    return static_cast<double>(_filled) / static_cast<double>(_capacity);
}
