#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

// --- Node Structure (Individual elements of the list) ---
struct Node {
    string data;    // Data stored in the node (e.g., Note name)
    Node* next;     // Standard linked list pointer (Next element)
    Node* hop;      // Shortcut pointer (Skips several steps ahead)

    Node(string value) {
        data = value;
        next = nullptr;
        hop = nullptr;
    }
};

// --- Hop Linked List Structure ---
struct HopList {
    Node* head; // Entry point of the list

    HopList() {
        head = nullptr;
    }

    // --- Insert New Element ---
    void insert(const string& value) {
        Node* newNode = new Node(value);

        // If the list is empty, set new node as head
        if (head == nullptr) {
            head = newNode;
            return;
        }

        // Temporary pointer to traverse to the end
        Node* temp = head;
        int counter = 1;

        // Traverse until the last node
        while (temp->next != nullptr) {
            temp = temp->next;
            counter++;
        }

        // Attach the new node at the end
        temp->next = newNode;

        // Structure Rule: Create a hop connection every 3 nodes
        // If the current node (temp) is at a position multiple of 3, point its hop to the new node
        if (counter % 3 == 0)
            temp->hop = newNode;
    }

    // --- Update Hop Bridges ---
    // When an element is deleted, the order is broken; this function repairs the bridges
    void updateHop() {
        Node* temp = head;
        Node* lastHopNode = nullptr;
        int counter = 1;

        // First, clear all existing hop connections and rebuild
        while (temp != nullptr) {
            temp->hop = nullptr; // Reset existing hop

            // Select the first element of each group (1, 4, 7...) as "hop start"
            if (counter % 3 == 1)
                lastHopNode = temp;

            // Connect the start node to every 3rd element (3, 6, 9...)
            if (counter % 3 == 0 && lastHopNode != nullptr)
                lastHopNode->hop = temp;

            temp = temp->next;
            counter++;
        }
    }

    // --- Remove Element ---
    void remove(const string& value) {
        if (head == nullptr) return;

        // If the element to be deleted is at the head
        if (head->data == value) {
            Node* del = head;
            head = head->next;
            delete del;
            updateHop(); // Update hops as the structure changed
            return;
        }

        Node* temp = head;
        Node* prev = nullptr;

        // Traverse the list to find the value
        while (temp != nullptr && temp->data != value) {
            prev = temp;
            temp = temp->next;
        }

        // Exit if value is not found
        if (temp == nullptr) return;

        // Disconnect the node and free memory
        prev->next = temp->next;
        delete temp;

        // Re-calculate all hop bridges after deletion
        updateHop();
    }

    // --- Smart Search ---
    bool search(const string& value) {
        Node* temp = head;

        while (temp != nullptr) {
            // Check if arget value is found
            if (temp->data == value)
                return true;

            // STRATEGY: If a hop exists AND the data at the hop destination 
            // is less than or equal to target value, jump 3 steps at once!
            if (temp->hop != nullptr && temp->hop->data <= value) {
                cout << "[Hop Jump Used] " << temp->data << " -> " << temp->hop->data << endl;
                temp = temp->hop;
            }
            else {
                // If jump is not possible, proceed node by node
                temp = temp->next;
            }
        }
        return false;
    }

    // --- Print List to Console ---
    void print() {
        cout << "String Elements:\n";
        Node* temp = head;

        while (temp != nullptr) {
            cout << "[" << temp->data << "]";
            // If this node has a shortcut, display its destination
            if (temp->hop != nullptr)
                cout << "(hop->" << temp->hop->data << ") ";
            else
                cout << " ";
            temp = temp->next;
        }
        cout << "\n";
    }
};

int main() {
    // Support for UTF-8 characters in Windows console
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    vector<HopList> strings; // Vector holding multiple lists (violin strings)
    strings.push_back(HopList()); // Create 1st string by default

    int choice;
    while (true) {
        cout << "\n================ VIOLIN DATA STRUCTURE ================\n";
        cout << "1) Add element\n2) Delete element\n3) Search element\n4) Print string\n5) Create new string\n6) Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 6) break;

        // Ask the user which string to operate on
        int strIdx;
        cout << "Which string do you want to operate on? (1-" << strings.size() << "): ";
        cin >> strIdx;
        cin.ignore();

        strIdx--; // Convert user input to 0-based index
        if (strIdx < 0 || strIdx >= (int)strings.size()) {
            cout << "Invalid string index!\n";
            continue;
        }

        // --- Menu Operations ---
        if (choice == 1) {
            string val;
            cout << "Value to add: ";
            getline(cin, val);
            strings[strIdx].insert(val);
        }
        else if (choice == 2) {
            string val;
            cout << "Value to delete: ";
            getline(cin, val);
            strings[strIdx].remove(val);
        }
        else if (choice == 3) {
            string val;
            cout << "Value to search: ";
            getline(cin, val);
            if (strings[strIdx].search(val))
                cout << "Result: Found!\n";
            else
                cout << "Result: Not Found.\n";
        }
        else if (choice == 4) {
            strings[strIdx].print();
        }
        else if (choice == 5) {
            strings.push_back(HopList());
            cout << "String #" << strings.size() << " created successfully!\n";
        }
    }

    return 0;
}