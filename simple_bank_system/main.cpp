#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

string getTime() {
	auto now = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(now);

	ostringstream oss;
	oss << put_time(localtime(&t), "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

class Logger {
private:
	Logger() {}
	string filename = "log.txt";

	void writeToFile(const string& message) {
		ofstream file(filename, ios::app);
		if (file.is_open()) {
			file << message << "\n";
		}
	}
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

	void logTransaction(const string& type, int accountID, double amount, bool status, int receiverID = -1) {
		ostringstream oss;
		oss << "time=" << getTime() << " type=" << type;
		// If transaciton type is transfer
		if (receiverID != -1) oss << " senderID=" << accountID << " receiverID=" << receiverID;
		else oss << " accountID=" << accountID;
		oss << " amount=" << amount
			<< " status=" << ((status) ? "SUCCESSFUL" : "FAILED");
		writeToFile(oss.str());
		cout << oss.str() << endl;
	}
};

class BankAccount {
private:
	int accountID = 0;
	string name;
	double balance = 0.0;
	static int accountCounter;
public:
	BankAccount(string name_input, double balance_input) {
		this->accountID = accountCounter;
		this->name = name;
		this->balance = balance;
		accountCounter++;
	}

	string getName() const {
		return name;
	}

	void setName(string name) {
		this->name = name;
	}

	double getBalance() const {
		return balance;
	}

	void setBalance(double balance) {
		this->balance = balance;
	}

	int getAccountID() const {
		return accountID;
	}

	string getAccountInfo() {
		ostringstream oss;
		oss << "accountID=" << accountID
			<< " name=" << name
			<< " balance=" << balance << "\n";
		return oss.str();
	}

	void withdraw(double amount) {
		bool status = false;
		if (amount <= this->balance && amount > 0) {
			this->balance -= amount;
			status = true;
		}
		Logger::getInstance().logTransaction("withdraw", accountID, amount, status);
	}

	void deposit(double amount) {
		bool status = false;
		if (amount > 0) {
			this->balance += amount;
			status = true;
		}
		Logger::getInstance().logTransaction("deposit", accountID, amount, status);
	}

	void transfer(BankAccount& receiver, double amount) {
		auto* sender = this;
		bool status = false;
		if (sender->accountID != receiver.accountID) {
			if (amount <= sender->balance && amount > 0) {
				sender->balance -= amount;
				receiver.balance += amount;
				status = true;
			}
		}
		Logger::getInstance().logTransaction("transfer", sender->accountID, amount, status, receiver.accountID);
	}
};

int BankAccount::accountCounter = 1;

int main() {
	BankAccount ba1("Suleyman", 1000);
	BankAccount ba2("Mehmet", 3000);
	ba1.transfer(ba2, 100);
	ba1.deposit(10000);
	ba2.withdraw(400);
	ba2.transfer(ba1, 999);
	ba1.transfer(ba1, 200);
	ba1.transfer(ba2, 100000);
}