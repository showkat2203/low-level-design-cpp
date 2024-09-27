# LRU Cache Implementation

This project implements a generic **Least Recently Used (LRU)** cache in C++, which is capable of storing key-value pairs with an eviction mechanism when the cache reaches full capacity.

## Features

1. **Cache with Pluggable Eviction Policy**: 
   - The cache supports eviction policies through a flexible interface. For now, **LRU (Least Recently Used)** eviction is implemented, but other eviction policies can be added.
   
2. **LRU Eviction Policy**:
   - The LRU policy ensures that the least recently accessed items are evicted when the cache reaches its capacity limit.
   
3. **Storage Backend**:
   - The cache uses a **hashmap-based storage** to store key-value pairs efficiently and retrieve them in constant time.
   
4. **Flexible Design**:
   - The cache is designed to be flexible, allowing different storage backends and eviction policies to be easily swapped without modifying the core cache logic.

## Classes and Design

### 1. **IEvictionPolicy<T>**
   - This is an interface that defines the behavior of an eviction policy.
   - It has two main responsibilities:
     - Track access to keys in the cache.
     - Evict a key when necessary.

### 2. **LRUEvictionPolicy<T>**
   - A concrete implementation of the `IEvictionPolicy` interface, based on the **Least Recently Used (LRU)** algorithm.
   - It uses a combination of a doubly linked list and a hashmap for efficient tracking of key access.
     - The linked list helps in keeping the order of usage (most recent to least recent).
     - The hashmap provides O(1) access to move keys in the list and retrieve the least recently used key for eviction.

### 3. **IStorage<T, V>**
   - This is an interface that defines the basic operations of a storage backend.
   - It manages key-value pairs and provides the following operations:
     - Add a new key-value pair.
     - Retrieve the value associated with a key.
     - Remove a key.
     - Check if a key exists.
     - Get the current size of the storage.

### 4. **HashMapStorage<T, V>**
   - A concrete implementation of the `IStorage` interface.
   - This storage class is based on a **hashmap**, providing efficient O(1) lookups, additions, and removals of key-value pairs.

### 5. **Cache<T, V>**
   - This class represents the cache itself. It uses an eviction policy and storage backend to manage cache entries.
   - Key responsibilities of the `Cache` class:
     - Add new key-value pairs to the cache.
     - Retrieve existing values from the cache.
     - Handle the eviction of the least recently used item when the cache reaches its capacity.
     - Ensure proper tracking of access patterns to keep the cache efficient.

## Usage

In the main program, the cache is instantiated with an LRU eviction policy and hashmap-based storage. Here's how the cache behaves:

1. The user can insert key-value pairs using the `put` method.
2. When a key is accessed (via `put` or `get`), it is marked as the most recently used.
3. If the cache exceeds the predefined capacity, the least recently used key is evicted to make room for new entries.
4. The user can retrieve values from the cache using the `get` method. If a key does not exist, an exception is thrown.

## Example Scenario

1. **Create Cache**: The cache is created with a capacity of 4, an LRU eviction policy, and hashmap-based storage.
2. **Put Entries**: Insert key-value pairs like (1, "one"), (2, "two"), etc.
3. **Access Entries**: Access entries to mark them as recently used.
4. **Eviction**: Once the cache exceeds the capacity, the least recently used entry will be evicted to accommodate new entries.

## Advantages

1. **Efficiency**: The combination of a linked list and hashmap in the LRU policy ensures that key accesses and evictions happen in constant time.
2. **Flexibility**: The design allows for the easy addition of new eviction policies or storage mechanisms without modifying the core logic.
3. **Modularity**: Each class has a well-defined responsibility, adhering to SOLID principles, which makes the code easier to maintain and extend.

## Future Improvements

1. **Additional Eviction Policies**: Implement other eviction strategies, such as **Least Frequently Used (LFU)**, **FIFO (First-In-First-Out)**, etc.
2. **Different Storage Backends**: Introduce additional storage backends, such as **disk-based storage** for large datasets that cannot fit into memory.
3. **Concurrency Support**: Add thread safety and concurrent access mechanisms to make the cache suitable for multi-threaded environments.
