#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>

// Cache eviction policy interface
template<typename K>
class EvictionPolicy {
public:
    virtual ~EvictionPolicy() {}
    virtual void accessKey(K key) = 0;
    virtual void addKey(K key) = 0;
    virtual K evictKey() = 0;
};

// LRU eviction policy
template<typename K>
class LRUPolicy : public EvictionPolicy<K> {
private:
    std::list<K> keys;
    std::unordered_map<K, typename std::list<K>::iterator> keyPosition;

public:
    void accessKey(K key) override {
        if (keyPosition.find(key) != keyPosition.end()) {
            keys.erase(keyPosition[key]);
            keys.push_front(key);
            keyPosition[key] = keys.begin();
        }
    }

    void addKey(K key) override {
        keys.push_front(key);
        keyPosition[key] = keys.begin();
    }

    K evictKey() override {
        if (!keys.empty()) {
            auto last = keys.back();
            keys.pop_back();
            keyPosition.erase(last);
            return last;
        }
        // throw std::runtime_error("Attempt to evict from an empty cache");
        return K();
    }
};

// LFU eviction policy
template<typename K>
class LFUPolicy : public EvictionPolicy<K> {
private:
    struct Node {
        K key;
        int freq;
        typename std::list<K>::iterator iter;
    };

    int minFreq = 0;
    std::unordered_map<K, Node> nodes;
    std::unordered_map<int, std::list<K>> freqList;

public:
    void accessKey(K key) override {
        if (nodes.find(key) != nodes.end()) {
            auto& node = nodes[key];
            freqList[node.freq].erase(node.iter);
            node.freq++;
            freqList[node.freq].push_front(key);
            node.iter = freqList[node.freq].begin();
            if (freqList[minFreq].empty()) {
                minFreq++;
            }
        }
    }

    void addKey(K key) override {
        minFreq = 1;
        freqList[minFreq].push_front(key);
        nodes[key] = {key, minFreq, freqList[minFreq].begin()};
    }

    K evictKey() override {
        if (!freqList[minFreq].empty()) {
            auto evict = freqList[minFreq].back();
            freqList[minFreq].pop_back();
            nodes.erase(evict);
            return evict;
        }
        // throw std::runtime_error("Attempt to evict from an empty cache");
        return K();
    }
};

// Generic cache class
template<typename K, typename V>
class Cache {
private:
    std::unordered_map<K, V> store;
    std::unique_ptr<EvictionPolicy<K>> policy;
    int capacity;

public:
    Cache(int cap, std::unique_ptr<EvictionPolicy<K>> evictionPolicy)
        : capacity(cap), policy(std::move(evictionPolicy)) {}

    V get(K key) {
        if (store.find(key) == store.end()) {
            std::cout << "Key not found: " << key << std::endl;
            return V();  // Return default-constructed value
        }
        policy->accessKey(key);
        return store[key];
    }

    void put(K key, V value) {
        if (store.find(key) == store.end() && store.size() >= capacity) {
            K evict = policy->evictKey();
            store.erase(evict);
            std::cout << "Evicted key: " << evict << std::endl;
        }
        store[key] = value;
        policy->accessKey(key);
    }

    void printCache() {
        std::cout << "Cache contents:" << std::endl;
        for (auto& p : store) {
            std::cout << "Key: " << p.first << ", Value: " << p.second << std::endl;
        }
    }

    void flushAll() {
        store.clear();
    }
};

int main() {
    // Using LRU policy
    Cache<int, std::string> lruCache(2, std::make_unique<LRUPolicy<int>>());
    lruCache.put(1, "one");
    lruCache.put(2, "two");
    lruCache.get(1); // Access to update LRU order
    lruCache.put(3, "three"); // Should evict 'two'
    lruCache.printCache();

    // Using LFU policy
    Cache<int, std::string> lfuCache(2, std::make_unique<LFUPolicy<int>>());
    lfuCache.put(1, "one");
    lfuCache.put(2, "two");
    lfuCache.get(1);
    lfuCache.get(1);
    lfuCache.put(3, "three"); // Should evict 'two'
    lfuCache.printCache();

    return 0;
}
