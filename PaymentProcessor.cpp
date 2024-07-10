#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <mutex>
#include <stdexcept>

using namespace std;

// Class to hold amount info
class Account {
public:

    int AccountId;
    string OwnerName;
    double Balance;

    Account(const int &accountId, const string &ownerName, const double &balance) : AccountId(accountId), OwnerName(move(ownerName)), Balance(balance) {}
};

// Class to hold transaction info
class Transaction {
public:

    int TransactionId;
    int FromAccountId;
    int ToAccountId;
    double Amount;
    time_t TransactionTS = 0;

    Transaction( const int &transactionId,
        const int &fromAccountId,
        const int &toAccountId,
        const double &amount,
        const time_t &transactionTS) : TransactionId(transactionId), FromAccountId(fromAccountId), ToAccountId(toAccountId), Amount(amount), TransactionTS(transactionTS) {}
};

// Class to process info
class PaymentProcessor {

private:

    // Mutex variable to be used on thread safe operations
    mutex mu;

    // unordered map for O(1)
    unordered_map<int, Account*> accountList;

    // List of Transactions
    vector<Transaction*> transactionList;

    // Last Transaction ID
    int currentTransactionId = 0;

    // Last Account  ID
    int currentAccountId = 0;

    // method to make a deposit
    bool Deposit(double amount, const int& id) {
        if (amount <= 0) {
            return false;
        }

        lock_guard<mutex> lock(mu);

        auto it = accountList.find(id);
        if (it != accountList.end()) {
            it->second->Balance += amount;
        }
        else {
            return false;
        }

        return true;
    }
    
    // method to make a withdraw
    bool Withdraw(double amount, const int& id) {
        if (amount <= 0) {
            return false;
        }

        lock_guard<mutex> lock(mu);

        auto it = accountList.find(id);
        if (it == accountList.end() || it->second->Balance < amount) {
            return false;
        }

        it->second->Balance -= amount;

        return true;
    }

    // method to add Transaction
    bool addTransaction(const double& amount, const int& from, const int& to) {

        lock_guard<mutex> lock(mu);

        currentTransactionId++;

        const int currenT = currentTransactionId;
        const auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());

        Transaction* transaction = new Transaction(currenT, from, to, amount, timenow);

        transactionList.push_back(transaction);

        return true;

    }

public:

    // destructor to destroy the items of the collections (every item is a pointer)
    ~PaymentProcessor() {

        for (auto& pair : accountList) {
            delete pair.second;
        }

        for (auto& item : transactionList) {
            delete item;
        }
    }

    // method to return the count of transaction
    int TransactionCount() {

        return transactionList.size();

    }

    // method to create account (returns AccountId)
    int createAccount(const string& ownerName, double initialBalance) {

        lock_guard<mutex> lock(mu);

        currentAccountId++;

        const int currenT = currentAccountId;

        Account* account = new Account(currenT, ownerName, initialBalance);

        accountList.insert(pair<int, Account*>(currenT, account));

        return currentAccountId;

    }
 
    // method to process de transaction
    bool processTransaction(double amount, const int& from, const int& to) {

        if (from == to) {
            
            return false;

        }

        if (Withdraw(amount, from)) {

            if (Deposit(amount, to)) {

                return addTransaction(from, to, amount);;
            }

        }

        

        return false;

    }

    // return method Balance
    double getAccountBalance(const int& id) {
        lock_guard<mutex> lock(mu);

        auto it = accountList.find(id);
        if (it == accountList.end()) {
            throw runtime_error("Account not found");
        }

        return it->second->Balance;
    }

};
