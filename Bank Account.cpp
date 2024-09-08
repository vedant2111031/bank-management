#include <iostream>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <iomanip>  // For std::setw and std::setfill
using namespace std;

class Account {
private:
    string AccountNo, Password, Role;
    int Balance;

public:
    Account() : AccountNo(""), Password(""), Role(""), Balance(0) {}

    void setAccountNo(string id) {
        AccountNo = id;
    }

    void setPassword(string pw) {
        Password = pw;
    }

    void setRole(string role) {
        Role = role;
    }

    void setBalance(int balance) {
        Balance = balance;
    }

    string getAccountNo() {
        return AccountNo;
    }

    string getPassword() {
        return Password;
    }

    string getRole() {
        return Role;
    }

    int getBalance() {
        return Balance;
    }
};

void login(Account &user) {
    system("cls");
    string role, id, pw;

    cout << "\tEnter Role (Admin/Employee/Customer): ";
    cin >> role;

    if (role == "Admin" || role == "Employee") {
        cout << "\tEnter ID: ";
        cin >> id;
        cout << "\tEnter Password: ";
        cin >> pw;
    } else if (role == "Customer") {
        cout << "\tEnter Account No: ";
        cin >> id;
        cout << "\tEnter Password: ";
        cin >> pw;
    } else {
        cout << "\tInvalid Role!" << endl;
        Sleep(2000);
        return;
    }

    ifstream infile("D:/Account.txt");
    if (!infile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(infile, line)) {
        stringstream ss(line);
        string userID, userPW, userRole;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance >> delimiter >> userRole;

        if (id == userID && pw == userPW && userRole == role) {
            found = true;
            user.setAccountNo(userID);
            user.setPassword(userPW);
            user.setBalance(userBalance);
            user.setRole(userRole);
            break;
        }
    }

    infile.close();

    if (found) {
        cout << "\tLogin Successful!" << endl;
        Sleep(2000);
    } else {
        cout << "\tInvalid Credentials!" << endl;
        Sleep(2000);
    }
}

string generateAccountNo() {
    ifstream infile("D:/Account.txt");
    int lastNumber = 0;
    string line;

    while (getline(infile, line)) {
        stringstream ss(line);
        string userID;
        char delimiter;
        ss >> userID >> delimiter;
        if (!userID.empty()) {
            lastNumber = stoi(userID);
        }
    }
    infile.close();

    return to_string(lastNumber + 1);  // Generate next sequential number
}

void openAccount(string role) {
    system("cls");
    Account user;
    string name, dob, address, pw, accountNo;

    cout << "\tEnter Name: ";
    cin.ignore();  // To clear the input buffer
    getline(cin, name);

    cout << "\tEnter Date of Birth (DD/MM/YYYY): ";
    getline(cin, dob);

    cout << "\tEnter Address: ";
    getline(cin, address);

    cout << "\tEnter Password: ";
    cin >> pw;

    // Generate a new Account No
    accountNo = generateAccountNo();

    user.setAccountNo(accountNo);
    user.setPassword(pw);
    user.setRole(role);
    user.setBalance(0);

    ofstream outfile("D:/Account.txt", ios::app);
    if (!outfile) {
        cout << "\tError: File Can't Open!" << endl;
    } else {
        outfile << user.getAccountNo() << " : " << user.getPassword() << " : " << user.getBalance() << " : " << role << " : " << name << " : " << dob << " : " << address << endl;
        cout << "\t" << role << " Account Created Successfully!" << endl;
        cout << "\tAccount Number: " << user.getAccountNo() << endl;
    }
    outfile.close();
    Sleep(5000);
}

void addEmployeeOrAdmin(string role) {
    system("cls");
    string id, pw;

    cout << "\tEnter Employee ID: ";
    cin >> id;

    cout << "\tEnter A Strong Password: ";
    cin >> pw;

    ofstream outfile("D:/Account.txt", ios::app);
    if (!outfile) {
        cout << "\tError: File Can't Open!" << endl;
    } else {
        outfile << id << " : " << pw << " : 0 : " << role << " : N/A : N/A : N/A" << endl;
        cout << "\t" << role << " Created Successfully!" << endl;
    }
    outfile.close();
    Sleep(5000);
}

void addCash() {
    system("cls");
    string id;
    cout << "\tEnter Account No: ";
    cin >> id;

    ifstream infile("D:/Account.txt");
    ofstream outfile("D:/Account Temp.txt");
    if (!infile || !outfile) {
        cout << "\tError: File Can't Open!" << endl;
    }

    string line;
    bool found = false;

    while (getline(infile, line)) {
        stringstream ss(line);
        string userID, userPW, userRole;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance >> delimiter >> userRole;
        if (id == userID) {
            found = true;
            int cash;
            cout << "\tEnter Cash: ";
            cin >> cash;

            int newBalance = userBalance + cash;
            userBalance = newBalance;
            outfile << userID << " : " << userPW << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
            cout << "\tNew Balance Is: " << userBalance << endl;
        } else {
            outfile << line << endl;
        }
    }
    if (!found) {
        cout << "\tEnter Valid Account No!" << endl;
    }
    outfile.close();
    infile.close();
    remove("D:/Account.txt");
    rename("D:/Account Temp.txt", "D:/Account.txt");
    Sleep(5000);
}

void withdraw() {
    system("cls");
    string id, pw;
    cout << "\tEnter Account No: ";
    cin >> id;
    cout << "\tEnter Password: ";
    cin >> pw;

    ifstream infile("D:/Account.txt");
    ofstream outfile("D:/Account Temp.txt");
    if (!infile || !outfile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }

    string line;
    bool found = false;
    while (getline(infile, line)) {
        stringstream ss(line);
        string userID, userPW, userRole;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance >> delimiter >> userRole;

        if (id == userID && pw == userPW && userRole == "Admin") {  // Only Admin can withdraw
            found = true;
            int cash;
            cout << "\tEnter Cash: ";
            cin >> cash;
            if (cash <= userBalance) {
                int newBalance = userBalance - cash;
                userBalance = newBalance;
                outfile << userID << " : " << userPW << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
                cout << "\tTransaction Was Successful!" << endl;
                cout << "\tRemaining Balance: " << userBalance << endl;
            } else {
                cout << "\tLow Balance!" << endl;
                outfile << userID << " : " << userPW << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
            }
        } else if (id == userID && pw == userPW) {
            cout << "\tOnly Admin can withdraw cash!" << endl;
            outfile << userID << " : " << userPW << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
        } else {
            outfile << line << endl;
        }
    }
    if (!found) {
        cout << "\tInvalid Account No or Password!" << endl;
    }
    outfile.close();
    infile.close();
    remove("D:/Account.txt");
    rename("D:/Account Temp.txt", "D:/Account.txt");
    Sleep(5000);
}

void checkBalance(Account &user) {
    system("cls");
    ifstream infile("D:/Account.txt");
    if (!infile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(infile, line)) {
        stringstream ss(line);
        string userID, userPW, userRole;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance >> delimiter >> userRole;

        if (user.getAccountNo() == userID) {
            found = true;
            cout << "\tYour Balance: " << userBalance << endl;
            break;
        }
    }

    if (!found) {
        cout << "\tAccount Not Found!" << endl;
    }
    infile.close();
    Sleep(5000);
}

void changePassword() {
    system("cls");
    string id, newPassword;
    cout << "\tEnter Account No or Employee ID: ";
    cin >> id;

    cout << "\tEnter New Password: ";
    cin >> newPassword;

    ifstream infile("D:/Account.txt");
    ofstream outfile("D:/Account Temp.txt");
    if (!infile || !outfile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }

    string line;
    bool found = false;
    while (getline(infile, line)) {
        stringstream ss(line);
        string userID, userPW, userRole;
        int userBalance;
        char delimiter;

        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance >> delimiter >> userRole;

        if (id == userID) {
            found = true;
            outfile << userID << " : " << newPassword << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
            cout << "\tPassword Changed Successfully!" << endl;
        } else {
            outfile << line << endl;
        }
    }

    if (!found) {
        cout << "\tAccount No or Employee ID Not Found!" << endl;
    }

    outfile.close();
    infile.close();
    remove("D:/Account.txt");
    rename("D:/Account Temp.txt", "D:/Account.txt");
    Sleep(5000);
}

void transferFunds() {
    system("cls");
    string senderID, receiverID, senderPW;
    int amount;

    cout << "\tEnter Your Account No: ";
    cin >> senderID;

    cout << "\tEnter Your Password: ";
    cin >> senderPW;

    cout << "\tEnter Receiver's Account No: ";
    cin >> receiverID;

    cout << "\tEnter Amount to Transfer: ";
    cin >> amount;

    ifstream infile("D:/Account.txt");
    ofstream outfile("D:/Account Temp.txt");
    if (!infile || !outfile) {
        cout << "\tError: File Can't Open!" << endl;
        return;
    }

    string line;
    bool senderFound = false, receiverFound = false;
    stringstream ss;
    string userID, userPW, userRole;
    int userBalance;
    char delimiter;

    while (getline(infile, line)) {
        ss.clear();
        ss.str(line);
        ss >> userID >> delimiter >> userPW >> delimiter >> userBalance >> delimiter >> userRole;

        if (senderID == userID) {
            senderFound = true;
            if (senderPW == userPW) {
                if (amount <= userBalance) {
                    userBalance -= amount;
                    outfile << userID << " : " << userPW << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
                } else {
                    cout << "\tInsufficient Balance!" << endl;
                    outfile << line << endl;
                }
            } else {
                cout << "\tIncorrect Password!" << endl;
                outfile << line << endl;
            }
        } else if (receiverID == userID) {
            receiverFound = true;
            userBalance += amount;
            outfile << userID << " : " << userPW << " : " << userBalance << " : " << userRole << " : N/A : N/A : N/A" << endl;
        } else {
            outfile << line << endl;
        }
    }

    if (!senderFound) {
        cout << "\tSender Account Not Found!" << endl;
    } else if (!receiverFound) {
        cout << "\tReceiver Account Not Found!" << endl;
    } else {
        cout << "\tTransfer Successful!" << endl;
    }

    outfile.close();
    infile.close();
    remove("D:/Account.txt");
    rename("D:/Account Temp.txt", "D:/Account.txt");
    Sleep(5000);
}

int main() {
    bool loggedIn = false;
    Account user;

    while (!loggedIn) {
        system("cls");
        cout << "\t*****************************************" << endl;
        cout << "\tBank Account Management System" << endl;
        cout << "\t*****************************************" << endl;
        cout << "\t1. Login" << endl;
        cout << "\t2. Open New Account" << endl;
        cout << "\t3. Add Admin" << endl;
        cout << "\t4. Add Employee" << endl;
        cout << "\t5. Exit" << endl;
        cout << "\tEnter Your Choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            login(user);
            if (!user.getAccountNo().empty()) {
                loggedIn = true;
                cout << "\tWelcome " << user.getRole() << " to Bank Account Management System" << endl;
            }
        } else if (choice == 2) {
            system("cls");
            cout << "\t1. Open Admin Account" << endl;
            cout << "\t2. Open Employee Account" << endl;
            cout << "\t3. Open Customer Account" << endl;
            cout << "\tEnter Your Choice: ";
            int accountChoice;
            cin >> accountChoice;

            if (accountChoice == 1) {
                openAccount("Admin");
            } else if (accountChoice == 2) {
                openAccount("Employee");
            } else if (accountChoice == 3) {
                openAccount("Customer");
            } else {
                cout << "\tInvalid Option!" << endl;
                Sleep(2000);
            }
        } else if (choice == 3) {
            addEmployeeOrAdmin("Admin");
        } else if (choice == 4) {
            addEmployeeOrAdmin("Employee");
        } else if (choice == 5) {
            cout << "\tGoodbye!" << endl;
            Sleep(2000);
            return 0;
        } else {
            cout << "\tInvalid Choice!" << endl;
            Sleep(2000);
        }
    }

    while (loggedIn) {
        system("cls");
        cout << "\t*****************************************" << endl;
        cout << "\tWelcome " << user.getRole() << " to Bank Account Management System" << endl;
        cout << "\t*****************************************" << endl;

        if (user.getRole() == "Admin") {
            cout << "\t1. Add Cash" << endl;
            cout << "\t2. Withdraw Cash" << endl;
            cout << "\t3. Change Password" << endl;
            cout << "\t4. Logout" << endl;
        } else if (user.getRole() == "Employee") {
            cout << "\t1. Add Cash" << endl;
            cout << "\t2. Logout" << endl;
        } else if (user.getRole() == "Customer") {
            cout << "\t1. Check Balance" << endl;
            cout << "\t2. Transfer Funds" << endl;
            cout << "\t3. Logout" << endl;
        } else {
            cout << "\tInvalid Role!" << endl;
            loggedIn = false;
            Sleep(2000);
            continue;
        }

        int val;
        cout << "\tEnter Your Choice: ";
        cin >> val;

        if (val == 1) {
            if (user.getRole() == "Admin" || user.getRole() == "Employee") {
                addCash();
            } else if (user.getRole() == "Customer") {
                checkBalance(user);
            }
        } else if (val == 2) {
            if (user.getRole() == "Admin") {
                withdraw();
            } else if (user.getRole() == "Customer") {
                transferFunds();
            }
        } else if (val == 3) {
            if (user.getRole() == "Admin") {
                changePassword();
            } else if (user.getRole() == "Customer") {
                loggedIn = false;
            }
        } else if (val == 4 || (val == 2 && user.getRole() == "Customer")) {
            loggedIn = false;
        } else {
            cout << "\tInvalid Option!" << endl;
            Sleep(2000);
        }
    }

    return 0;
}
