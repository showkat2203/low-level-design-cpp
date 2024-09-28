#include <bits/stdc++.h>
#include <chrono>


using namespace std;
using namespace std::chrono;

template<typename T>
class IEvictionPolicy{
  public:
    virtual void keyAccessed(const T& key){};
    virtual T evict() = 0;
    virtual ~IEvictionPolicy() = default;
};


template<typename T>
class LRUEvictionPolicy : public IEvictionPolicy<T>{
private:
    list<T> keyList;
    unordered_map<T, typename list<T> :: iterator> keyMap;
    
public:
    void keyAccessed(const T& key) override{
        if( keyMap.find(key) != keyMap.end() ){
            keyList.erase(keyMap[key]);
        }
        keyList.push_front(key);
        keyMap[key] = keyList.begin();
    }
    
    T evict() override {
        if( keyList.empty() ){
            throw runtime_error("No item to evict");
        }
        T lastKey = keyList.back();
        keyList.pop_back();
        keyMap.erase(lastKey);
        
        return lastKey;
    }
};

template<typename T, typename V >
class IStorage{
public:
    virtual void add(const T& key, const V& val) = 0;
    virtual V get(const T& key) = 0;
    virtual void remove(const T& Key) = 0;
    virtual bool exists(const T& key) = 0;
    virtual size_t size() const = 0;
    
    virtual ~IStorage() = default;
};

template<typename T, typename V>
class HashMapStorage : public IStorage<T, V> {
private:
    unordered_map<T, V> data;

public:
    void add(const T& key, const V& val) override{
        data[key] = val;
    }
     V get(const T& key) override{
        if (!exists(key)) {
            throw runtime_error("Key not found in HashMap Cache");
        }
        return data[key];
     }
     
    void remove(const T& key) override{
        data.erase(key);
    }
    
    bool exists(const T& key) override{
        return data.find(key) != data.end();
    }
     size_t size() const override{
         return data.size();
     }
};

// TTL-enabled Storage
template<typename T, typename V>
class TTLHashMapStorage : public IStorage<T, V> {
private:
    unordered_map<T, pair<V, time_point<steady_clock>>> data; // Pair of value and expiration time
    milliseconds ttl; // Default TTL for all keys

public:
    // Constructor with TTL
    TTLHashMapStorage(milliseconds _ttl) : ttl(_ttl) {}

    void add(const T& key, const V& val) override {
        data[key] = { val, steady_clock::now() + ttl };
    }

    V get(const T& key) override {
        if (!exists(key)) {
            throw runtime_error("Key not found in TTL HashMap Cache");
        }
        return data[key].first;
    }

    void remove(const T& key) override {
        data.erase(key);
    }

    bool exists(const T& key) override {
        auto it = data.find(key);
        if (it == data.end()) return false;

        // Check if key has expired
        if (steady_clock::now() > it->second.second) {
            data.erase(it);  // Remove expired key
            return false;
        }

        return true;
    }

    size_t size() const override {
        return data.size();
    }
};



template<typename T, typename V>
class Cache{
private:
    unique_ptr<IStorage<T, V>> storage;
    unique_ptr<IEvictionPolicy<T>> policy;
    size_t capacity;
    
public:
    Cache(unique_ptr<IStorage<T, V>> _storage, unique_ptr<IEvictionPolicy<T>> _policy, size_t _capacity) : storage(move(_storage)), policy(move(_policy)), capacity(_capacity){}
    
    void put(const T& key, const V& val){
        if( storage->exists(key) ){
            storage->add(key, val);
            policy->keyAccessed(key);
            return;
        }
        if( storage->size() == capacity ){
            T evictedKey = policy->evict();
            storage->remove(evictedKey);
        }
        storage->add(key, val);
        policy->keyAccessed(key);
    }
    
    V get(const T& key){
        if( !storage->exists(key) ){
            throw runtime_error("Key Not Found in the Cache");
        }
        policy->keyAccessed(key);
        return storage->get(key);
    }
};


int main() {
    auto hashStorage = make_unique<HashMapStorage<int, string>>();
    auto lruEvictionPolicy = make_unique<LRUEvictionPolicy<int>>();
    Cache<int, string> lruCache(move(hashStorage), move(lruEvictionPolicy), 4);
    
    lruCache.put(1, "one");
    lruCache.put(2, "two");
    lruCache.put(3, "three");
    
    cout << lruCache.get(1) << endl;
    lruCache.put(4, "four");
    lruCache.put(5, "five");
    cout << lruCache.get(4) << endl;

    return 0;
}
