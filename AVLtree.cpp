#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

//////////////////ВСПОМОГАТЕЛЬНЫЕ///////////////////
bool isString(const string& inString)
{
    for (int i = 0; i < inString.length(); i++)
    {
        if (!isalpha(inString[i]))
        {
            return false;
        }
        if (!isupper(inString[0]))
        {
            return false;
        }
        if ((i != 0) && !islower(inString[i]))
        {
            return false;
        }
    }
    if (inString.length() <= 1)
    {
        return false;
    }
    return !inString.empty();
}

bool isFullName(const string& inString)
{
    for (char ch : inString)
    {
        if (!isalpha(ch))
        {
            return false;
        }
        if (!isupper(ch))
        {
            return false;
        }
    }

    if (inString.length() != 3)
    {
        return false;
    }

    return !inString.empty();
}

void cinError()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(100, '\n');
    }
}


////////Двусвязный кольцевой список повторов////////

struct Node
{
    int data;
    Node* next;
    Node* prev;

    Node(int value) : data(value), next(this), prev(this) {}
};

class LinkedList
{
private:

    Node* head = nullptr;

    // Удаление следующего элемента 
    void deleteNextElement(Node* elem)
    {
        if (head == nullptr)
        {
            return;
        }

        if (elem->next == head)
        {
            if (head->next == head)
            {
                delete head;
                head = nullptr;
                return;
            }

            elem->next = head->next;
            delete head;
            head = elem->next;
        }

        else
        {
            Node* toDelete = elem->next;
            elem->next = toDelete->next;
            elem->next->prev = elem;
            delete toDelete;
        }
    }

public:

    //Добавление элемента (по убыванию)
    void add(int value)
    {
        Node* newNode = new Node(value);
        if (head == nullptr)
        {
            head = newNode;
            return;
        }

        Node* current = head;
        if (value > head->data)
        {
            while (current->next != head)
            {
                current = current->next;
            }
            newNode->next = head;
            newNode->prev = current;
            current->next = newNode;
            head->prev = newNode;
            head = newNode;
            return;
        }

        while (current->next != head && current->next->data >= value)
        {
            current = current->next;
        }

        newNode->next = current->next;
        newNode->prev = current;
        current->next->prev = newNode;
        current->next = newNode;

    }

    //Вывод списка
    void printList()
    {
        if (head == nullptr)
        {
            cout << "Empty List!!!" << endl;
            return;
        }

        Node* current = head;

        cout << "[";

        do
        {
            if (current->next == head)
            {
                cout << current->data;
                current = current->next;
            }
            else
            {
                cout << current->data << ", ";
                current = current->next;
            }
        } while (current != head);

        cout << "]";
        cout << endl;
    }

    // Удаление по значению
    void deleteByValue(int value)
    {
        if (head == nullptr)
        {
            return;
        }

        Node* current = head;

        while (current->next != head)
        {
            if (current->next->data == value)
            {
                deleteNextElement(current);
            }
            else
            {
                current = current->next;
            }
        }

        if (head->data == value)
        {
            deleteNextElement(current);
        }
    }

    // Подсчет элементов в списке
    int countList() const
    {
        if (head == nullptr)
        {
            return 0;
        }

        int count = 1;

        Node* temp = head;
        while (temp->next != head) {
            temp = temp->next;
            count++;
        }
        return count;
    }

    // Функция возвращающая голову
    Node* getHead()
    {
        return head;
    }

    int getData()
    {
        return head->data;
    }
};



// Структура ключа дерева
struct treeData
{
    string fullName;
    string surname;
    string name;
    string patronymic;

    bool operator < (const treeData& key) const
    {
        if (fullName != key.fullName)
        {
            return fullName < key.fullName;
        }
        if (name != key.name)
        {
            return name < key.name;
        }
        if (surname != key.surname)
        {
            return surname < key.surname;
        }
        if (patronymic != key.patronymic)
        {
            return patronymic < key.patronymic;
        }

        return false;
    }

    bool operator == (const treeData& key) const
    {
        return ((fullName == key.fullName) && (surname == key.surname) &&
            (name == key.name) && (patronymic == key.patronymic));
    }
};

// Структура Авл дерева
struct AVLTree
{
    treeData key;
    int balanceFactor;
    AVLTree* right;
    AVLTree* left;
    LinkedList currList;

    AVLTree(treeData data, int value) : key(data), balanceFactor(0), right(nullptr), left(nullptr)
    {
        currList.add(value);
    }
};

// Добавление элемента по Вирту
void add(AVLTree*& root, const treeData& key, bool& h, int& value)
{

    AVLTree* p1, * p2;

    // Вставка
    if (root == nullptr)
    {
        root = new AVLTree(key, value);
        h = true;
    }
    else if (key < root->key)
    {
        add(root->left, key, h, value);
        if (h) // Выросла левая ветвь
        {
            if (root->balanceFactor == 1)
            {
                root->balanceFactor = 0;
                h = false;
            }
            else if (root->balanceFactor == 0)
            {
                root->balanceFactor = -1;
            }
            else // Восстановление баланса
            {
                p1 = root->left;
                if (p1->balanceFactor == -1) // Одинарная LL-ротация
                {
                    root->left = p1->right;
                    p1->right = root;
                    root->balanceFactor = 0;
                    root = p1;
                }
                else // Двойная LR-ротация
                {
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left = p1;
                    root->left = p2->right;
                    p2->right = root;
                    if (p2->balanceFactor == -1)
                    {
                        root->balanceFactor = 1;
                    }
                    else
                    {
                        root->balanceFactor = 0;
                    }
                    if (p2->balanceFactor == 1)
                    {
                        p1->balanceFactor = -1;
                    }
                    else
                    {
                        p1->balanceFactor = 0;
                    }
                    root = p2;
                }
                root->balanceFactor = 0;
                h = false;
            }
        }
    }
    else if (root->key < key)
    {
        add(root->right, key, h, value);

        if (h) // Выросла правая ветвь
        {
            if (root->balanceFactor == -1)
            {
                root->balanceFactor = 0;
                h = false;
            }
            else if (root->balanceFactor == 0)
            {
                root->balanceFactor = 1;
            }
            else // Восстановление баланса
            {
                p1 = root->right;
                if (p1->balanceFactor == 1) // Одинарная RR-ротация
                {
                    root->right = p1->left;
                    p1->left = root;
                    root->balanceFactor = 0;
                    root = p1;
                }
                else // Двойная RL-ротация
                {
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right = p1;
                    root->right = p2->left;
                    p2->left = root;
                    if (p2->balanceFactor == 1)
                    {
                        root->balanceFactor = -1;
                    }
                    else
                    {
                        root->balanceFactor = 0;
                    }
                    if (p2->balanceFactor == -1)
                    {
                        p1->balanceFactor = 1;
                    }
                    else
                    {
                        p1->balanceFactor = 0;
                    }
                    root = p2;
                }
                root->balanceFactor = 0;
                h = false;
            }
        }
    }
    else if (root->key == key) // Ключ уже существует
    {
        root->currList.add(value);
        h = false;
    }
}

// Балансировка левого поддерева по Вирту
void balanceL(AVLTree*& root, bool& h)
{
    AVLTree* p1, * p2;

    if (root->balanceFactor == -1)
    {
        root->balanceFactor = 0;
    }
    else if (root->balanceFactor == 0)
    {
        root->balanceFactor = 1;
        h = false;
    }
    else // Восстановление баланса
    {
        p1 = root->right;
        if (p1->balanceFactor >= 0) // Одиночная RR-ротация
        {
            root->right = p1->left;
            p1->left = root;
            if (p1->balanceFactor == 0)
            {
                root->balanceFactor = 1;
                p1->balanceFactor = -1;
                h = false;
            }
            else
            {
                root->balanceFactor = 0;
                p1->balanceFactor = 0;
            }
            root = p1;
        }
        else // Двойная RL-ротация
        {
            p2 = p1->left;
            p1->left = p2->right;
            p2->right = p1;
            root->right = p2->left;
            p2->left = root;
            if (p2->balanceFactor == 1)
            {
                root->balanceFactor = -1;
            }
            else
            {
                root->balanceFactor = 0;
            }
            if (p2->balanceFactor == -1)
            {
                p1->balanceFactor = 1;
            }
            else
            {
                p1->balanceFactor = 0;
            }
            root = p2;
            p2->balanceFactor = 0;
        }
    }
}

// Балансировка правого поддерева по Вирту
void balanceR(AVLTree*& root, bool& h)
{
    AVLTree* p1, * p2;

    if (root->balanceFactor == 1)
    {
        root->balanceFactor = 0;
    }
    else if (root->balanceFactor == 0)
    {
        root->balanceFactor = -1;
        h = false;
    }
    else // Восстановление баланса
    {
        p1 = root->left;
        if (p1->balanceFactor <= 0) // Одиночная LL-ротация
        {
            root->left = p1->right;
            p1->right = root;
            if (p1->balanceFactor == 0)
            {
                root->balanceFactor = -1;
                p1->balanceFactor = 1;
                h = false;
            }
            else
            {
                root->balanceFactor = 0;
                p1->balanceFactor = 0;
            }
            root = p1;
        }
        else // Двойная LR-ротация
        {
            p2 = p1->right;
            p1->right = p2->left;
            p2->left = p1;
            root->left = p2->right;
            p2->right = root;
            if (p2->balanceFactor == -1)
            {
                root->balanceFactor = 1;
            }
            else
            {
                root->balanceFactor = 0;
            }
            if (p2->balanceFactor == 1)
            {
                p1->balanceFactor = -1;
            }
            else
            {
                p1->balanceFactor = 0;
            }
            root = p2;
            p2->balanceFactor = 0;
        }
    }
}

// Функция поиска наименьшего элемента справа
AVLTree* del(AVLTree*& root, bool& h)
{
    if (root->left != nullptr)  // идем в левое поддерево для поиска минимального элемента
    {
        AVLTree* r = del(root->left, h);
        if (h)
        {
            balanceL(root, h);
        }
        return r;
    }
    else
    {
        AVLTree* q;
        q = root;
        root = root->right;
        h = true;
        return q;
    }
}


// Функция удаления узла по Вирту
void deleteNode(AVLTree*& root, const treeData& key, bool& h)
{

    if (root == nullptr)
    {
        return;
    }

    int stringNumber = root->currList.getData();

    if (key < root->key)
    {
        deleteNode(root->left, key, h);
        {
            if (h)
            {
                balanceL(root, h);
            }
        }
    }
    else if (root->key < key)
    {
        deleteNode(root->right, key, h);
        if (h)
        {
            balanceR(root, h);
        }
    }
    else
    {
        if (root->currList.countList() > 1)
        {
            root->currList.deleteByValue(stringNumber);
            return;
        }
        AVLTree* q = root;
        if (root->right == nullptr)
        {
            root = root->left;
            h = true;
        }
        else if (root->left == nullptr)
        {
            root = root->right;
            h = true;
        }
        else
        {
            q = del(root->left, h);
            root->key = q->key;
            root->currList = q->currList;
            if (h)
            {
                balanceL(root, h);
            }
        }
    }
}

// Поиск элементов по заданному ключу
void searchData(AVLTree*& root, const treeData& key)
{
    if (root == nullptr)
    {
        cout << endl;
        cout << "*** Node not found ***" << endl;
        return;
    }
    else if (key == root->key)
    {
        cout << endl;
        cout << "*** Node was found ***" << endl;
        return;
    }
    if (key < root->key)
    {
        searchData(root->left, key);
    }
    else if (root->key < key)
    {
        searchData(root->right, key);
    }
}

// Очистка дерева
void clearTree(AVLTree*& root, int& value)
{
    if (root == nullptr)
    {
        return;
    }

    clearTree(root->right, value);
    clearTree(root->left, value);
    delete root;
    root = nullptr;
    value = 0;
}

// Проверка входных данных
bool stringCheck(treeData& curr)
{
    if (curr.name.length() <= 1 || curr.surname.length() <= 1
        || curr.patronymic.length() <= 1 || curr.fullName.length() != 3)
    {
        return false;
    }
    if (!isString(curr.name))
    {
        return false;
    }
    if (!isString(curr.surname))
    {
        return false;
    }
    if (!isString(curr.patronymic))
    {
        return false;
    }

    if (isFullName(curr.fullName))
    {
        if (curr.fullName[0] != curr.surname[0] || curr.fullName[1] != curr.name[0]
            || curr.fullName[2] != curr.patronymic[0])
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

// Получение данных из файла
void getFromFile(AVLTree*& root, fstream& file_input, int& value, bool& h)
{

    if (!file_input.is_open())
    {
        cout << "*** ERROR::File can't be open ***" << endl;
        return;
    }

    string curr;

    while (getline(file_input, curr))
    {
        istringstream currLine(curr);
        treeData data;
        currLine >> data.surname >> data.name >> data.patronymic >> data.fullName;
        if (stringCheck(data))
        {
            add(root, data, h, ++value);
        }
        else
        {
            value++;
        }
    }
}

// Вывод в файл
void outInFile(AVLTree*& root, ofstream& file_output)
{
    if (root == nullptr)
    {
        return;
    }
    outInFile(root->left, file_output);

    Node* temp = root->currList.getHead();

    do
    {
        if (temp->next != root->currList.getHead())
        {
            file_output << temp->data << ", ";
        }
        else
        {
            file_output << temp->data << " | ";
        }
        temp = temp->next;
    } while (temp != root->currList.getHead());

    file_output << root->key.surname << " " << root->key.name << " " << root->key.patronymic << " " << root->key.fullName << endl;

    outInFile(root->right, file_output);
}

// Функция создания ключа
treeData createKey()
{
    treeData key;

    bool flag = true;

    while (flag)
    {
        cout << "Type your Surname: ";
        cin >> key.surname;
        if (isString(key.surname))
        {
            flag = false;
        }
        else
        {
            cout << "*** ERROR::Incorrect Surname ***" << endl;
        }
    }

    flag = true;

    while (flag)
    {
        cout << "Type your Name: ";
        cin >> key.name;
        if (isString(key.name))
        {
            flag = false;
        }
        else
        {
            cout << "*** ERROR::Incorrect Name ***" << endl;
        }
    }

    flag = true;

    while (flag)
    {
        cout << "Type your Patronymic: ";
        cin >> key.patronymic;
        if (isString(key.patronymic))
        {
            flag = false;
        }
        else
        {
            cout << "*** ERROR::Incorrect Patronymic ***" << endl;
        }
    }

    flag = true;

    while (flag)
    {
        cout << "Type your FullName: ";
        cin >> key.fullName;
        if (isFullName(key.fullName))
        {
            if (key.fullName[0] != key.surname[0] || key.fullName[1] != key.name[0]
                || key.fullName[2] != key.patronymic[0])
            {
                cout << "*** ERROR::Incorrect FullName ***" << endl;
            }
            else
            {
                flag = false;
            }
        }
        else
        {
            cout << "*** ERROR::Incorrect FullName ***" << endl;
        }
    }
    return key;
}

// Обход справа налево (симметричный)
void rightLeftOrder(AVLTree*& root, ofstream& file)
{
    if (root == nullptr)
    {
        return;
    }

    // Сначала правое поддерево
    rightLeftOrder(root->right, file);

    cout << root->key.surname << " " << root->key.name << " " << root->key.patronymic << " | "
        << root->key.fullName << endl;

    file << root->key.surname << " " << root->key.name << " " << root->key.patronymic << " | "
        << root->key.fullName << endl;

    // Затем левое поддерево
    rightLeftOrder(root->left, file);
}

// Вспомогательная функция для печати узла (справа налево)
void printTreeHelper(AVLTree* root, int indent = 0)
{
    Node* temp = root->currList.getHead();

    if (root != nullptr)
    {
        // Сначала правое поддерево
        if (root->right != nullptr)
        {
            printTreeHelper(root->right, indent + 4);
        }

        if (indent > 0)
        {
            cout << setw(indent) << ' ';
        }

        cout << root->key.surname << " " << root->key.name
            << " " << root->key.patronymic << " | " << root->key.fullName << "(" << root->balanceFactor << ") | Line:";

        do
        {
            if (temp->next != root->currList.getHead())
            {
                cout << temp->data << ", ";
            }
            else
            {
                cout << temp->data << "\n";
            }
            temp = temp->next;
        } while (temp != root->currList.getHead());

        // Затем левое поддерево
        if (root->left != nullptr)
        {
            printTreeHelper(root->left, indent + 4);
        }
    }
}

// Основная функция для печати дерева
void printTree(AVLTree* root)
{
    if (root == nullptr)
    {
        cout << "*** Tree is empty ***\n";
        return;
    }
    printTreeHelper(root);
}


int main()
{
    ofstream output;
    output.open("output.txt");
    int answer = 0;
    bool h;
    AVLTree* root = nullptr;
    fstream input;
    int value = 0;

    cout << "-----------------***MENU***-----------------" << endl;
    cout << "1 - to add your element to the tree" << endl;
    cout << "2 - Print all tree" << endl;
    cout << "3 - Find 1 element by key" << endl;
    cout << "4 - Delete 1 element by key" << endl;
    cout << "5 - Import all data from the file" << endl;
    cout << "6 - Export all data from the tree to the file" << endl;
    cout << "7 - Print tree (right-left)" << endl;
    cout << "8 - Delete all the tree" << endl;
    cout << "9 - EXIT" << endl;
   
    while (answer != 9)
    {
        cout << "Your choice: ";
        cin >> answer;
        cout << endl;
        treeData new_node;
        switch (answer)
        {
        case 1:
        {
            value++;
            add(root, createKey(), h, value);
            cout << endl;
            break;
        }
        case 2:
        {
            cout << "Your tree: " << endl;
            cout << "-----------" << endl;
            cout << endl;
            printTree(root);
            cout << endl;
            break;
        }
        case 7:
        {
            cout << "Your ordered tree: " << endl;
            cout << "-------------------" << endl;
            rightLeftOrder(root, output);
            cout << endl;
            cout << "*** SUCCES::All files was exported ***" << endl;
            cout << endl;
            break;
        }
        case 8:
        {
            cout << "*** SUCCES::Your tree was deleted ***" << endl;
            clearTree(root, value);
            value = 0;
            cout << endl;
            break;
        }
        case 3:
        {
            searchData(root, createKey());
            cout << endl;
            break;
        }
        case 4:
        {
            deleteNode(root, createKey(), h);
            cout << endl;
            break;
        }
        case 5:
        {
            input.open("input.txt");
            cout << "*** SUCCES::All files was imported ***" << endl;
            getFromFile(root, input, value, h);
            input.close();
            cout << endl;
            break;
        }
        case 6:
        {
            cout << "*** SUCCES::All files was exported ***" << endl;
            outInFile(root, output);
            cout << endl;
            break;
        }
        case 9:
        {
            cout << "*** EXIT::PROGRAM STOPED ***" << endl;
            break;
        }
        default:
        {
            cinError();
            cout << "*** ERROR::Incorrect answer ***" << endl;
            cout << endl;
            break;

        }
        }
    }
    output.close();
}
