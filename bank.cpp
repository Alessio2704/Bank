#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void pauseTerminal() {
    cin.ignore (numeric_limits<streamsize>::max(), '\n');
    cout << "Press any key to continue";
    cin.get();
}

class Account {
public:
    string usernameDB;
    string passwordDB;
    float balanceDB;
    string username;
    string password;
    float balance;

    Account() {}
    Account(string user) {
        this->getAccount(user);
    }

    bool exist(string user) {
        string const filename = user + ".dat";
        fstream file(filename, ios::in | ios::out | ios::binary);

        if (file.is_open()) {
            return true;
            file.close();
        } else {
            return false;
        }
    }

    void createAccount(string user) {
        system("clear");
        if (!exist(user)) {
            Account account;
            system("clear");
            cout << "Username: "; cin >> account.username;
            cout << "Password: "; cin >> account.password;
            cout << "Balance: "; cin >> account.balance;

            string const filename = user + ".dat";

            // inizialising the file as blank
            ofstream file_zero(filename);
            file_zero.close();

            fstream file(filename, ios::in | ios::out | ios::binary);
            if (file.is_open()) {
                file.write((char*)&account, sizeof(account));
                file.close();
                cout << "Account created succesfully" << endl;
                pauseTerminal();
            } else {
                cout << "Error writing to the file" << endl;
                pauseTerminal();
            }
        } else {
            cout << "An account for this user already exists." << endl;
            pauseTerminal();
        }
    }

    void saveAccountToDB(string user, Account account) {
        string const filename = user + ".dat";
        fstream file(filename, ios::in | ios::out | ios::binary);
        if (file.is_open()) {
            file.write((char*)&account, sizeof(account));
            file.close();
        } else {
            cout << "Error saving back data" << "User: " << user << endl;
        }

    }

void getAccount(string user) {
    Account account;
    string const filename = user + ".dat";   
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (file.is_open()) {
        file.read((char*)&account, sizeof(account));
        file.close();
        this ->usernameDB = account.username;
        this ->passwordDB = account.password;
        this ->balanceDB = account.balance;
    } else {
        cout << "Error loading to the file" << endl;
        pauseTerminal();
    }
}

void displayAccountData(string user) {
    getAccount(user);
    system("clear");
    cout << "User: " << user << endl;
    cout << "Username: " << usernameDB << endl;
    cout << "Balance: " << balanceDB << "$"<< endl;
    pauseTerminal();
}

void login(string user,bool* logged) {
    system("clear");
    if (exist(user)) {
        string username_login;
        string password_login;
        cout << "Enter Username: "; cin >> username_login;
        getAccount(user);

        if (username_login == usernameDB) {
            cout << "Enter Password: "; cin >> password_login;
            if (password_login == passwordDB) {
                *logged = true;
            } else {
                *logged = false;
                cout << "Wrong password" << endl;
                pauseTerminal();
            }
        } else {
            *logged = false;
            cout << "Wrong Username" << endl;
            pauseTerminal();
        }
    } else {
        cout << "Create credentials first." << endl;
        pauseTerminal();
    }
}

void chageCredentials(string user,bool* logged) {
    system("clear");
    string password_old; cout << "Enter old password: "; cin >> password_old;
    getAccount(user);

    if (password_old == passwordDB) {
        Account account;
        string username_new; cout << "New username: "; cin >> account.username;
        string password_new; cout << "New password: "; cin >> account.password;
        account.balance = balanceDB;
        string const filename = user + ".dat";
        fstream file(filename, ios::in | ios::out | ios::binary);

        if (file.is_open()) {
            file.write((char*)&account,sizeof(account));
            file.close();
            *logged = false;
            cout << "Login credentials changed succesfully" << endl;
            cout << "Logged out" << endl;
            pauseTerminal();
        } else {
            cout << "Error changing login credentials" << endl;
            pauseTerminal();
        }
    } else {
        cout << "Wrong password" << endl;
        pauseTerminal();
    }
}
};

class transferManager {
    public:
        string user;

        transferManager() {

        }
        transferManager(string* user) {
            this -> user = *user;
        }
        bool checkSecondAccountExistence(string user2) {
            if (Account().exist(user2)) {
                return true;
            } else {
                return false;
            }
        }

        bool checkFirstAccountBalance(int transactionAmount) {
            Account account1;
            account1.getAccount(user);

            if (transactionAmount <= account1.balanceDB) {
                return true;
            } else {
                return false;
            }
        }


        void transferMoney() {
            system("clear");
            cout << "Available balance: " << Account(user).balanceDB << "$" << endl << endl;
            string user2; cout << "Destinatary IBAN: "; cin >> user2;
            float transactionAmount;
            Account account1;
            Account account2;

            if (user2 != user) {
                if (checkSecondAccountExistence(user2)) {
                    cout << "Amount to transfer: "; cin >> transactionAmount;
                    if (checkFirstAccountBalance(transactionAmount)) {
                        if (transactionAmount > 0) {
                            account1.getAccount(user);
                            account2.getAccount(user2);
                            account1.username = account1.usernameDB;
                            account1.password = account1.passwordDB;
                            account1.balance = account1.balanceDB - transactionAmount;
                            account2.username = account2.usernameDB;
                            account2.password = account2.passwordDB;
                            account2.balance = account2.balanceDB + transactionAmount;
                            Account().saveAccountToDB(user,account1);
                            Account().saveAccountToDB(user2,account2);
                            cout << "Transaction ended with success" << endl;
                            pauseTerminal();
                        } else {
                            cout << "You cannot send a negative amount of money" << endl;
                            pauseTerminal();
                        }

                    } else {
                        cout << "Not sufficient funds to complete the operation" << endl;
                        pauseTerminal();
                    }
                } else {
                    cout << "Destinatary account does not exist" << endl;
                    pauseTerminal();
                }

                } else {
                    cout << "You can't send money to yourself" << endl;
                    pauseTerminal();
                }
        }
};

void firstMenu(bool* logged) {
    system("clear");
    if (*logged) {
        cout << "LOGGED  (Secure Area)" << endl << endl;
    } else {
        cout << "NOT LOGGED" << endl << endl;
    }
    cout << "1. Create credentials" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
}

void loggedMenu(string* user,bool* logged){
    system("clear");
    if (*logged) {
        cout << "LOGGED  (Secure Area)" << "    " << "Balance: " << Account(*user).balanceDB << "$" << endl << endl;
    } else {
        cout << "NOT LOGGED" << endl << endl;
    }
    cout << "1. Show account data " << endl;
    cout << "2. Change login credentials " << endl;
    cout << "3. Transfer " << endl;
    cout << "4. Log out " << endl;
}

void loggedMenuManager(string* user,bool* logged) {
    int choice_logged;
    do {
        loggedMenu(&*user,&*logged);
        cin >> choice_logged;
        switch(choice_logged) {
            case 1:
                Account().displayAccountData(*user);
                break;
            case 2:
                Account().chageCredentials(*user,&*logged);
                break;
            case 3:
                transferManager(&*user).transferMoney();
                break;
            case 4:
                *logged = false;
                break;
            
            default:
                cout << "Invalid input" << endl;
                pauseTerminal();
        }
    } while(*logged);
}

int main() {

    string user; cout << "User: "; cin >> user;
    bool logged;
    int choice;

    do {
        if (logged) {
            loggedMenuManager(&user,&logged);
        }
        firstMenu(&logged);
        cin >> choice;

        switch(choice) {
            case 1:
                Account().createAccount(user);
                break;
            case 2:
                Account().login(user,&logged);
                break;
            case 3:
                system("clear");
                cout << "Exited" << endl;
        }
    } while(choice != 3);
}