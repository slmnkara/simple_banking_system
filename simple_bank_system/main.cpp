#include <iostream>
#include <fstream>

class Account {
protected:
	int accountID = 0;
	std::string name;
	double balance = 0.0;
};

class BankAccount : public Account {
private:
	static int accountCounter;
public:
	BankAccount(std::string name, double balance) {
		this->accountID = accountCounter;
		accountCounter++;
		this->name = name;
		this->balance = balance;
	}

	std::string getName() const {
		return name;
	}

	void setName(std::string name) {
		this->name = name;
	}

	double getBalance() const {
		return balance;
	}

	void setBalance(double balance) {
		this->balance = balance;
	}

	int getAccount() const {
		return accountID;
	}

	void withdraw(double amount) {
		if (balance >= amount && amount > 0) {
			balance -= amount;
			std::cout << "Transaction successful. Current balance: " << balance << "\n";
		}
		else {
			std::cout << "Transaction failed. Current balance: " << balance << "\n";
		}
	}

	void deposit(double amount) {
		if (amount > 0) {
			balance += amount;
			std::cout << "Transaction successful. Current balance: " << balance << "\n";
		}
		else {
			std::cout << "Transaction failed. Current balance: " << balance << "\n";
		}
	}
	
	void transfer(BankAccount& bankAccount, double amount) {
		if (this->accountID == bankAccount.accountID) {
			std::cout << "Transaction failed. Accounts are the same.";
			return;
		}
		if (this->balance >= amount && amount > 0) {
			this->balance -= amount;
			bankAccount.balance += amount;
			std::cout << "Transaction successful.\n";
			this->displayInfo();
			bankAccount.displayInfo();
			// logging
			transferLog(accountID, bankAccount.accountID, amount);
		}
		else {
			std::cout << "Transaction failed.\n";
			this->displayInfo();
			bankAccount.displayInfo();
		}
	}

	void transferLog(int id1, int id2, double amount) {
		std::fstream file("log.txt", std::ios::app);
		if (!file) {
			std::cerr << "File could not opened!\n";
			return;
		}
		if (file.is_open()) {
			file << "Transfer info >>   ID: " << id1 << "   to   ID: " << id2 << "   Amount: " << amount << "\n";
			file.close();
		}
	}

	void displayInfo() {
		std::cout << "ID: " << accountID << ", Name: " << name << ", Balance: " << balance << "\n";
	}
};

int BankAccount::accountCounter = 1;

int main() {
	BankAccount ba1("Suleyman", 1000);
	BankAccount ba2("Mehmet", 3000);

	ba1.displayInfo();
	ba2.displayInfo();
	ba1.transfer(ba2, 100);
	ba2.transfer(ba1, 999);
	std::cout << "Final balances:\n";
	ba1.displayInfo();
	ba2.displayInfo();
}