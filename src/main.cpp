
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include "hash_table.hpp"

using namespace std;

struct Credential {
    string username;
    string password;
};

struct BenchmarkResult {
    double insertTime;
    double successfulSearchTime;
    double unsuccessfulSearchTime;

    double averageInsertProbes;
    double averageSuccessfulProbes;
    double averageUnsuccessfulProbes;

    long long collisions;

    int successfulLogins;
    int failedLookups;
    int insertedUsers;
};

// generate synthetic accounts
vector<Credential> generateData(int count) {
    vector<Credential> data;

    for (int i = 0; i < count; i++) {
        Credential user;

        user.username = "user_" + to_string(i);
        user.password = "password_" + to_string(i);

        data.push_back(user);
    }

    return data;
}

// test one of the two hash table implementations
BenchmarkResult testMethod(
    vector<Credential>& data,
    int count,
    int capacity,
    bool useDoubleHashing
) {
    HashTable table(capacity, useDoubleHashing);

    BenchmarkResult result;
    int searchCount = 10000;

   // test 1, insertion performance
    auto start = chrono::steady_clock::now();

    for (int i = 0; i < count; i++) {
        table.insert(data[i].username, data[i].password);
    }

    auto end = chrono::steady_clock::now();

    result.insertTime =
        chrono::duration<double, milli>(end - start).count();

    result.insertedUsers = table.getSize();
    result.collisions = table.getCollisions();

    result.averageInsertProbes =
        (double)table.getInsertProbes() / count;

    // test 2, successful searches

    table.resetSearchProbes();

    int successful = 0;

    start = chrono::steady_clock::now();

    for (int i = 0; i < searchCount; i++) {
        // Select usernames from across the dataset
        int index = (1LL * i * count) / searchCount;

        if (table.search(data[index].username,
                         data[index].password)) {
            successful++;
        }
    }

    end = chrono::steady_clock::now();

    result.successfulSearchTime =
        chrono::duration<double, milli>(end - start).count();

    result.averageSuccessfulProbes =
        (double)table.getSearchProbes() / searchCount;

    result.successfulLogins = successful;

    // test 3, unsuccessful searches

    table.resetSearchProbes();

    int failed = 0;

    start = chrono::steady_clock::now();

    for (int i = 0; i < searchCount; i++) {
        string username = "missing_user_" + to_string(i);

        if (!table.search(username, "wrong_password")) {
            failed++;
        }
    }

    end = chrono::steady_clock::now();

    result.unsuccessfulSearchTime =
        chrono::duration<double, milli>(end - start).count();

    result.averageUnsuccessfulProbes =
        (double)table.getSearchProbes() / searchCount;

    result.failedLookups = failed;

    return result;
}

void printResult(string name, BenchmarkResult result) {
    cout << "\n" << name << endl;
    cout << "----------------------------------\n";

    cout << fixed << setprecision(3);

    cout << "Users inserted: "
         << result.insertedUsers << endl;

    cout << "Insertion time: "
         << result.insertTime << " ms\n";

    cout << "Successful search time: "
         << result.successfulSearchTime << " ms\n";

    cout << "Unsuccessful search time: "
         << result.unsuccessfulSearchTime << " ms\n";

    cout << "Insertion collisions: "
         << result.collisions << endl;

    cout << "Average insertion probes: "
         << result.averageInsertProbes << endl;

    cout << "Average successful search probes: "
         << result.averageSuccessfulProbes << endl;

    cout << "Average unsuccessful search probes: "
         << result.averageUnsuccessfulProbes << endl;

    cout << "Successful logins: "
         << result.successfulLogins << "/10000\n";

    cout << "Correct failed lookups: "
         << result.failedLookups << "/10000\n";
}

// compare 
void runBenchmark() {
    const int capacity = 200003;
    const int maxUsers = 190000;

    cout << "\n===== HASH TABLE BENCHMARK =====\n";
    cout << "Generating 190,000 synthetic users...\n";

    vector<Credential> data = generateData(maxUsers);

    int sizes[] = {
        100000,
        120000,
        140000,
        170000,
        190000
    };

    for (int i = 0; i < 5; i++) {
        int count = sizes[i];
        double loadFactor = (double)count / capacity;

        cout << "\n====================================\n";
        cout << "Testing " << count << " users\n";
        cout << fixed << setprecision(2);
        cout << "Load Factor: " << loadFactor * 100 << "%\n";
        cout << "====================================\n";

        // run both on same data
        BenchmarkResult linear = testMethod(
            data, count, capacity, false
        );

        BenchmarkResult doubleHash = testMethod(
            data, count, capacity, true
        );

        printResult("LINEAR PROBING", linear);
        printResult("DOUBLE HASHING", doubleHash);

        // verify
        if (linear.insertedUsers != count ||
            doubleHash.insertedUsers != count ||
            linear.successfulLogins != 10000 ||
            doubleHash.successfulLogins != 10000 ||
            linear.failedLookups != 10000 ||
            doubleHash.failedLookups != 10000) {
            cout << "\nWARNING: A correctness check failed!\n";
        }

        cout << "\nCOMPARISON\n";
        cout << "----------------------------------\n";

        if (linear.averageInsertProbes <
            doubleHash.averageInsertProbes) {
            cout << "Linear probing used fewer insertion probes.\n";
        }
        else if (doubleHash.averageInsertProbes <
                 linear.averageInsertProbes) {
            cout << "Double hashing used fewer insertion probes.\n";
        }
        else {
            cout << "Both used the same insertion probes.\n";
        }

        if (linear.averageSuccessfulProbes <
            doubleHash.averageSuccessfulProbes) {
            cout << "Linear probing used fewer successful "
                 << "search probes.\n";
        }
        else if (doubleHash.averageSuccessfulProbes <
                 linear.averageSuccessfulProbes) {
            cout << "Double hashing used fewer successful "
                 << "search probes.\n";
        }
        else {
            cout << "Both used the same successful "
                 << "search probes.\n";
        }

        if (linear.averageUnsuccessfulProbes <
            doubleHash.averageUnsuccessfulProbes) {
            cout << "Linear probing used fewer unsuccessful "
                 << "search probes.\n";
        }
        else if (doubleHash.averageUnsuccessfulProbes <
                 linear.averageUnsuccessfulProbes) {
            cout << "Double hashing used fewer unsuccessful "
                 << "search probes.\n";
        }
        else {
            cout << "Both used the same unsuccessful "
                 << "search probes.\n";
        }
    }

    cout << "\nBenchmark complete!\n";
}

void runDemo() {
    HashTable linear(101, false);
    HashTable doubleHash(101, true);

    int choice = 0;
    string username;
    string password;

    while (choice != 4) {
        cout << "\n===== USER LOGIN DEMO =====\n";
        cout << "1. Add User\n";
        cout << "2. Login\n";
        cout << "3. View Statistics\n";
        cout << "4. Back to Main Menu\n";

        cout << "\nEnter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 1) {
            cout << "Username: ";
            cin >> username;

            cout << "Password: ";
            cin >> password;

            bool result1 = linear.insert(username, password);
            bool result2 = doubleHash.insert(username, password);

            if (result1 && result2) {
                cout << "User added successfully!\n";
            }
            else {
                cout << "Could not add user. Table may be full.\n";
            }
        }

        else if (choice == 2) {
            cout << "Username: ";
            cin >> username;

            cout << "Password: ";
            cin >> password;

            bool result1 = linear.search(username, password);
            bool result2 = doubleHash.search(username, password);

            cout << "\nLinear Probing: ";

            if (result1) {
                cout << "Login Successful\n";
            }
            else {
                cout << "Login Failed\n";
            }

            cout << "Double Hashing: ";

            if (result2) {
                cout << "Login Successful\n";
            }
            else {
                cout << "Login Failed\n";
            }
        }

        else if (choice == 3) {
            cout << "\n===== TABLE STATISTICS =====\n";

            cout << "\nLinear Probing\n";
            cout << "Users: " << linear.getSize() << endl;
            cout << "Load factor: "
                 << linear.getLoadFactor() << endl;
            cout << "Collisions: "
                 << linear.getCollisions() << endl;
            cout << "Insertion probes: "
                 << linear.getInsertProbes() << endl;
            cout << "Search probes: "
                 << linear.getSearchProbes() << endl;

            cout << "\nDouble Hashing\n";
            cout << "Users: " << doubleHash.getSize() << endl;
            cout << "Load factor: "
                 << doubleHash.getLoadFactor() << endl;
            cout << "Collisions: "
                 << doubleHash.getCollisions() << endl;
            cout << "Insertion probes: "
                 << doubleHash.getInsertProbes() << endl;
            cout << "Search probes: "
                 << doubleHash.getSearchProbes() << endl;
        }

        else if (choice != 4) {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    int choice = 0;

    cout << "====================================\n";
    cout << "         HASH TABLE SHOWDOWN\n";
    cout << "    Linear Probing vs Double Hashing\n";
    cout << "====================================\n";

    cout << "Educational demo: Do not use real passwords.\n";

    while (choice != 3) {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. User Login Demo\n";
        cout << "2. Run Benchmark\n";
        cout << "3. Exit\n";

        cout << "\nEnter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 1) {
            runDemo();
        }
        else if (choice == 2) {
            runBenchmark();
        }
        else if (choice == 3) {
            cout << "Goodbye!\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
