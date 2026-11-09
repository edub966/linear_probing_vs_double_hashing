
#include <iostream>
#include <cassert>
#include <string>
#include "../src/hash_table.hpp"

using namespace std;

void testTable(bool doubleHashing) {
    HashTable table(101, doubleHashing);

    assert(table.insert("alice", "pass123"));
    assert(table.getSize() == 1);

    assert(table.search("alice", "pass123"));

    assert(!table.search("alice", "wrong"));

    assert(!table.search("bob", "pass123"));

    assert(table.insert("bob", "hello456"));
    assert(table.getSize() == 2);

    assert(table.insert("alice", "newpass"));
    assert(table.getSize() == 2);

    assert(table.search("alice", "newpass"));
    assert(!table.search("alice", "pass123"));

    assert(!table.insert("", "test"));

    for (int i = 0; i < 99; i++) {
        string username = "user" + to_string(i);

        assert(table.insert(username, "test"));
    }

    assert(table.getSize() == 101);

    assert(!table.insert("extra", "test"));

    assert(table.search("bob", "hello456"));
    assert(table.search("alice", "newpass"));

    assert(table.insert("bob", "updated"));
    assert(table.search("bob", "updated"));

    cout << "All tests passed!\n";
}

int main() {
    cout << "Testing Linear Probing...\n";
    testTable(false);

    cout << "\nTesting Double Hashing...\n";
    testTable(true);

    cout << "\nAll tests completed successfully.\n";

    return 0;
}
