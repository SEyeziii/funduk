#include <iostream>

class Node {
public:
    int value;
    Node* next;
    Node* prev;

    Node(int val) : value(val), next(nullptr), prev(nullptr) {}
};

class CDLList {
private:
    Node* head;

public:
    CDLList() : head(nullptr) {}

    void initialize() {
        head = nullptr;
    }

    void freeMemory() {
        if (!head) return;

        Node* current = head;
        do {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        } while (current != head);

        head = nullptr;
    }

    void addElement(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
        } else {
            Node* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
    }

    void deleteElement(int value) {
    if (!head) return;

    Node* current = head;
    do {
        if (current->value == value) {
            Node* toRemove = current;
            if (toRemove == head && toRemove->next == head) {
                head = nullptr;
                delete toRemove;
                return;
            } else {
                toRemove->prev->next = toRemove->next;
                toRemove->next->prev = toRemove->prev;
                if (toRemove == head) {
                    head = toRemove->next; // если удаляем головной элемент
                }
                Node* nextNode = toRemove->next; // не меняем current на next, сохраняем ссылку на следующий 
                delete toRemove;
                current = nextNode; // к следующему элементу
            }
        }
        current = current->next;
    } while (current != head);
    }

    void deleteBefore(int value) {
        if (!head) return;

        Node* current = head;
        do {
            if (current->value == value) {
                Node* toRemove = current->prev;
                if (toRemove != current) {
                    if (toRemove == head) {
                        head = current;
                    }
                    toRemove->prev->next = toRemove->next;
                    toRemove->next->prev = toRemove->prev;
                    delete toRemove;
                }
            }
            current = current->next;
        } while (current != head);
    }

    Node* find(int value) {
        if (!head) return nullptr;

        Node* current = head;
        do {
            if (current->value == value) {
                return current;
            }
            current = current->next;
        } while (current != head);

        return nullptr;
    }

    void print() {
        if (!head) return;

        Node* current = head;
        do {
            std::cout << current->value << " ";
            current = current->next;
        } while (current != head);
        std::cout << std::endl;
    }

    void merge(CDLList& other) {
        if (!other.head) return;

        if (!head) {
            head = other.head;
            return;
        }

        Node* tail1 = head->prev;
        Node* tail2 = other.head->prev;

        tail1->next = other.head;
        other.head->prev = tail1;
        tail2->next = head;
        head->prev = tail2;

        other.head = nullptr;
    }
};

int main() {
    CDLList list1, list2;
    int choice, value;
    
    list2.addElement(2);
    list2.addElement(1);
    list2.addElement(8);
    list2.addElement(-1);
    
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Add element to list\n";
        std::cout << "2. Delete element from list\n";
        std::cout << "3. Delete element before a value\n";
        std::cout << "4. Find an element\n";
        std::cout << "5. Print list\n";
        std::cout << "6. Merge two lists\n";
        std::cout << "7. Free memory\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter value to add: ";
            std::cin >> value;
            list1.addElement(value);
            break;

        case 2:
            std::cout << "Enter value to delete: ";
            std::cin >> value;
            list1.deleteElement(value);
            break;

        case 3:
            std::cout << "Enter value to delete the element before it: ";
            std::cin >> value;
            list1.deleteBefore(value);
            break;

        case 4:
            std::cout << "Enter value to find: ";
            std::cin >> value;
            if (list1.find(value)) {
                std::cout << "Element " << value << " found in the list.\n";
            } else {
                std::cout << "Element " << value << " not found.\n";
            }
            break;

        case 5:
            std::cout << "List: ";
            list1.print();
            break;

        case 6:
            list1.merge(list2);
            std::cout << "Lists merged.\n";
            break;

        case 7:
            list1.freeMemory();
            std::cout << "Memory freed.\n";
            break;

        case 8:
            std::cout << "Exiting program.\n";
            break;

        default:
            std::cout << "Invalid choice, please try again.\n";
            break;
        }

    } while (choice != 8);

    return 0;
}
