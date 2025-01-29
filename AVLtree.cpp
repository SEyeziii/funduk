#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <vector>

struct Person {
    std::string fullName;
    std::string passport;

    bool operator<(const Person& other) const {
        return std::tie(fullName, passport) < std::tie(other.fullName, other.passport);
    }

    bool operator==(const Person& other) const {
        return fullName == other.fullName && passport == other.passport;
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << person.fullName << ", " << person.passport;
        return os;
    }
};

struct Node {
    Person data;
    Node* left;
    Node* right;
    int height;

    Node(const Person& person) : data(person), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) {
        return height(node->left) - height(node->right);
    }

    void updateHeight(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, const Person& person) {
        if (!node) {
            return new Node(person);
        }

        if (person < node->data) {
            node->left = insert(node->left, person);
        }
        else if (node->data < person) {
            node->right = insert(node->right, person);
        }

        return balance(node);
    }

    Node* minValueNode(Node* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* deleteNode(Node* node, const Person& person) {
        if (!node) return node;

        if (person < node->data) {
            node->left = deleteNode(node->left, person);
        }
        else if (node->data < person) {
            node->right = deleteNode(node->right, person);
        }
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }

        return balance(node);
    }

    void inOrder(Node* node) {
        if (node) {
            inOrder(node->left);
            std::cout << node->data << std::endl;
            inOrder(node->right);
        }
    }

    void reverseOrder(Node* node) {
        if (node) {
            reverseOrder(node->right);
            std::cout << node->data << std::endl;
            reverseOrder(node->left);
        }
    }

    void clean(Node* node) {
        if (node) {
            clean(node->left);
            clean(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clean(root);
    }

    void insert(const Person& person) {
        root = insert(root, person);
    }

    void remove(const Person& person) {
        root = deleteNode(root, person);
    }

    void search(const Person& person) {
        Node* node = root;
        while (node) {
            if (node->data == person) {
                std::cout << "Found: " << node->data << std::endl;
                return;
            }
            node = (person < node->data) ? node->left : node->right;
        }
        std::cout << "Not Found: " << person << std::endl;
    }

    void display() {
        if (root) {
            inOrder(root);
        }
        else {
            std::cout << "The tree is empty!" << std::endl;
        }
    }

    void displayReverse() {
        if (root) {
            reverseOrder(root);
        }
        else {
            std::cout << "The tree is empty!" << std::endl;
        }
    }
};

void displayMenu() {
    std::cout << "Select an action:" << std::endl;
    std::cout << "1. Add multiple people" << std::endl;
    std::cout << "2. Remove a person" << std::endl;
    std::cout << "3. Search for a person" << std::endl;
    std::cout << "4. Print (in order)" << std::endl;
    std::cout << "5. Print (reverse order)" << std::endl;
    std::cout << "6. Exit" << std::endl;
}

std::vector<Person> parseInput(const std::string& input) {
    std::vector<Person> people;
    std::istringstream stream(input);
    std::string line;

    while (std::getline(stream, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            Person person;
            person.fullName = line.substr(0, commaPos);
            person.passport = line.substr(commaPos + 1);
            people.push_back(person);
        }
    }
    return people;
}

int main() {
    AVLTree tree;
    int choice;
    std::string fullName, passport;

    do {
        displayMenu();
        std::cout << "Your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Игнорируем символ новой строки, оставшийся в буфере

        switch (choice) {
        case 1: {
            std::cout << "Enter multiple people (each person on a new line, separate Full Name and Passport Number by comma):" << std::endl;
            std::string input;
            std::string line;
            while (std::getline(std::cin, line)) {
                if (line.empty()) break;
                input += line + "\n";
            }
            std::vector<Person> people = parseInput(input);
            for (const auto& person : people) {
                tree.insert(person);
            }
            break;
        }
        case 2:
            std::cout << "Enter full name and passport number to remove (Full Name, Passport Number): ";
            std::getline(std::cin, fullName, ',');
            std::getline(std::cin, passport);
            tree.remove({ fullName, passport });
            break;
        case 3:
            std::cout << "Enter full name and passport number to search (Full Name, Passport Number): ";
            std::getline(std::cin, fullName, ',');
            std::getline(std::cin, passport);
            tree.search({ fullName, passport });
            break;
        case 4:
            std::cout << "People list (in order):" << std::endl;
            tree.display();
            break;
        case 5:
            std::cout << "People list (reverse order):" << std::endl;
            tree.displayReverse();
            break;
        case 6:
            std::cout << "Exiting." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    } while (choice != 6);

    return 0;
}
