#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Customer Class
class Customer {
private:
    string name;
    string contact;
    vector<pair<string, int>> purchaseHistory; // {Product Name, Quantity}

public:
    Customer(string n, string c) : name(n), contact(c) {}

    void addPurchase(string product, int quantity) {
        purchaseHistory.push_back({product, quantity});
    }

    void displayHistory() {
        cout << "\nPurchase History for " << name << " (" << contact << "):\n";
        for (const auto& item : purchaseHistory) {
            cout << "- " << item.first << " x" << item.second << endl;
        }
    }

    string getName() { return name; }
    string getContact() { return contact; }

    string serialize() {
        stringstream ss;
        ss << name << " " << contact;
        for (const auto& item : purchaseHistory) {
            ss << " " << item.first << " " << item.second;
        }
        return ss.str();
    }

    static Customer deserialize(const string& line) {
        stringstream ss(line);
        string name, contact, product;
        int quantity;
        ss >> name >> contact;
        Customer customer(name, contact);
        while (ss >> product >> quantity) {
            customer.addPurchase(product, quantity);
        }
        return customer;
    }
};

// Store Class (Manages Inventory & Customers)
class Store {
private:
    map<string, map<string, vector<Product*>>> categorizedInventory;
    map<string, double> salesReport;
    vector<Customer> customers;

public:
    Store() {
        loadProducts("food.txt", "Food");
        loadProducts("stationery.txt", "Stationery");
        loadProducts("electronics.txt", "Electronics & Entertainment");
        loadProducts("sports.txt", "Sports & Exercise");
        loadProducts("comics.txt", "Comic and Manga");
        loadProducts("pharmaceuticals.txt", "Pharmaceuticals");
        loadCustomers();
    }

    // Load products from text files
    void loadProducts(const string& filename, const string& category) {
        ifstream file(filename);
        if (!file) {
            cout << "Error: Unable to open " << filename << endl;
            return;
        }

        string subcategory, name;
        double price;
        int quantity;
        while (file >> subcategory >> name >> price >> quantity) {
            categorizedInventory[category][subcategory].push_back(new Product(name, price, quantity));
        }
        file.close();
        cout << "Loaded " << category << " products from " << filename << endl;
    }

    // Load customers from file
    void loadCustomers() {
        ifstream file("customers.txt");
        if (!file) {
            cout << "No previous customer records found.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            customers.push_back(Customer::deserialize(line));
        }
        file.close();
        cout << "Customer profiles loaded successfully.\n";
    }

    // Save customers to file
    void saveCustomers() {
        ofstream file("customers.txt");
        for (const auto& customer : customers) {
            file << customer.serialize() << endl;
        }
        file.close();
        cout << "Customer data saved.\n";
    }

    // Add new customer
    void addCustomer(string name, string contact) {
        customers.push_back(Customer(name, contact));
        cout << "Customer " << name << " added successfully.\n";
    }

    // Find customer
    Customer* findCustomer(string name) {
        for (auto& customer : customers) {
            if (customer.getName() == name) {
                return &customer;
            }
        }
        return nullptr;
    }

    // Process Sale
    void processSale(string customerName, string category, string subcategory, string name, int qty, string paymentMethod) {
        for (auto& p : categorizedInventory[category][subcategory]) {
            if (p->getName() == name && p->getQuantity() >= qty) {
                double total = qty * p->getPrice();
                p->updateQuantity(p->getQuantity() - qty);
                salesReport[name] += total;

                Customer* customer = findCustomer(customerName);
                if (!customer) {
                    string contact;
                    cout << "New customer detected! Enter contact number: ";
                    cin >> contact;
                    addCustomer(customerName, contact);
                    customer = findCustomer(customerName);
                }
                customer->addPurchase(name, qty);

                cout << "\n--- Receipt ---\n";
                cout << "Customer: " << customerName << "\nProduct: " << name;
                cout << "\nQuantity: " << qty << "\nTotal: $" << total;
                cout << "\nPayment Method: " << paymentMethod << "\n---------------\n";
                saveCustomers();
                return;
            }
        }
        cout << "Sale failed! Insufficient stock or product not found.\n";
    }

    // Display Purchase History
    void showCustomerHistory(string customerName) {
        Customer* customer = findCustomer(customerName);
        if (customer) {
            customer->displayHistory();
        } else {
            cout << "Customer not found!\n";
        }
    }
};

// User Authentication
class User {
protected:
    string username;
    string password;
public:
    User(string u, string p) : username(u), password(p) {}
    virtual bool authenticate(string u, string p) {
        return (username == u && password == p);
    }
    virtual void menu() = 0;
};

class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}
    void menu() override {
        cout << "Admin Menu: 1) Manage Inventory 2) View Reports 3) Exit\n";
    }
};

class Cashier : public User {
public:
    Cashier(string u, string p) : User(u, p) {}
    void menu() override {
        cout << "Cashier Menu: 1) Process Sales 2) View Customer History 3) Exit\n";
    }
};

// Main Function
int main() {
    Store myStore;
    Admin admin("admin", "1234");
    Cashier cashier("cashier", "4321");
    string userType, username, password;

    cout << "Login as (admin/cashier): ";
    cin >> userType;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (userType == "admin" && admin.authenticate(username, password)) {
        cout << "Login Successful as Admin!\n";
        int choice;
        do {
            admin.menu();
            cout << "Enter choice: ";
            cin >> choice;
            if (choice == 1) {
                myStore.displayInventory();
            }
        } while (choice != 3);
    } else if (userType == "cashier" && cashier.authenticate(username, password)) {
        cout << "Login Successful as Cashier!\n";
        int choice;
        do {
            cashier.menu();
            cout << "Enter choice: ";
            cin >> choice;
            if (choice == 1) {
                string customer, category, subcategory, pname, method;
                int qty;
                cout << "Enter customer name: ";
                cin >> customer;
                cout << "Enter category: ";
                cin >> category;
                cout << "Enter subcategory: ";
                cin >> subcategory;
                cout << "Enter product name: ";
                cin >> pname;
                cout << "Enter quantity: ";
                cin >> qty;
                cout << "Enter payment method (Cash/Credit): ";
                cin >> method;
                myStore.processSale(customer, category, subcategory, pname, qty, method);
            } else if (choice == 2) {
                string name;
                cout << "Enter customer name: ";
                cin >> name;
                myStore.showCustomerHistory(name);
            }
        } while (choice != 3);
    }
    return 0;
}
