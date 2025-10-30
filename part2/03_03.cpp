//@compile clang++ part2/03_03.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <cassert>

class List {
private:
    struct Node {
        int   value = 0;
        Node* next  = nullptr;
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    List() : head(nullptr), tail(nullptr) {}

    ~List() {
        while (!empty()) {
            pop_front();
        }
    }

    bool empty() const {
        return head == nullptr;
    }

    void push_front(int val) {
        Node* new_node = new Node{val, head};
        head = new_node;
        if (tail == nullptr) {
            tail = new_node;
        }
    }

    void push_back(int val) {
        Node* new_node = new Node{val, nullptr};
        if (empty()) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void pop_front() {
        if (empty()) {
            return;
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    void pop_back() {
        if (empty()) {
            return;
        }
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
            return;
        }

        Node* prev = head;
        while (prev->next != tail) {
            prev = prev->next;
        }
        delete tail;
        tail = prev;
        tail->next = nullptr;
    }

    void show() const {
        if (empty()) {
            std::cout << "Empty list";
        }
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << curr->value;
            if (curr->next != nullptr) {
                std::cout << ' ';
            }
            curr = curr->next;
        }
        std::cout << '\n';
    }

    int get() const {
        if (empty()) {
            return 0;
        }

        Node* slow = head;
        Node* fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow->value;
    }
};

// ----------------------------
//          TESTS
// ----------------------------
void print_state(const char* step, const List& lst) {
    std::cout << step << ": ";
    lst.show();
}

void test_unit_operations() {
    std::cout << "=== UNIT TESTS ===\n";
    List lst;
    assert(lst.empty());

    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);
    print_state("After push_back 3 elems", lst);

    lst.push_front(5);
    print_state("After push_front", lst);

    assert(!lst.empty());
    assert(lst.get() == 20);

    lst.pop_front();
    print_state("After pop_front", lst);

    lst.pop_back();
    print_state("After pop_back", lst);

    assert(lst.get() == 20);
    std::cout << "Unit tests passed.\n\n";
}

void test_integration_scenario() {
    std::cout << "=== INTEGRATION TEST ===\n";
    List lst;

    std::cout << "Step 1: Build list [1,2,3]\n";
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.show();

    std::cout << "Step 2: Add to front (0)\n";
    lst.push_front(0);
    lst.show();

    std::cout << "Step 3: Remove front and back\n";
    lst.pop_front(); // remove 0
    lst.pop_back();  // remove 3
    lst.show();

    std::cout << "Step 4: Add 5, 6, 7\n";
    lst.push_back(5);
    lst.push_back(6);
    lst.push_back(7);
    lst.show();

    std::cout << "Step 5: Middle element is " << lst.get() << "\n";

    std::cout << "Step 6: Clear all\n";
    while (!lst.empty())
        lst.pop_front();
    lst.show();
    std::cout << "Integration test passed.\n";
}

int main() {
    test_unit_operations();
    test_integration_scenario();
    std::cout << "\nAll tests completed successfully.\n";
    return 0;
}