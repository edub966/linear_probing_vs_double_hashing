# Linear Probing vs. Double Hashing


## Project Description

Hash Table Showdown is a C++ program that compares two hash table collision resolution algorithms: linear probing and double hashing.

The project simulates a username and password lookup system. Users can add accounts, log in, and view statistics about the hash tables. The program also generates 190,000 synthetic user records to compare the performance of both algorithms.

Both collision resolution algorithms are implemented from scratch without using built-in hash table libraries.

## Problem and Motivation

Hash tables provide an efficient way to store and retrieve data using keys. However, when two keys produce the same hash table index, a collision occurs.

Collisions can slow down operations, especially when a hash table becomes nearly full.

This project investigates how linear probing and double hashing handle collisions and how their performance changes as the load factor increases.

The goal is to determine which method performs better for inserting and searching user credentials under different conditions.

## Features

- Add users with usernames and passwords
- Log in using existing credentials
- Update passwords for existing users
- View hash table statistics
- Track collisions during insertion
- Track the number of probes during insertion and searching
- Generate 190,000 synthetic user records
- Benchmark algorithms at five different load factors
- Measure insertion and search execution times
- Compare successful and unsuccessful searches
- Run correctness tests for both algorithms

## Algorithms Implemented

### 1. Linear Probing

Linear probing resolves collisions by checking consecutive positions in the hash table until an available position is found.

The probing formula is:

    index = (hash1(key) + i) % capacity

Where:
- `hash1(key)` is the original hash position
- `i` is the number of probing attempts
- `capacity` is the hash table size

Linear probing is relatively simple to implement, but it can experience primary clustering, where consecutive occupied positions make future operations slower.

### 2. Double Hashing

Double hashing resolves collisions using a second hash function to determine the distance between probing attempts.

The probing formula is:

    index = (hash1(key) + i * hash2(key)) % capacity

Where:
- `hash1(key)` determines the starting position
- `hash2(key)` determines the step size
- `i` is the number of probing attempts

Double hashing helps reduce primary clustering by distributing probes across different positions.

Both algorithms use open addressing and are implemented manually in `hash_table.hpp`.

## Dataset

The project generates 190,000 synthetic username and password records using C++.

Example records:

    user_0, password_0
    user_1, password_1
    user_2, password_2
    ...
    user_189999, password_189999

The dataset is generated internally rather than downloaded from an external source.

The benchmark uses a hash table containing **200,003 slots** and tests the following dataset sizes:

| Number of Users | Approximate Load Factor |
|-----------------|-------------------------|
| 100,000 | 50% |
| 120,000 | 60% |
| 140,000 | 70% |
| 170,000 | 85% |
| 190,000 | 95% |

Both algorithms are tested using identical records in the same insertion order.

This ensures that the performance comparison uses the same data.

**Note:** All credentials are synthetic. The program is an educational simulation and should not be used to store real passwords.

## Technologies Used

- C++11
- Standard Template Library (`vector`, `string`)
- `chrono` for runtime measurements
- `iomanip` for formatting output
- `cassert` for correctness testing
- Clang++ compiler
- Git and GitHub for version control

No external libraries are required.

## Project Structure

```text
linear_probing_vs_double_hashing/
│
├── src/
│   ├── hash_table.hpp
│   └── main.cpp
│
├── tests/
│   └── test.cpp
│
├── README.md
└── .gitignore
```

## How to Run

A C++11 or newer compiler is required.

The program was developed and tested on macOS using Apple's Clang compiler.

### 1. Compile the Program

Open a terminal in the project's root directory.

**Mac/Linux:**

```bash
clang++ -std=c++11 -O2 src/main.cpp -o showdown
```

Alternatively, if a working GCC installation is available:

```bash
g++ -std=c++11 -O2 src/main.cpp -o showdown
```

**Windows (MinGW g++):**

```bash
g++ -std=c++11 -O2 src/main.cpp -o showdown.exe
```

### 2. Run the Program

**Mac/Linux:**

```bash
./showdown
```

**Windows:**

```bash
.\showdown.exe
```

### 3. Main Menu

When the program starts, the following menu appears:

```text
====================================
         HASH TABLE SHOWDOWN
    Linear Probing vs Double Hashing
====================================
Educational demo: Do not use real passwords.

===== MAIN MENU =====
1. User Login Demo
2. Run Benchmark
3. Exit
```

Choose option 1 to test the login functionality or option 2 to run the full benchmark.

## User Login Demonstration

The login demonstration contains four options:

```text
===== USER LOGIN DEMO =====
1. Add User
2. Login
3. View Statistics
4. Back to Main Menu
```

### Adding a User

Example:

```text
Username: edub966
Password: example123

User added successfully!
```

### Logging In

Using the correct credentials:

```text
Linear Probing: Login Successful
Double Hashing: Login Successful
```

Using incorrect credentials:

```text
Linear Probing: Login Failed
Double Hashing: Login Failed
```

### Viewing Statistics

The program displays:

- Number of users
- Current load factor
- Number of insertion collisions
- Total insertion probes
- Total search probes

Statistics are displayed separately for both algorithms.

## Benchmark

Selecting option 2 from the main menu runs the performance comparison.

The program generates 190,000 synthetic user records and tests both algorithms at five different load factors.

For each algorithm, the benchmark measures:

1. Insertion time in milliseconds
2. Successful search time in milliseconds
3. Unsuccessful search time in milliseconds
4. Total insertion collisions
5. Average insertion probes
6. Average successful search probes
7. Average unsuccessful search probes

Each experiment performs 10,000 successful searches and 10,000 unsuccessful searches.

The benchmark also checks that all records were inserted successfully.

### Experimental Results

The benchmark was executed three times on a MacBook Air.

The following table shows average insertion times across the three runs:

| Load Factor | Linear Probing | Double Hashing |
|-------------|----------------|----------------|
| 50% | 20.052 ms | 9.360 ms |
| 60% | 45.993 ms | 9.454 ms |
| 70% | 96.047 ms | 12.915 ms |
| 85% | 572.510 ms | 21.789 ms |
| 95% | 1977.747 ms | 39.314 ms |

### Average Successful Search Probes

| Load Factor | Linear Probing | Double Hashing |
|-------------|----------------|----------------|
| 50% | 2.366 | 1.331 |
| 60% | 13.057 | 1.577 |
| 70% | 26.673 | 1.808 |
| 85% | 132.550 | 2.392 |
| 95% | 428.631 | 3.247 |

### Average Unsuccessful Search Probes

| Load Factor | Linear Probing | Double Hashing |
|-------------|----------------|----------------|
| 50% | 13.992 | 1.664 |
| 60% | 102.653 | 2.248 |
| 70% | 211.688 | 2.962 |
| 85% | 2259.727 | 6.223 |
| 95% | 12280.377 | 19.582 |

### Results Discussion

Double hashing required fewer probes than linear probing across all five tested load factors.

As the table became more full, linear probing experienced a substantial increase in collisions and probe counts.

This behavior is consistent with primary clustering, where occupied positions form long consecutive groups.

At 95% load, double hashing completed insertion approximately 50 times faster on average in this experiment.

However, these results are specific to the synthetic dataset, hash functions, insertion order, and test environment. The sequential usernames and primary hash function likely contributed to the unusually large performance difference.

The results should not be interpreted as proving that double hashing is always faster under every workload.

## Time Complexity Analysis

Both methods provide efficient average-case performance when the hash function distributes keys effectively and the load factor remains sufficiently low.

| Operation | Linear Probing | Double Hashing |
|-----------|----------------|----------------|
| Insert (Average) | O(1) | O(1) |
| Insert (Worst) | O(n) | O(n) |
| Search (Average) | O(1) | O(1) |
| Search (Worst) | O(n) | O(n) |
| Update (Average) | O(1) | O(1) |
| Update (Worst) | O(n) | O(n) |
| Space | O(n) | O(n) |

Here, `n` represents the number of hash table slots.

This analysis assumes username lengths are bounded. Hash calculation itself also requires time proportional to the username length.

In the worst case, an operation may need to examine every position in the table.

## Running Tests

The project includes correctness tests for both collision resolution algorithms.

### 1. Compile the Tests

From the project root directory:

**Mac/Linux:**

```bash
clang++ -std=c++11 tests/test.cpp -o tests_runner
```

**Windows (MinGW g++):**

```bash
g++ -std=c++11 tests/test.cpp -o tests_runner.exe
```

### 2. Run the Tests

**Mac/Linux:**

```bash
./tests_runner
```

**Windows:**

```bash
.\tests_runner.exe
```

### Expected Output

```text
Testing Linear Probing...
All tests passed!

Testing Double Hashing...
All tests passed!

All tests completed successfully.
```

### Test Cases

The tests verify:

- Inserting users
- Successful credential searches
- Incorrect passwords
- Nonexistent usernames
- Updating existing passwords
- Preventing duplicate usernames
- Rejecting empty usernames
- Filling the entire hash table
- Rejecting new users when the table is full
- Searching existing users after the table becomes full

Both implementations passed the correctness tests.

## Limitations

- Hash tables have fixed capacities.
- The program does not automatically resize tables.
- Deletion is not implemented.
- Usernames cannot contain spaces in the interactive demonstration.
- Passwords are stored as plain strings for educational purposes.
- The application is not a secure authentication system.
- Benchmark timing varies depending on the computer and compiler.
- The dataset uses generated usernames rather than real-world account data.

## Conclusion

This project demonstrates how collision resolution affects hash table performance.

Linear probing checks consecutive positions after collisions, which makes it simple to implement but susceptible to primary clustering.

Double hashing uses a second hash function to calculate a different probing sequence for each key.

The experimental results showed that double hashing performed significantly better for this dataset, especially at higher load factors.

The project provided practical experience implementing hash table algorithms, analyzing time complexity, using large datasets, and measuring algorithm performance in C++.
