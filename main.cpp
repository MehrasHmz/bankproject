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