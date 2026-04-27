#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct Employee {
    string lastName;
    string position;
    int yearJoined;
    double salary;
};

struct TreeNode {
    Employee data;
    TreeNode *left, *right;
};

void showMenu();
TreeNode* insert(TreeNode* root, Employee e);
void displayInOrder(TreeNode* root);
void searchByName(TreeNode* root, string name);
TreeNode* deleteNode(TreeNode* root, string name);
void saveToFile(TreeNode* root, ofstream& out);
void loadFromFile(TreeNode*& root);
void clearTree(TreeNode*& root);
TreeNode* balanceTree(TreeNode* root);
void storeInVector(TreeNode* root, vector<Employee>& vec);
TreeNode* buildFromVector(const vector<Employee>& vec, int start, int end);

int getInt() {
    int val;
    while (!(cin >> val)) {
        cout << "Pomylka! Vvedit chyslo: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return val;
}

int main() {
    TreeNode* root = nullptr;
    int choice;
    do {
        showMenu();
        choice = getInt();
        switch (choice) {
            case 1: {
                Employee e;
                cout << "Prizvyshche: "; cin.ignore(); getline(cin, e.lastName);
                cout << "Posada: "; getline(cin, e.position);
                cout << "Rik: "; e.yearJoined = getInt();
                cout << "Oklad: "; cin >> e.salary;
                root = insert(root, e);
                break;
            }
            case 2: {
                string name;
                cout << "Vvedit prizvyshche dlia vydalennia: "; cin.ignore(); getline(cin, name);
                root = deleteNode(root, name);
                break;
            }
            case 3: displayInOrder(root); break;
            case 4: {
                string name;
                cout << "Poshuk: "; cin.ignore(); getline(cin, name);
                searchByName(root, name);
                break;
            }
            case 5: root = balanceTree(root); cout << "Zbalansovano.\n"; break;
            case 6: {
                string fn; cout << "Imia fajlu: "; cin >> fn;
                ofstream out(fn);
                saveToFile(root, out);
                out.close();
                break;
            }
            case 7: loadFromFile(root); break;
            case 0: clearTree(root); break;
            default: cout << "Nevirnyi vybir!\n";
        }
    } while (choice != 0);
    return 0;
}

void showMenu() {
    cout << "\n--- MENU (DEREVO) ---\n"
         << "1. Dodaty\n2. Vyluchyty\n3. Perehliad\n4. Poshuk\n"
         << "5. Balansuvannia\n6. Zberehty\n7. Zavantazhyty\n0. Vykhid\nVybir: ";
}

TreeNode* insert(TreeNode* root, Employee e) {
    if (!root) return new TreeNode{e, nullptr, nullptr};
    if (e.lastName < root->data.lastName) root->left = insert(root->left, e);
    else root->right = insert(root->right, e);
    return root;
}

void displayInOrder(TreeNode* root) {
    if (root) {
        displayInOrder(root->left);
        cout << root->data.lastName << " | " << root->data.position << " | " << root->data.salary << endl;
        displayInOrder(root->right);
    }
}

void searchByName(TreeNode* root, string name) {
    if (!root) { cout << "Ne znajdeno.\n"; return; }
    if (root->data.lastName == name) {
        cout << "Znajdeno: " << root->data.position << " | Oklad: " << root->data.salary << endl;
    } else if (name < root->data.lastName) searchByName(root->left, name);
    else searchByName(root->right, name);
}

TreeNode* findMin(TreeNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

TreeNode* deleteNode(TreeNode* root, string name) {
    if (!root) return nullptr;
    if (name < root->data.lastName) root->left = deleteNode(root->left, name);
    else if (name > root->data.lastName) root->right = deleteNode(root->right, name);
    else {
        if (!root->left) { TreeNode* t = root->right; delete root; return t; }
        if (!root->right) { TreeNode* t = root->left; delete root; return t; }
        TreeNode* t = findMin(root->right);
        root->data = t->data;
        root->right = deleteNode(root->right, t->data.lastName);
    }
    return root;
}

void saveToFile(TreeNode* root, ofstream& out) {
    if (root) {
        out << root->data.lastName << "\n" << root->data.position << "\n" 
            << root->data.yearJoined << " " << root->data.salary << "\n";
        saveToFile(root->left, out);
        saveToFile(root->right, out);
    }
}

void loadFromFile(TreeNode*& root) {
    string fn; cout << "Imia fajlu: "; cin >> fn;
    ifstream in(fn);
    if (!in) return;
    clearTree(root);
    Employee e;
    while (getline(in, e.lastName) && getline(in, e.position) && (in >> e.yearJoined >> e.salary)) {
        in.ignore();
        root = insert(root, e);
    }
}

void clearTree(TreeNode*& root) {
    if (root) {
        clearTree(root->left);
        clearTree(root->right);
        delete root;
        root = nullptr;
    }
}

void storeInVector(TreeNode* root, vector<Employee>& vec) {
    if (!root) return;
    storeInVector(root->left, vec);
    vec.push_back(root->data);
    storeInVector(root->right, vec);
}

TreeNode* buildFromVector(const vector<Employee>& vec, int start, int end) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    TreeNode* node = new TreeNode{vec[mid], nullptr, nullptr};
    node->left = buildFromVector(vec, start, mid - 1);
    node->right = buildFromVector(vec, mid + 1, end);
    return node;
}

TreeNode* balanceTree(TreeNode* root) {
    vector<Employee> vec;
    storeInVector(root, vec);
    clearTree(root);
    return buildFromVector(vec, 0, vec.size() - 1);
}
