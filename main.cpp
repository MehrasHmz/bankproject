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
