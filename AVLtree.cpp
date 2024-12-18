
#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
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

    Node(const Person& person) : data(person), left(nullptr), right(nullptr) {}
};

class SimpleAVLTree {
private:
    Node* root;

    void insert(Node*& node, const Person& person) {
        if (!node) {
            node = new Node(person);
            return;
        }

        if (person < node->data)
            insert(node->left, person);
        else if (node->data < person)
            insert(node->right, person);
    }

    Node* minValueNode(Node* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* deleteNode(Node* node, const Person& person) {
        if (!node) return node;

        if (person < node->data)
            node->left = deleteNode(node->left, person);
        else if (node->data < person)
            node->right = deleteNode(node->right, person);
        else {
            if (!node->left)
                return node->right;
            else if (!node->right)
                return node->left;

            Node* temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }

        return node;
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
    SimpleAVLTree() : root(nullptr) {}

    ~SimpleAVLTree() {
        clean(root);
    }

    void insert(const Person& person) {
        insert(root, person);
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
        inOrder(root);
    }

    void displayReverse() {
        reverseOrder(root);
    }

    static SimpleAVLTree loadFromFile(const std::string& filename) {
        SimpleAVLTree tree;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                Person person;
                person.fullName = line.substr(0, commaPos);
                person.passport = line.substr(commaPos + 1);
                tree.insert(person);
            }
        }

        return tree;
    }
};

int main() {
    SimpleAVLTree tree = SimpleAVLTree::loadFromFile("data.txt");

    std::cout << "In-order display:" << std::endl;
    tree.display();

    Person personToFind{ "A", "1234567890" };
    tree.search(personToFind);

    Person personToRemove{ "A", "1234567890" };
    tree.remove(personToRemove);

    std::cout << "After removal:" << std::endl;
    tree.display();

    std::cout << "Reverse order display:" << std::endl;
    tree.displayReverse();

    return 0;
}
