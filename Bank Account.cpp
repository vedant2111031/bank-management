#include <iostream>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
using namespace std;

// Data structure for Account using Linked List Node
class Account {
public:
    string AccountNo, Password, Role;
    int Balance;
    Account* next;

    Account() : AccountNo(""), Password(""), Role(""), Balance(0), next(nullptr) {}
};

// Stack for undo transactions
stack<pair<string, int>> undoStack;

// Queue for customer service simulation
queue<string> customerQueue;

// Binary Search Tree for Account lookup
class AccountTreeNode {
public:
    string AccountNo;
    int Balance;
    AccountTreeNode* left;
    AccountTreeNode* right;

    AccountTreeNode(string accNo, int balance) : AccountNo(accNo), Balance(balance), left(nullptr), right(nullptr) {}
};

// Binary Search Tree functions
AccountTreeNode* insertIntoBST(AccountTreeNode* root, string accountNo, int balance) {
    if (root == nullptr) {
        return new AccountTreeNode(accountNo, balance);
    }
    if (accountNo < root->AccountNo) {
        root->left = insertIntoBST(root->left, accountNo, balance);
    } else {
        root->right = insertIntoBST(root->right, accountNo, balance);
    }
    return root;
}

AccountTreeNode* searchBST(AccountTreeNode* root, string accountNo) {
    if (root == nullptr || root->AccountNo == accountNo) {
        return root;
    }
    if (accountNo < root->AccountNo) {
        return searchBST(root->left, accountNo);
    }
    return searchBST(root->right, accountNo);
}

// Hash map for quick user authentication
unordered_map<string, string> userAuthMap;

// Global variable to manage linked list of accounts
Account* head = nullptr;

// Function to add account to linked list
void addAccountToList(Account* account) {
    if (!head) {
        head = account;
    } else {
        Account* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = account;
    }
}

// Function to display accounts (In-Order Traversal of BST)
void displayAccounts(AccountTreeNode* root) {
    if (!root) return;
    displayAccounts(root->left);
    cout << "Account No: " << root->AccountNo << " | Balance: " << root->Balance << endl;
    displayAccounts(root->right);
}

// Function to handle login
void login(Account& user) {
    system("cls");
    string id, pw;

    cout << "\tEnter Account No: ";
    cin >> id;
    cout << "\tEnter Password: ";
    cin >> pw;

    // Hash map lookup for authentication
    if (userAuthMap.find(id) != userAuthMap.end() && userAuthMap[id] == pw) {
        user.AccountNo = id;
        cout << "\tLogin Successful!" << endl;
        Sleep(2000);
    } else {
        cout << "\tInvalid Credentials!" << endl;
        Sleep(2000);
    }
}

// Function to open a new account (using BST for efficient lookup)
AccountTreeNode* openAccount(AccountTreeNode* root) {
    system("cls");
    Account* newAccount = new Account();
    cout << "\tEnter New Account Number: ";
    cin >> newAccount->AccountNo;
    cout << "\tEnter Password: ";
    cin >> newAccount->Password;
    newAccount->Balance = 0;

    // Add to linked list and BST
    addAccountToList(newAccount);
    root = insertIntoBST(root, newAccount->AccountNo, newAccount->Balance);

    // Add to hash map for quick authentication
    userAuthMap[newAccount->AccountNo] = newAccount->Password;

    cout << "\tAccount Created Successfully!" << endl;
    Sleep(2000);
    return root;
}

// Function to deposit cash
void depositCash(AccountTreeNode* root) {
    system("cls");
    string id;
    int cash;
    cout << "\tEnter Account No: ";
    cin >> id;
    cout << "\tEnter Cash Amount: ";
    cin >> cash;

    // Search in BST for account
    AccountTreeNode* account = searchBST(root, id);
    if (account) {
        account->Balance += cash;
        undoStack.push(make_pair(id, cash));  // Add to undo stack
        cout << "\tCash Deposited! New Balance: " << account->Balance << endl;
    } else {
        cout << "\tAccount Not Found!" << endl;
    }
    Sleep(2000);
}

// Function to undo last transaction
void undoTransaction(AccountTreeNode* root) {
    if (!undoStack.empty()) {
        pair<string, int> lastTransaction = undoStack.top();
        undoStack.pop();

        AccountTreeNode* account = searchBST(root, lastTransaction.first);
        if (account) {
            account->Balance -= lastTransaction.second;
            cout << "\tTransaction Undone! New Balance: " << account->Balance << endl;
        }
    } else {
        cout << "\tNo transactions to undo!" << endl;
    }
    Sleep(2000);
}

// Function to simulate customer service (using queue)
void simulateCustomerService() {
    if (!customerQueue.empty()) {
        string customer = customerQueue.front();
        customerQueue.pop();
        cout << "\tServing customer: " << customer << endl;
    } else {
        cout << "\tNo customers in queue!" << endl;
    }
    Sleep(2000);
}

int main() {
    AccountTreeNode* root = nullptr;
    Account user;

    while (true) {
        system("cls");
        cout << "\t1. Login\n\t2. Open New Account\n\t3. Deposit Cash\n\t4. Undo Last Transaction\n\t5. Display Accounts\n\t6. Simulate Customer Service\n\t7. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            login(user);
        } else if (choice == 2) {
            root = openAccount(root);
        } else if (choice == 3) {
            depositCash(root);
        } else if (choice == 4) {
            undoTransaction(root);
        } else if (choice == 5) {
            displayAccounts(root);
        } else if (choice == 6) {
            simulateCustomerService();
        } else if (choice == 7) {
            break;
        }
    }

    return 0;
}
