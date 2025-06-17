#ifndef HISTORY_LINKEDLIST_H
#define HISTORY_LINKEDLIST_H

#include "QString"

class hislinkedlist {
private:
    int listSize;
    
    // Helper function to get node at specific position
    struct Game* getNodeAt(int position) const {
        if (position < 0 || position >= listSize) return nullptr;
        
        Game* current = head;
        for (int i = 0; i < position; ++i) {
            current = current->next;
        }
        return current;
    }
    
    // Helper function to update all indices after modification
    void updateIndices() {
        Game* current = head;
        int index = 0;
        while (current != nullptr) {
            current->index = index++;
            current = current->next;
        }
    }

public:
    struct Game {
        int index;
        char used_token;
        QString game_result;
        Game* next;
        
        // Improved constructor with initialization list
        Game(char used, const QString& res, int idx = 0) 
            : index(idx), used_token(used), game_result(res), next(nullptr) {}
    };

    Game* head;
    Game* tail; // Add tail pointer for O(1) push_back

    // Constructor
    hislinkedlist() : head(nullptr), tail(nullptr), listSize(0) {}

    // Copy constructor
    hislinkedlist(const hislinkedlist& other) : head(nullptr), tail(nullptr), listSize(0) {
        Game* current = other.head;
        while (current != nullptr) {
            push_back(current->used_token, current->game_result);
            current = current->next;
        }
    }

    // Assignment operator
    hislinkedlist& operator=(const hislinkedlist& other) {
        if (this != &other) {
            clear();
            Game* current = other.head;
            while (current != nullptr) {
                push_back(current->used_token, current->game_result);
                current = current->next;
            }
        }
        return *this;
    }

    // Destructor
    ~hislinkedlist() {
        clear();
    }

    // Clear all elements
    void clear() {
        while (head != nullptr) {
            Game* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        listSize = 0;
    }

    // Check if the list is empty
    bool empty() const {
        return head == nullptr;
    }

    // Get the size of the list
    int size() const {
        return listSize;
    }

    // Improved search - now searches by index more efficiently
    bool isfound(int index) const {
        return getNodeAt(index) != nullptr;
    }

    // Search by game result
    bool findByResult(const QString& result) const {
        Game* current = head;
        while (current != nullptr) {
            if (current->game_result == result) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // O(1) push_back with tail pointer
    void push_back(char used, const QString& res) {
        Game* newNode = new Game(used, res, listSize);
        
        if (empty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++listSize;
    }

    // Add push_front for completeness
    void push_front(char used, const QString& res) {
        Game* newNode = new Game(used, res, 0);
        
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        ++listSize;
        updateIndices(); // Update indices after insertion at front
    }

    // Improved pop_front
    void pop_front() {
        if (empty()) return;
        
        Game* temp = head;
        head = head->next;
        
        if (head == nullptr) { // List became empty
            tail = nullptr;
        }
        
        delete temp;
        --listSize;
        updateIndices(); // Update indices after removal
    }

    // Add pop_back for completeness
    void pop_back() {
        if (empty()) return;
        
        if (head == tail) { // Only one element
            delete head;
            head = tail = nullptr;
        } else {
            Game* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        --listSize;
    }

    // Improved insert with better bounds checking
    void insert(int position, char used, const QString& res) {
        if (position < 0 || position > listSize) return;
        
        if (position == 0) {
            push_front(used, res);
            return;
        }
        
        if (position == listSize) {
            push_back(used, res);
            return;
        }
        
        Game* newNode = new Game(used, res);
        Game* prev = getNodeAt(position - 1);
        
        newNode->next = prev->next;
        prev->next = newNode;
        ++listSize;
        updateIndices(); // Update indices after insertion
    }

    // Improved erase with better bounds checking
    void erase(int position) {
        if (position < 0 || position >= listSize || empty()) return;
        
        if (position == 0) {
            pop_front();
            return;
        }
        
        Game* prev = getNodeAt(position - 1);
        Game* toDelete = prev->next;
        
        prev->next = toDelete->next;
        
        if (toDelete == tail) { // Removing tail
            tail = prev;
        }
        
        delete toDelete;
        --listSize;
        updateIndices(); // Update indices after removal
    }

    // Improved GetGameNode with const correctness
    Game* GetGameNode(int index) const {
        return getNodeAt(index);
    }

    // Get game by index (alternative name)
    Game* at(int index) const {
        return getNodeAt(index);
    }

    // Iterator-like access to first and last elements
    Game* front() const {
        return head;
    }
    
    Game* back() const {
        return tail;
    }

    // Utility function to print all games (for debugging)
    void printAll() const {
        Game* current = head;
        while (current != nullptr) {
            // Print logic here - depends on your output requirements
            current = current->next;
        }
    }

    // Reverse the list
    void reverse() {
        if (empty() || head == tail) return;
        
        Game* prev = nullptr;
        Game* current = head;
        tail = head; // Old head becomes new tail
        
        while (current != nullptr) {
            Game* next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        head = prev;
        updateIndices();
    }
};

#endif // HISTORY_LINKEDLIST_H