#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct User {
    string username;
    string password;
    bool occupied = false;
};

class HashTable {
private:
    vector<User> table;

    int capacity;
    int numUsers;
    bool doubleHashing;

    long long collisions;
    long long insertProbes;
    long long searchProbes;

    // primary hash function
    int hash1(string username) {
        long long hash = 0;

        for (int i = 0; i < (int)username.length(); i++) {
            hash = (hash * 31 + username[i]) % capacity;
        }

        return hash;
    }

    // secondary hash function for double hashing
    int hash2(string username) {
        long long hash = 0;

        for (int i = 0; i < (int)username.length(); i++) {
            hash = (hash * 37 + username[i]) % (capacity - 1);
        }

        return 1 + hash;
    }

    // linear probing checks consecutive positions
    int linearProbe(string username, int attempt) {
        int start = hash1(username);

        return (start + attempt) % capacity;
    }

    // double hashing uses a second hash for the step size
    int doubleHashProbe(string username, int attempt) {
        int start = hash1(username);
        int step = hash2(username);

        return (start + 1LL * attempt * step) % capacity;
    }

    // choose the correct collision resolution method
    int getIndex(string username, int attempt) {
        if (doubleHashing) {
            return doubleHashProbe(username, attempt);
        }

        return linearProbe(username, attempt);
    }

public:
    HashTable(int size, bool useDoubleHashing) {
        capacity = size;
        doubleHashing = useDoubleHashing;

        numUsers = 0;
        collisions = 0;
        insertProbes = 0;
        searchProbes = 0;

        table.resize(capacity);
    }

    // insert a user or update an existing password
    bool insert(string username, string password) {
        if (username.empty()) {
            return false;
        }

        for (int i = 0; i < capacity; i++) {
            int index = getIndex(username, i);

            insertProbes++;

            if (!table[index].occupied) {
                table[index].username = username;
                table[index].password = password;
                table[index].occupied = true;

                numUsers++;
                return true;
            }

            if (table[index].username == username) {
                table[index].password = password;
                return true;
            }

            collisions++;
        }

        return false;
    }

    bool search(string username, string password) {
        for (int i = 0; i < capacity; i++) {
            int index = getIndex(username, i);

            searchProbes++;

            if (!table[index].occupied) {
                return false;
            }

            if (table[index].username == username) {
                return table[index].password == password;
            }
        }

        return false;
    }

    int getSize() {
        return numUsers;
    }

    double getLoadFactor() {
        return (double)numUsers / capacity;
    }

    long long getCollisions() {
        return collisions;
    }

    long long getInsertProbes() {
        return insertProbes;
    }

    long long getSearchProbes() {
        return searchProbes;
    }

    void resetSearchProbes() {
        searchProbes = 0;
    }
};

#endif
