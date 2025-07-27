#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

// ======================== Helpers =========================

string generateCardNumber() {
    string card = "6274";
    for (int i = 0; i < 12; i++)
        card += to_string(rand() % 10);
    return card;
}

string generateShaba() {
    string shaba = "IR";
    for (int i = 0; i < 24; i++)
        shaba += to_string(rand() % 10);
    return shaba;
}

string generateAccountNumber() {
    string acc = "";
    for (int i = 0; i < 12; i++)
        acc += to_string(rand() % 10);
    return acc;
}

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return to_string(ltm->tm_year + 1900 + 5) + "/" + to_string(ltm->tm_mon + 1);
}
// ==================== Account Classes ========================

class BankAccount {
protected:
    string cardNumber, shabaNumber, accountNumber;
    string cvv2, expDate, pin, secondPassword, secondPasswordDynamic;
    double balance;
public:
    BankAccount() {
        cardNumber = generateCardNumber();
        shabaNumber = generateShaba();
        accountNumber = generateAccountNumber();
        cvv2 = to_string(100 + rand() % 900);
        expDate = getCurrentDate();
        pin = "1234";
        secondPassword = "2222";
        secondPasswordDynamic = "999999";
        balance = 0.0;
    }

    virtual string getType() const = 0;

    string getCardNumber() const { return cardNumber; }
    string getShaba() const { return shabaNumber; }
    string getAccountNumber() const { return accountNumber; }

    double getBalance() const { return balance; }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void deposit(double amount) {
        balance += amount;
    }

    void displayInfo() const {
        cout << "Account Type: " << getType() << endl;
        cout << "Card Number: " << cardNumber << ", CVV2: " << cvv2 << ", Expiration: " << expDate << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << " Toman" << endl;
    }

    virtual ~BankAccount() {}
};

class DepositAccount : public BankAccount {
public:
    string getType() const override { return "Deposit"; }
};

class CurrentAccount : public BankAccount {
public:
    string getType() const override { return "Current"; }
};

class GharzAccount : public BankAccount {
public:
    string getType() const override { return "Loan"; }
};
// ==================== User Class ========================

class User {
protected:
    string name, lastName, nationalCode;
    int age;
    string username, password;
    vector<BankAccount*> accounts;

public:
    User(string name, string last, string nat, int age, string user, string pass)
            : name(name), lastName(last), nationalCode(nat), age(age), username(user), password(pass) {}

    virtual string getRole() const = 0;

    string getUsername() const { return username; }
    string getName() const { return name; }

    bool login(string user, string pass) {
        return (username == user && password == pass);
    }

    bool addAccount(BankAccount* acc) {
        if (accounts.size() >= 5) return false;
        accounts.push_back(acc);
        return true;
    }

    BankAccount* findAccountByCard(string card) {
        for (auto acc : accounts) {
            if (acc->getCardNumber() == card) return acc;
        }
        return nullptr;
    }

    void showAccounts() {
        for (auto acc : accounts)
            acc->displayInfo();
    }

    vector<BankAccount*>& getAccounts() { return accounts; }

    virtual ~User() {
        for (auto acc : accounts)
            delete acc;
    }
};
// ==================== Customer ========================

class Customer : public User {
public:
    Customer(string name, string last, string nat, int age, string user, string pass)
            : User(name, last, nat, age, user, pass) {}

    string getRole() const override { return "Customer"; }

    bool transfer(string toCard, double amount, vector<User*>& allUsers) {
        for (auto myAcc : accounts) {
            if (myAcc->getBalance() >= amount) {
                for (auto user : allUsers) {
                    for (auto acc : user->getAccounts()) {
                        if (acc->getCardNumber() == toCard) {
                            myAcc->withdraw(amount);
                            acc->deposit(amount);
                            cout << "Transfer successful!" << endl;
                            return true;
                        }
                    }
                }
            }
        }
        cout << "Transfer failed. Card not found or insufficient funds." << endl;
        return false;
    }
};
// ==================== Admin ========================

class Admin : public User {
public:
    Admin(string name, string last, string nat, int age, string user, string pass)
            : User(name, last, nat, age, user, pass) {}

    string getRole() const override { return "Admin"; }

    void viewAllUsers(vector<User*>& users) {
        for (auto user : users) {
            cout << "Username: " << user->getUsername() << ", Name: " << user->getName()
                 << ", Role: " << user->getRole() << endl;
            user->showAccounts();
            cout << "---------------------" << endl;
        }
    }

    void removeCustomer(string username, vector<User*>& users) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUsername() == username && users[i]->getRole() == "Customer") {
                delete users[i];
                users.erase(users.begin() + i);
                cout << "Customer removed." << endl;
                return;
            }
        }
        cout << "User not found or is an admin." << endl;
    }
};

// ==================== Menu Functions ========================

void registerUser(vector<User*>& users) {
    string name, last, nat, user, pass, role;
    int age;

    cout << "Register New User" << endl;
    cout << "First Name: "; cin >> name;
    cout << "Last Name: "; cin >> last;
    cout << "National Code: "; cin >> nat;
    cout << "Age: "; cin >> age;
    cout << "Username: "; cin >> user;
    cout << "Password: "; cin >> pass;
    cout << "Role (admin/customer): "; cin >> role;

    if (role == "admin") {
        users.push_back(new Admin(name, last, nat, age, user, pass));
        cout << "Admin created." << endl;
    } else if (role == "customer") {
        Customer* c = new Customer(name, last, nat, age, user, pass);
        int count = 0;
        while (count < 3) {
            cout << "Add account type (1: Deposit, 2: Current, 3: Loan, 0: Done): ";
            int type; cin >> type;
            if (type == 0) break;
            BankAccount* acc = nullptr;
            if (type == 1) acc = new DepositAccount();
            else if (type == 2) acc = new CurrentAccount();
            else if (type == 3) acc = new GharzAccount();
            if (acc && c->addAccount(acc)) {
                cout << "Account added." << endl;
                count++;
            }
        }
        users.push_back(c);
        cout << "Customer created." << endl;
    } else {
        cout << "Invalid role!" << endl;
    }
}

User* loginUser(const vector<User*>& users) {
    string u, p;
    cout << "Username: ";
    cin >> u;
    cout << "Password: ";
    cin >> p;

    for (auto user : users) {
        if (user->login(u, p)) {
            cout << "Login successful. Role: " << user->getRole() << endl;
            return user;
        }
    }
    cout << "Login failed." << endl;
    return nullptr;
}

void adminMenu(Admin* admin, vector<User*>& users) {
    int choice;
    do {
        cout << "\n[Admin Menu]" << endl;
        cout << "1. View All Users\n2. Remove Customer\n0. Logout\nChoice: ";
        cin >> choice;
        if (choice == 1) {
            admin->viewAllUsers(users);
        } else if (choice == 2) {
            string uname;
            cout << "Customer Username to Remove: ";
            cin >> uname;
            admin->removeCustomer(uname, users);
        }
    } while (choice != 0);
}

void customerMenu(Customer* cust, vector<User*>& users) {
    int choice;
    do {
        cout << "\n[Customer Menu]" << endl;
        cout << "1. View Accounts\n2. Transfer Money\n0. Logout\nChoice: ";
        cin >> choice;
        if (choice == 1) {
            cust->showAccounts();
        } else if (choice == 2) {
            string toCard;
            double amount;
            cout << "Target Card Number: ";
            cin >> toCard;
            cout << "Amount: ";
            cin >> amount;
            cust->transfer(toCard, amount, users);
        }
    } while (choice != 0);
}

// ==================== Main ========================

int main() {
    srand(time(0));
    vector<User*> users;

    int choice;
    do {
        cout << "\n====== Banking System ======" << endl;
        cout << "1. Register\n2. Login\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            registerUser(users);
        } else if (choice == 2) {
            User* loggedIn = loginUser(users);
            if (loggedIn) {
                if (loggedIn->getRole() == "Admin") {
                    adminMenu(static_cast<Admin*>(loggedIn), users);
                } else if (loggedIn->getRole() == "Customer") {
                    customerMenu(static_cast<Customer*>(loggedIn), users);
                }
            }
        }
    } while (choice != 0);

    for (auto user : users)
        delete user;

    cout << "Program terminated." << endl;
    return 0;
}