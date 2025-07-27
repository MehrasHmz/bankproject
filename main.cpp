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
