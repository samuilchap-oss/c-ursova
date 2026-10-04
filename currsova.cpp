#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;


class Transaction
{
private:
    int id;
    string date;
    double amount;
    string category;
    string description;
    bool isIncome;

public:

    Transaction()
    {
        id = 0;
        amount = 0;
        isIncome = false;
    }

    Transaction(int id,
        string date,
        double amount,
        string category,
        string description,
        bool isIncome)
    {
        this->id = id;
        this->date = date;
        this->amount = amount;
        this->category = category;
        this->description = description;
        this->isIncome = isIncome;
    }

    int getId() const
    {
        return id;
    }

    string getDate() const
    {
        return date;
    }

    double getAmount() const
    {
        return amount;
    }

    string getCategory() const
    {
        return category;
    }

    string getDescription() const
    {
        return description;
    }

    bool getIsIncome() const
    {
        return isIncome;
    }

    void display() const
    {
        cout << left
            << setw(5) << id
            << setw(15) << date
            << setw(12) << (isIncome ? "Income" : "Expense")
            << setw(12) << fixed << setprecision(2) << amount
            << setw(18) << category
            << description << endl;
    }
};



class FinanceManager
{
private:
    vector<Transaction> transactions;
    double monthlyBudget;
    int nextId;

public:

    FinanceManager()
    {
        monthlyBudget = 0;
        nextId = 1;
    }


    void addIncome()
    {
        string date;
        string category;
        string description;
        double amount;

        cout << "\nEnter date (DD.MM.YYYY): ";
        cin >> date;

        cout << "Enter amount: ";
        cin >> amount;

        if (cin.fail() || amount <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount.\n";
            return;
        }

        cin.ignore();

        cout << "Enter category: ";
        getline(cin, category);

        cout << "Enter description: ";
        getline(cin, description);

        transactions.push_back(
            Transaction(
                nextId++,
                date,
                amount,
                category,
                description,
                true
            )
        );

        cout << "Income added successfully.\n";
    }


    void addExpense()
    {
        string date;
        string category;
        string description;
        double amount;

        cout << "\nEnter date (DD.MM.YYYY): ";
        cin >> date;

        cout << "Enter amount: ";
        cin >> amount;

        if (cin.fail() || amount <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount.\n";
            return;
        }

        cin.ignore();

        cout << "Enter category: ";
        getline(cin, category);

        cout << "Enter description: ";
        getline(cin, description);

        transactions.push_back(
            Transaction(
                nextId++,
                date,
                amount,
                category,
                description,
                false
            )
        );

        cout << "Expense added successfully.\n";

        checkBudget();
    }


    void showTransactions() const
    {
        if (transactions.empty())
        {
            cout << "\nNo transactions found.\n";
            return;
        }

        cout << "\n";
        cout << left
            << setw(5) << "ID"
            << setw(15) << "Date"
            << setw(12) << "Type"
            << setw(12) << "Amount"
            << setw(18) << "Category"
            << "Description" << endl;

        cout << string(90, '-') << endl;

        for (const auto& transaction : transactions)
        {
            transaction.display();
        }
    }


    void deleteTransaction()
    {
        int id;

        cout << "\nEnter transaction ID: ";
        cin >> id;

        auto it = remove_if(
            transactions.begin(),
            transactions.end(),
            [id](const Transaction& transaction)
            {
                return transaction.getId() == id;
            }
        );

        if (it != transactions.end())
        {
            transactions.erase(it, transactions.end());
            cout << "Transaction deleted successfully.\n";
        }
        else
        {
            cout << "Transaction not found.\n";
        }
    }


    void searchTransactions() const
    {
        string category;

        cin.ignore();

        cout << "\nEnter category to search: ";
        getline(cin, category);

        bool found = false;

        for (const auto& transaction : transactions)
        {
            if (transaction.getCategory() == category)
            {
                if (!found)
                {
                    cout << "\nSearch results:\n";
                    cout << string(90, '-') << endl;
                }

                transaction.display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "No transactions found in this category.\n";
        }
    }


    void sortTransactions()
    {
        if (transactions.empty())
        {
            cout << "\nNo transactions to sort.\n";
            return;
        }

        int choice;

        cout << "\n1. Sort by amount ascending\n";
        cout << "2. Sort by amount descending\n";
        cout << "Choose option: ";
        cin >> choice;

        if (choice == 1)
        {
            sort(
                transactions.begin(),
                transactions.end(),
                [](const Transaction& a, const Transaction& b)
                {
                    return a.getAmount() < b.getAmount();
                }
            );

            cout << "Transactions sorted in ascending order.\n";
        }
        else if (choice == 2)
        {
            sort(
                transactions.begin(),
                transactions.end(),
                [](const Transaction& a, const Transaction& b)
                {
                    return a.getAmount() > b.getAmount();
                }
            );

            cout << "Transactions sorted in descending order.\n";
        }
        else
        {
            cout << "Invalid option.\n";
        }
    }


    double calculateBalance() const
    {
        double balance = 0;

        for (const auto& transaction : transactions)
        {
            if (transaction.getIsIncome())
            {
                balance += transaction.getAmount();
            }
            else
            {
                balance -= transaction.getAmount();
            }
        }

        return balance;
    }


    void showStatistics() const
    {
        double totalIncome = 0;
        double totalExpenses = 0;

        for (const auto& transaction : transactions)
        {
            if (transaction.getIsIncome())
            {
                totalIncome += transaction.getAmount();
            }
            else
            {
                totalExpenses += transaction.getAmount();
            }
        }

        cout << "\n====================================\n";
        cout << "        FINANCIAL STATISTICS\n";
        cout << "====================================\n";

        cout << fixed << setprecision(2);

        cout << "Total income:    " << totalIncome << " UAH\n";
        cout << "Total expenses:  " << totalExpenses << " UAH\n";
        cout << "Current balance: " << calculateBalance() << " UAH\n";

        if (monthlyBudget > 0)
        {
            cout << "Monthly budget:  " << monthlyBudget << " UAH\n";

            if (totalExpenses > monthlyBudget)
            {
                cout << "WARNING: Monthly budget exceeded!\n";
            }
            else
            {
                cout << "Remaining budget: "
                    << monthlyBudget - totalExpenses
                    << " UAH\n";
            }
        }

        cout << "====================================\n";
    }


    void setBudget()
    {
        cout << "\nEnter monthly budget: ";
        cin >> monthlyBudget;

        if (cin.fail() || monthlyBudget < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid budget.\n";
            monthlyBudget = 0;

            return;
        }

        cout << "Monthly budget set successfully.\n";
    }


    void checkBudget() const
    {
        if (monthlyBudget <= 0)
        {
            return;
        }

        double expenses = 0;

        for (const auto& transaction : transactions)
        {
            if (!transaction.getIsIncome())
            {
                expenses += transaction.getAmount();
            }
        }

        if (expenses > monthlyBudget)
        {
            cout << "\nWARNING: You have exceeded your monthly budget!\n";
        }
    }


    void saveToFile() const
    {
        ofstream file("finance_data.txt");

        if (!file)
        {
            cout << "Error opening file for saving.\n";
            return;
        }

        file << monthlyBudget << endl;

        for (const auto& transaction : transactions)
        {
            file << transaction.getId() << "|"
                << transaction.getDate() << "|"
                << transaction.getAmount() << "|"
                << transaction.getCategory() << "|"
                << transaction.getDescription() << "|"
                << transaction.getIsIncome()
                << endl;
        }

        file.close();

        cout << "Data saved successfully.\n";
    }


    void loadFromFile()
    {
        ifstream file("finance_data.txt");

        if (!file)
        {
            cout << "No saved data found.\n";
            return;
        }

        transactions.clear();

        file >> monthlyBudget;
        file.ignore();

        string line;

        while (getline(file, line))
        {
            size_t pos = 0;
            vector<string> data;

            while ((pos = line.find('|')) != string::npos)
            {
                data.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }

            data.push_back(line);

            if (data.size() == 6)
            {
                int id = stoi(data[0]);
                string date = data[1];
                double amount = stod(data[2]);
                string category = data[3];
                string description = data[4];
                bool isIncome = stoi(data[5]);

                transactions.push_back(
                    Transaction(
                        id,
                        date,
                        amount,
                        category,
                        description,
                        isIncome
                    )
                );

                if (id >= nextId)
                {
                    nextId = id + 1;
                }
            }
        }

        file.close();

        cout << "Data loaded successfully.\n";
    }
};


void showMenu()
{
    cout << "\n";
    cout << "====================================\n";
    cout << "       PERSONAL FINANCE MANAGER\n";
    cout << "====================================\n";
    cout << "1. Add Income\n";
    cout << "2. Add Expense\n";
    cout << "3. View Transactions\n";
    cout << "4. Delete Transaction\n";
    cout << "5. Search Transactions\n";
    cout << "6. Sort Transactions\n";
    cout << "7. View Statistics\n";
    cout << "8. Set Monthly Budget\n";
    cout << "9. Save Data\n";
    cout << "10. Load Data\n";
    cout << "0. Exit\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";
}


int main()
{
    FinanceManager manager;

    manager.loadFromFile();

    int choice;

    do
    {
        showMenu();
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            manager.addIncome();
            break;

        case 2:
            manager.addExpense();
            break;

        case 3:
            manager.showTransactions();
            break;

        case 4:
            manager.deleteTransaction();
            break;

        case 5:
            manager.searchTransactions();
            break;

        case 6:
            manager.sortTransactions();
            break;

        case 7:
            manager.showStatistics();
            break;

        case 8:
            manager.setBudget();
            break;

        case 9:
            manager.saveToFile();
            break;

        case 10:
            manager.loadFromFile();
            break;

        case 0:
            manager.saveToFile();
            cout << "\nThank you for using Personal Finance Manager!\n";
            break;

        default:
            cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}