//============================================================================
// Name        : HashTable.cpp
// Author      : Justin Guida
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <limits>
#include <string> // atoi
#include <time.h>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

/**
 * A utility struct to hold ANSI escape codes for coloring console output.
 * This provides a clean, readable way to add color to the user interface
 * without adding external dependencies. The strings are static and const
 * for efficiency.
 */
struct Color {
    static const std::string RESET;
    static const std::string BRIGHT_BLUE;
    static const std::string BRIGHT_YELLOW;
    static const std::string BRIGHT_CYAN;
    static const std::string BRIGHT_GREEN;
    static const std::string BRIGHT_RED;
    static const std::string MAGENTA;
};

// Define the color codes
const std::string Color::RESET = "[0m";
const std::string Color::BRIGHT_BLUE = "[1;34m";
const std::string Color::BRIGHT_YELLOW = "[1;33m";
const std::string Color::BRIGHT_CYAN = "[1;36m";
const std::string Color::BRIGHT_GREEN = "[1;32m";
const std::string Color::BRIGHT_RED = "[1;31m";
const std::string Color::MAGENTA = "[1;35m";

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;

    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 **/
class HashTable {
private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
     // tightening parameter types
    void Insert(const Bid& bid);
    void Remove(const std::string& bidId);
    Bid  Search(const std::string& bidId);
    void PrintAll() const;
    size_t Size();

    // Hash a string bidId into a bucket index using std::hash<string>
    // Supports alphanumeric IDs, unlike the int-based hash()
    unsigned int hash(const std::string& key) const;

};

/**
 * Default constructor
 **/
HashTable::HashTable() {
    tableSize = DEFAULT_SIZE;
    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize); // single head node per bucket
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 **/
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this ->
    this->tableSize = (size == 0 ? DEFAULT_SIZE : size);

    // resize nodes size
    nodes.resize(this->tableSize);
}


/**
 * Destructor
 **/

HashTable::~HashTable() {
    for (unsigned int bucket_index = 0; bucket_index < tableSize; ++bucket_index) {
        // start at head in the buckets chain
        Node *head = &nodes[bucket_index];
        Node *curr_bucket = head->next;
        // walk to delete chain
        while (curr_bucket != nullptr) {
            Node *nextBucket = curr_bucket->next;
            // free the current bucket
            delete curr_bucket;
            //move to the nextBucket
            curr_bucket = nextBucket;
        }
        // reset head so buckets empty
        nodes[bucket_index].next = nullptr;
        nodes[bucket_index].key = UINT_MAX;
    }
    // erase head the vector of heads
    nodes.clear();
}

/**
 * Calculate the hash value of a numeric key.
 * Keeps compatibility with legacy code that may pass ints.
 *
 * @param key The integer key to hash
 * @return The bucket index (0 ..tableSize-1)
 */
unsigned int HashTable::hash(int key) {
    return static_cast<unsigned int>(key) % tableSize;
}

/**
 * Calculate the hash value of a string key (ex bidId).
 * Uses std::hash<std::string> which safely handle alphanumeric IDs.
 * Preferred overload for all bidId lookups.
 *
 * @param key The string key to hash
 * @return The bucket index (0 .. tableSize-1)
 */
unsigned int HashTable::hash(const std::string& key) const {
    return std::hash<std::string>{}(key) % tableSize;
}


/**
 * Insert a bid into the hash table.
 *
 * The bid is hashed using its bidId (string) to compute a bucket index.
 * If the bucket head is empty, store the bid directly in the head node.
 * If the bucket already has a head:
 *   -Walk the linked list at this bucket.
 *   - If a node with the same bidId is found, replace it, update in place
 *   - Otherwise, append a new node to the end of the chain.
 *
 * This prevents collisions from overwriting data
 * by using chaining, linked lists per bucket.
 *
 * @param bid The bid to insert (const reference to avoid copies).
 */

void HashTable::Insert(const Bid& bid) {
    // compute bucket index from the string bidId (works for alphanumeric IDs)
    unsigned int bucket_index = hash(bid.bidId);
    Node* head = &nodes[bucket_index];

    // In each bucket, nodes[bucket_index] is a head node.
    // head.key == UINT_MAX means "this bucket is empty".
    if (head->key == UINT_MAX) {
        head->key = bucket_index;
        head->bid = bid;
        head->next = nullptr;
        return;
    }
    // walk the chain; checks for duplicate or append
    Node *curr_bucket = head;
    while (curr_bucket->next != nullptr) {
        // check last node for same-id overwrite
        if (curr_bucket->bid.bidId == bid.bidId) {
            //update the existing bid
            curr_bucket->bid = bid;
            return;
        }
        curr_bucket = curr_bucket->next;
    }
    // final duplicate check for last node
    if (curr_bucket->bid.bidId == bid.bidId) {
        curr_bucket->bid = bid;
        return;
    }
    // if not found, need to append to the end of the chain
    Node *newNode = new Node(bid, bucket_index);
    newNode->next = nullptr;
    curr_bucket->next = newNode;
}

/**
 * Print all bids stored in the hash table.
 *
 * Iterates over every bucket thtas in the table:
 *  - Starts at the head node for each bucket.
 *  - Walks through the linked list chain at that bucket.
 *  - Prints any node holds a bid
 *    (nodes with key == UINT_MAX are empty).
 *
 * Output format: bucket_index, bidId, title, amount, fund
 */
void HashTable::PrintAll() const {
    // iterate through every bucket in the table
    for (unsigned int bucket_index = 0; bucket_index < tableSize; ++bucket_index) {
        const Node *iter = &nodes[bucket_index];
        // walk the chain for this bucket
        while (iter != nullptr) {
            if (iter->key != UINT_MAX) {

                // output key, bidID, title, amount and fund
                cout << iter->key << ", " << iter->bid.bidId << ", " << iter->bid.title << ", " << iter->bid.amount <<
                        ", " << iter->bid.fund << endl;
            }
                //move to next node in chain
                iter = iter->next;
        }
    }
}

/**
* Remove a bid by bidId.
*
* Process:
*  - Compute the bucket index from the bidId (string hash).
*  - If the bucket is empty; head empty and no chain, return.
*  - If the head holds the target bid:
*      - If no chain; clear the head node to mark the bucket empty.
*      - If there is a chain; promote the first chained node into the head then delete it.
*      - Otherwise, walk the chain; unlink the matching node if found.
*/
void HashTable::Remove(const std::string& bidId) {
    // map the bidId to a bucket index with string hash
    unsigned int bucket_index = hash(bidId);
    Node *head = &nodes[bucket_index];

    // empty bucket; nothing to remove
    if (head->key == UINT_MAX && head->next == nullptr) {
        return;
    }
    // head node matches; head holds the target bid
    if (head->key != UINT_MAX && head->bid.bidId == bidId) {
        if (head->next == nullptr) {
            // only head in the bucket; clear; mark bucket empty
            head->key = UINT_MAX;
            head->bid = Bid();
            head->next = nullptr;
        } else {
            // promote 1st chained node to head, then delete the node
            Node *nextBucket = head->next;
            head->bid = nextBucket->bid;
            head->next = nextBucket->next;
            head->key = nextBucket->key; // maintain invariant: head->key must equal its bucket index
            delete nextBucket;
        }
        return;
    }
    // walk chain; search for match
    Node *prev_bucket = head;
    Node *curr_bucket = head->next;
    while (curr_bucket != nullptr) {
        if (curr_bucket->bid.bidId == bidId) {
            prev_bucket->next = curr_bucket->next; // unlink node
            delete curr_bucket; // free memory
            return;
        }
        prev_bucket = curr_bucket;
        curr_bucket = curr_bucket->next;
    }
}


/**
* Search for a bid by bidId.
*
* Process:
*  - Hash the bidId (string-based hash) to find its bucket index.
*  - Check the head node at that bucket; if it matches, return it.
*  - Otherwise, walk down the linked list chain for that bucket:
*      - If a matching bidId is found, return the bid.
*      - Continue until the end of the chain.
*  - If no match is found, return default-constructed Bid
*    (with bidId == "") signifying "not found".
*
* @param bidId The bid identifier string to look up.
* @return The matching Bid if found, or an empty Bid otherwise.
*/
    Bid HashTable::Search(const std::string& bidId) {
        Bid bid; // default-constructed Bid; "not found" result
        // map the bidId to a bucket index with string hash
        unsigned int bucket_index = hash(bidId);

        // if entry found for the key
        Node *head= &nodes[bucket_index];

        // check the bucket head first
        if (head->key != UINT_MAX && head->bid.bidId == bidId) {
            return head->bid;
        }
        // walk chain for matches
        const Node *curr_bucket = head ->next;
        while (curr_bucket != nullptr) {
            if (curr_bucket->bid.bidId == bidId) {
                return curr_bucket->bid;
            }
            curr_bucket = curr_bucket->next;
        }
        return bid; // not found
    }


    //============================================================================
    // Static methods used for testing
    //============================================================================

    /**
     * Display the bid information to the console (std::out)
     *
     * @param bid struct containing the bid info
     **/
    void displayBid(Bid bid) {
        cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
                << bid.fund << endl;
        return;
    }

    /**
     * Load a CSV file containing bids into a container
     *
     * @param csvPath the path to the CSV file to load
     * @return a container holding all the bids read
     **/
    void loadBids(string csvPath, HashTable *hashTable) {
        cout << "Loading CSV file " << csvPath << endl;

        // initialize the CSV Parser using the given path
        csv::Parser file = csv::Parser(csvPath);

        // read and display header row - optional
        vector<string> header = file.getHeader();
        for (auto const &c: header) {
            cout << c << " | ";
        }
        cout << "" << endl;

        try {
            // loop to read rows of a CSV file
            for (unsigned int i = 0; i < file.rowCount(); i++) {
                // Create a data structure and add to the collection of bids
                Bid bid;
                bid.bidId = file[i][1];
                bid.title = file[i][0];
                bid.fund = file[i][8];
                bid.amount = strToDouble(file[i][4], '$');

                //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

                // push this bid to the end
                hashTable->Insert(bid);
            }
        } catch (csv::Error &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    /**
     * Simple C function to convert a string to a double
     * after stripping out unwanted char
     *
     * credit: http://stackoverflow.com/a/24875936
     *
     * @param ch The character to strip out
     **/
    double strToDouble(string str, char ch) {
        str.erase(remove(str.begin(), str.end(), ch), str.end());
        return atof(str.c_str());
    }

/**
*    Purpose: Prevents menu from printing immediately, till user presses Enter
*   - Giving the user time to read the previous output.
*   - Clears any leftover characters in the input buffer from prior
*    cin operations
*   Details:
*  - Prints a prompt message Press Enter to continue...
*  - Calls cin.ignore() to get rid of any buffered characters up to newline.
*  - Calls cin.get() to wait for the user to press Enter.
**/
    void pauseForUser() {
        // prompt the user
        cout << endl << Color::BRIGHT_CYAN << "Press Enter to continue..." << Color::RESET;
        cout.flush();

        // clear any leftover characters in the input buffer from previous cin operations
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // wait for the user to press Enter.
        cin.get();
    }

    /**
     * The one and only main() method
     */
    int main(int argc, char *argv[]) {
        // process command line arguments
        string csvPath, bidKey;
        switch (argc) {
            case 2:
                csvPath = argv[1];
                bidKey = "98223";
                break;
            case 3:
                csvPath = argv[1];
                bidKey = argv[2];
                break;
            default:
                csvPath = "data/eBid_Monthly_Sales.csv";
                bidKey = "98223";
        }

        // define a timer variable
        clock_t ticks;

        // define a hash table to hold all the bids
        HashTable *bidTable;

        Bid bid;
        bidTable = new HashTable();

        int choice = 0;
        while (choice != 9) {
            // display the menu with ANSI color codes
            // menu layout around the options
            cout << endl;
            cout << Color::BRIGHT_BLUE << "+-------------------------------------------+" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|      eBid Bidder HashTable System         |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "+-------------------------------------------+" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|                                           |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|   " << Color::BRIGHT_YELLOW << "[1]" << Color::RESET << " Load Bids                           |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|   " << Color::BRIGHT_YELLOW << "[2]" << Color::RESET << " Display All Bids                    |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|   " << Color::BRIGHT_YELLOW << "[3]" << Color::RESET << " Find Bid                            |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|   " << Color::BRIGHT_YELLOW << "[4]" << Color::RESET << " Remove Bid                          |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|                                           |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|   " << Color::BRIGHT_YELLOW << "[9]" << Color::RESET << " Exit                                |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "|                                           |" << Color::RESET << endl;
            cout << Color::BRIGHT_BLUE << "+-------------------------------------------+" << Color::RESET << endl;
            cout << endl; // newline for spacing

            cout << Color::BRIGHT_CYAN << "Enter choice: " << Color::RESET;

            // flush the output buffer so the prompt is displayed appears before user input
            cout.flush();
            cin >> choice;

            // guard against non-integer input
            if (cin.fail()) {
                cout << endl << Color::BRIGHT_RED << "Error: Invalid input. Please enter a number." << Color::RESET << endl;

                // clear error flags
                cin.clear();
                // get rid of the rest of the line from input buffer
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                choice = 0; // reset choice so menu will re-display
                continue;
            }

            cout << endl; // Add a newline for cleaner output spacing

            // process user choice
            switch (choice) {

                case 1:
                    {
                        // initialize timer variable before loading bids
                        ticks = clock();

                        // method call to load the bids
                        loadBids(csvPath, bidTable);

                        // calculate elapsed time and display the  result
                        ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                        cout << Color::BRIGHT_GREEN << "Load complete." << Color::RESET << endl;
                        cout << Color::MAGENTA << "time: " << ticks << " clock ticks" << Color::RESET << endl;
                        cout << Color::MAGENTA << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << Color::RESET << endl;
                    }
                    // pause to allow user to read output before menu redisplays
                    pauseForUser();
                    break;

                case 2:
                    bidTable->PrintAll();
                    pauseForUser();
                    break;

                case 3:
                    {
                        ticks = clock();

                        bid = bidTable->Search(bidKey);

                        ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                        if (!bid.bidId.empty()) {
                            cout << Color::BRIGHT_GREEN << "Bid found!" << Color::RESET << endl;
                            displayBid(bid);
                        } else {
                            cout << Color::BRIGHT_RED << "Bid Id " << bidKey << " not found." << Color::RESET << endl;
                        }

                        cout << Color::MAGENTA << "time: " << ticks << " clock ticks" << Color::RESET << endl;
                        cout << Color::MAGENTA << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << Color::RESET << endl;
                    }
                    pauseForUser();
                    break;

                case 4:
                    bidTable->Remove(bidKey);
                    // feedback that the operation was attempted
                    cout << Color::BRIGHT_GREEN << "Attempted to remove Bid Id " << bidKey << "." << Color::RESET << endl;
                    pauseForUser();
                    break;

                case 9:
                    // default case for exit
                    break;

                default:
                    cout << Color::BRIGHT_RED << "Error: " << choice << " is not a valid option." << Color::RESET << endl;
                    pauseForUser();
                    break;
            }
        }

        cout << endl << Color::BRIGHT_BLUE << "Good bye." << Color::RESET << endl;

        return 0;
    }


