# Key-Based Eviction Cache System

## Introduction
This project implements a generic cache system in C++ that employs a key-based eviction strategy. Unlike traditional LRU (Least Recently Used) or LFU (Least Frequently Used) caches, this cache evicts items based on the properties of the keys, allowing for eviction decisions that can be tailored to specific needs or priorities defined by the keys' natural order or other custom criteria.

## Problem Description
Caches generally evict entries based on the frequency or recency of access. However, there are scenarios where entries should be evicted based on the keys themselves, such as when certain keys are deemed less important or when keys are scored or ranked based on business rules.

## Requirements
The cache system should:
- Support basic operations such as `put`, `get`, `printCache`, and `flushAll`.
- Implement a key-based eviction policy where keys are evicted according to a specific criterion related to the key itself.
- Be generic to handle various data types for both keys and values.

## Solution
The implementation involves:
- **EvictionPolicy Interface**: An abstract base class that defines the eviction policy interface.
- **KeyBasedPolicy Class**: A concrete implementation of `EvictionPolicy` that manages keys in a sorted order, evicting the "smallest" key when the cache reaches its capacity.
- **Cache Class**: A generic cache class templated on key and value types, using the specified eviction policy to manage entries.

### Key Components
- **`EvictionPolicy<K>`**: An interface for eviction policies.
- **`KeyBasedPolicy<K>`**: Implements eviction based on key comparison.
- **`Cache<K, V>`**: Generic cache managing storage and eviction through policy.

## Code Example
```cpp
Cache<int, std::string> cache(2, std::make_unique<KeyBasedPolicy<int>>());
cache.put(3, "three");
cache.put(1, "one");
cache.put(2, "two"); // Should evict '1' because it is the smallest key
cache.printCache();
