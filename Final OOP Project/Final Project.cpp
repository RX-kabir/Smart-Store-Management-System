#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include<unordered_map>
#include <cstdlib> // for system()
#include <chrono>



using namespace std;

// Clear screen function
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // For Windows
    #else
        system("clear"); // For Linux and macOS
    #endif
}

// Pause function
void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(); // Ignore any leftover newline characters
    cin.get();    // Wait for Enter key
}



// Function to generate a timestamp
string getTimestamp() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%Y%m%d_%H%M%S"); // Format: YYYYMMDD_HHMMSS
    return ss.str();
}



// ✅ Product Class
class Product {
protected:
    string name;
    string barcode;  // Unique product identifier
    double price;
    int quantity;
public:
    Product(string b, string n, double p, int q) : barcode(b), name(n), price(p), quantity(q) {}

    void display() const {
        cout << left << setw(10) << barcode << setw(15) << name << setw(10) << price << setw(10) << quantity << endl;
    }

    string getName() const { return name; }
    string getBarcode() const { return barcode; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    void updateQuantity(int q) { quantity = q; }
    void setPrice(double p) { price = p; } // Added for seasonal discounts
};

// ✅ Customer Class
class Customer {
private:
    string name;
    string contact;
    double discountRate;
    int loyaltyPoints; // Added for loyalty program
    vector<pair<string, int>> purchaseHistory;

public:
    Customer(string n, string c, double d = 0.0) : name(n), contact(c), discountRate(d), loyaltyPoints(0) {}

    void addPurchase(string product, int quantity) {
        purchaseHistory.push_back({product, quantity});
    }

    void displayProfile() const {
        cout << "\n--- Customer Profile ---\n";
        cout << "Name: " << name << "\n";
        cout << "Contact: " << contact << "\n";
        cout << "Discount Rate: " << discountRate << "%\n";
        cout << "Loyalty Points: " << loyaltyPoints << "\n"; // Added for loyalty program
        cout << "--- Purchase History ---\n";
        if (purchaseHistory.empty()) {
            cout << "No purchases found.\n";
        } else {
            for (const auto& item : purchaseHistory) {
                cout << "- " << item.first << " x" << item.second << endl;
            }
        }
        cout << "-------------------------\n";
    }

    void updateContact(string newContact) { contact = newContact; }
    void setDiscount(double discount) { discountRate = discount; }
    double getDiscount() const { return discountRate; }
    string getName() const { return name; }
    string getContact() const { return contact; }

    // Loyalty Program
    void addLoyaltyPoints(int points) { loyaltyPoints += points; }
    void redeemPoints(int points) {
        if (loyaltyPoints >= points) {
            loyaltyPoints -= points;
            cout << "Points redeemed successfully!\n";
        } else {
            cout << "Insufficient points!\n";
        }
    }

    string serialize() const {
        stringstream ss;
        ss << name << " " << contact << " " << discountRate << " " << loyaltyPoints;
        for (const auto& item : purchaseHistory) {
            ss << " " << item.first << " " << item.second;
        }
        return ss.str();
    }

    static Customer deserialize(const string& line) {
        stringstream ss(line);
        string name, contact, product;
        double discount;
        int quantity, loyaltyPoints;
        ss >> name >> contact >> discount >> loyaltyPoints;
        Customer customer(name, contact, discount);
        customer.loyaltyPoints = loyaltyPoints;
        while (ss >> product >> quantity) {
            customer.addPurchase(product, quantity);
        }
        return customer;
    }
    const vector<pair<string, int>>& getPurchaseHistory() const {
    return purchaseHistory;
}

};

// ✅ User Authentication Base Class
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

// ✅ Admin Class
class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}

    static unordered_map<string, string> loadUsers(const string& filename) {
        unordered_map<string, string> users;
        ifstream file(filename);

        if (!file) {
            cout << "⚠️ ERROR: Unable to open file: " << filename << endl;
            return users;
        }

        string username, password;
        while (file >> username >> password) {
            users[username] = password;
        }

        file.close();
        return users;
    }

    void menu() override {
        cout << "Admin Menu: \n";
        cout << "1) Manage Inventory \n";
        cout << "2) View Inventory \n";
        cout << "3) Restock Alert \n";
        cout << "4) Apply Seasonal Discount \n"; // Added new feature
        cout << "5) Generate Sales Trend Graph \n"; // New option
        cout << "6) Generate Inventory Levels Graph \n"; // New option
        cout << "7) Exit\n";
    }
};

// ✅ Cashier Class
class Cashier : public User {
public:
    Cashier(string u, string p) : User(u, p) {}

     static unordered_map<string, string> loadUsers(const string& filename) {
        unordered_map<string, string> users;
        ifstream file(filename);

        if (!file) {
            cout << "⚠️ ERROR: Unable to open file: " << filename << endl;
            return users;
        }

        string username, password;
        while (file >> username >> password) {
            users[username] = password;
        }

        file.close();
        return users;
    }

    void menu() override {
        cout << "Cashier Menu: \n";
        cout << "1) Process Sale \n";
        cout << "2) View Customer Profile \n";
        cout << "3) Edit Customer Contact \n";
        cout << "4) Search Product \n"; // Added new feature
        cout << "5) View Sales History \n"; // Added new feature
        cout << "6) Process Refund \n"; // Added new feature
        cout << "7) Generate Customer Spending Graph \n"; // New option
        cout << "8) Exit\n";
    }
};

// ✅ Store Class
class Store {
private:
    map<string, map<string, vector<Product>>> categorizedInventory;
    vector<Customer> customers;
    vector<pair<string, double>> transactions; // Added for sales history

public:
    Store() {
        loadProductsFromFile("food.txt", "Food");
        loadProductsFromFile("electronics.txt", "Electronics");
        loadProductsFromFile("pharmaceuticals.txt", "Pharmaceuticals");
        loadProductsFromFile("comics.txt", "Comics");
        loadProductsFromFile("stationery.txt", "Comics");

        loadCustomers();
    }

    // ✅ Load Products from File
    void loadProductsFromFile(const string& filename, const string& category) {
        ifstream file(filename);
        if (!file) {
            cout << "⚠️ ERROR: Unable to open file: " << filename << endl;
            return;
        }

        string subcategory, name;
        double price;
        int quantity;

        while (file >> subcategory >> name >> price >> quantity) {
            string barcode = name; // Using name as barcode for simplicity
            categorizedInventory[category][subcategory].push_back(Product(barcode, name, price, quantity));
        }

        file.close();
    }

    // ✅ Save Inventory to File
    void saveProductsToFile(const string& filename, const string& category) {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Unable to save to " << filename << endl;
            return;
        }

        for (auto& [subcategory, products] : categorizedInventory[category]) {
            for (auto& p : products) {
                file << p.getBarcode() << " " << subcategory << " " << p.getName() << " " << p.getPrice() << " " << p.getQuantity() << endl;
            }
        }

        file.close();
        cout << "Inventory saved to " << filename << endl;
    }

    // ✅ Load Customers
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
    }

    // ✅ Save Customers
    void saveCustomers() {
        ofstream file("customers.txt");
        for (const auto& customer : customers) {
            file << customer.serialize() << endl;
        }
        file.close();
    }

    // ✅ Find Customer
    Customer* findCustomer(string name) {
        for (auto& customer : customers) {
            if (customer.getName() == name) {
                return &customer;
            }
        }
        return nullptr;
    }

    // ✅ Find Product by Barcode
    Product* findProductByBarcode(string barcode) {
        for (auto& [category, subcategories] : categorizedInventory) {
            for (auto& [subcategory, products] : subcategories) {
                for (auto& p : products) {
                    if (p.getBarcode() == barcode) return &p;
                }
            }
        }
        return nullptr;
    }

    // ✅ Add Customer
    void addCustomer(string name, string contact, double discount = 0.0) {
        customers.push_back(Customer(name, contact, discount));
        saveCustomers();
    }

    // ✅ View Inventory
    void viewInventory() {
        cout << "\n--- Store Inventory ---\n";

        if (categorizedInventory.empty()) {
            cout << "⚠️ Inventory is EMPTY!\n";
            return;
        }

        for (auto& [category, subcategories] : categorizedInventory) {
            cout << "\n[" << category << "]\n";
            for (auto& [subcategory, products] : subcategories) {
                cout << " - " << subcategory << " - \n";
                cout << left << setw(10) << "Barcode" << setw(15) << "Name" << setw(10) << "Price" << setw(10) << "Quantity\n";
                cout << "------------------------------------\n";
                if (products.empty()) {
                    cout << "⚠️ No products in this category!\n";
                }
                for (const auto& p : products) {
                    p.display();
                }
            }
        }
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after viewing
    }

    // ✅ Manage Inventory (Add, Remove, Update)
    void manageInventory() {
        int option;
        do {
            cout << "\n--- Inventory Management ---\n";
            cout << "1) Add Product\n";
            cout << "2) Remove Product\n";
            cout << "3) Update Product Quantity\n";
            cout << "4) Exit Inventory Management\n";
            cout << "Enter your choice: ";
            cin >> option;
            clearScreen(); // Clear screen after input

            if (option == 1) {  // Add Product
                string barcode, name, category, subcategory;
                double price;
                int quantity;

                cout << "Enter Product Barcode: ";
                cin >> barcode;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Name: ";
                cin >> name;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Category (Food, Electronics, Pharmaceuticals, Comics): ";
                cin >> category;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Subcategory: ";
                cin >> subcategory;
                clearScreen(); // Clear screen after input

                cout << "Enter Price: ";
                cin >> price;
                clearScreen(); // Clear screen after input

                cout << "Enter Quantity: ";
                cin >> quantity;
                clearScreen(); // Clear screen after input

                categorizedInventory[category][subcategory].push_back(Product(barcode, name, price, quantity));
                saveProductsToFile(category + ".txt", category);

                cout << "Product added successfully!\n";
                pause(); // Wait for user to press Enter
                clearScreen(); // Clear screen after adding product
            }
            else if (option == 2) {  // Remove Product
                string name, category, subcategory;
                cout << "Enter Product Category: ";
                cin >> category;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Subcategory: ";
                cin >> subcategory;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Name to Remove: ";
                cin >> name;
                clearScreen(); // Clear screen after input

                auto& products = categorizedInventory[category][subcategory];
                for (auto it = products.begin(); it != products.end(); ++it) {
                    if (it->getName() == name) {
                        products.erase(it);
                        saveProductsToFile(category + ".txt", category);
                        cout << "Product removed successfully!\n";
                        break;
                    }
                }
                pause(); // Wait for user to press Enter
                clearScreen(); // Clear screen after removing product
            }
            else if (option == 3) {  // Update Product Quantity
                string name, category, subcategory;
                int newQuantity;

                cout << "Enter Product Category: ";
                cin >> category;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Subcategory: ";
                cin >> subcategory;
                clearScreen(); // Clear screen after input

                cout << "Enter Product Name: ";
                cin >> name;
                clearScreen(); // Clear screen after input

                cout << "Enter New Quantity: ";
                cin >> newQuantity;
                clearScreen(); // Clear screen after input

                auto& products = categorizedInventory[category][subcategory];
                for (auto& p : products) {
                    if (p.getName() == name) {
                        p.updateQuantity(newQuantity);
                        saveProductsToFile(category + ".txt", category);
                        cout << "Product quantity updated successfully!\n";
                    }
                }
                pause(); // Wait for user to press Enter
                clearScreen(); // Clear screen after updating quantity
            }
        } while (option != 4);
    }

    // ✅ Restock Alert
    void restockAlert() {
        cout << "\n--- Restock Alert ---\n";
        for (auto& [category, subcategories] : categorizedInventory) {
            for (auto& [subcategory, products] : subcategories) {
                for (auto& p : products) {
                    if (p.getQuantity() < 5) {
                        cout << "Low stock: " << p.getName() << " (Barcode: " << p.getBarcode() << ") - Only " << p.getQuantity() << " left!\n";
                    }
                }
            }
        }
        cout << "-------------------------\n";
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after restock alert
    }

    // ✅ Process Sale
    void processSale(string customerName, string barcode, int qty, string paymentMethod) {
        Product* product = findProductByBarcode(barcode);
        if (!product || product->getQuantity() < qty) {
            cout << "Sale failed! Product not found or insufficient stock.\n";
            return;
        }

        Customer* customer = findCustomer(customerName);
        if (!customer) {
            string contact;
            cout << "New customer detected! Enter contact number: ";
            cin >> contact;
            clearScreen(); // Clear screen after input

            addCustomer(customerName, contact);
            customer = findCustomer(customerName);
        }

        double totalPrice = qty * product->getPrice();
        double discountAmount = (customer->getDiscount() / 100) * totalPrice;
        totalPrice -= discountAmount;

        product->updateQuantity(product->getQuantity() - qty);
        customer->addPurchase(product->getName(), qty);
        customer->addLoyaltyPoints(qty * 10); // Add 10 points per item purchased

        // Add transaction to sales history
        transactions.push_back({product->getName(), totalPrice});

        cout << "\n--- Receipt ---\n";
        cout << "Customer: " << customerName << "\n";
        cout << "Product: " << product->getName() << " (Barcode: " << barcode << ")\n";
        cout << "Quantity: " << qty << "\n";
        cout << "Total Price: $" << totalPrice << "\n";
        cout << "Discount Applied: $" << discountAmount << "\n";
        cout << "Payment Method: " << paymentMethod << "\n";
        cout << "-------------------------\n";
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after processing sale
    }

    // ✅ View Customer Profile
    void viewCustomerProfile(string customerName) {
        Customer* customer = findCustomer(customerName);
        if (customer) {
            customer->displayProfile();
        } else {
            cout << "Customer not found!\n";
        }
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after viewing profile
    }

    // ✅ Edit Customer Contact
    void editCustomerContact(string customerName) {
        Customer* customer = findCustomer(customerName);
        if (customer) {
            string newContact;
            cout << "Enter new contact number: ";
            cin >> newContact;
            clearScreen(); // Clear screen after input

            customer->updateContact(newContact);
            saveCustomers();
            cout << "Customer contact updated successfully!\n";
        } else {
            cout << "Customer not found!\n";
        }
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after editing contact
    }

    // ✅ Search Products
    void searchProduct(const string& query) {
        cout << "\n--- Search Results ---\n";
        for (auto& [category, subcategories] : categorizedInventory) {
            for (auto& [subcategory, products] : subcategories) {
                for (auto& p : products) {
                    if (p.getName().find(query) != string::npos || p.getBarcode() == query) {
                        p.display();
                    }
                }
            }
        }
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after search
    }

    // ✅ View Sales History
    void viewSalesHistory() {
        cout << "\n--- Sales History ---\n";
        for (const auto& transaction : transactions) {
            cout << "Product: " << transaction.first << ", Total: $" << transaction.second << endl;
        }
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after viewing sales history
    }

    // ✅ Process Refund
    void processRefund(const string& barcode, int quantity) {
        Product* product = findProductByBarcode(barcode);
        if (product) {
            product->updateQuantity(product->getQuantity() + quantity);
            cout << "Refund processed successfully!\n";
        } else {
            cout << "Product not found!\n";
        }
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after processing refund
    }

 void showSalesGraphinTerminal() {
    #ifdef _WIN32
        system("start sales_trend.png");
    #elif __APPLE__
        system("open sales_trend.png");
    #else
        system("xdg-open sales_trend.png");
    #endif

    cout << "\nPress Enter to close the graph and continue...";
    cin.ignore();
    cin.get(); // Wait for Enter key

    clearScreen(); // Clear the terminal after pressing Enter
}

 void showInventoryGraphinTerminal() {
    #ifdef _WIN32
        system("start inventory_levels.png");
    #elif __APPLE__
        system("open inventory_levels.png");
    #else
        system("xdg-open inventory_levels.png");
    #endif

    cout << "\nPress Enter to close the graph and continue...";
    cin.ignore();
    cin.get(); // Wait for Enter key

    clearScreen(); // Clear the terminal after pressing Enter
}

 void showCustomerSpendingGraphinTerminal() {
    #ifdef _WIN32
        system("start customer_spending.png");
    #elif __APPLE__
        system("open customer_spending.png");
    #else
        system("xdg-open customer_spending.png");
    #endif

    cout << "\nPress Enter to close the graph and continue...";
    cin.ignore();
    cin.get(); // Wait for Enter key

    clearScreen(); // Clear the terminal after pressing Enter
}


    // ✅ Apply Seasonal Discount
   void generateSalesTrendGraph() {
    ofstream dataFile("sales_data.txt", ios::app);
    if (transactions.empty()) {
        dataFile << "No_Sales 0" << endl;
    } else {

        for (const auto& transaction : transactions) {
            dataFile << transaction.first << " " << transaction.second << endl;
        }
    }
    dataFile.close();

    ofstream gnuplotScript("sales_trend.gnu");
    gnuplotScript << "set terminal png size 800,600\n";
    gnuplotScript << "set output 'sales_trend.png'\n";
    gnuplotScript << "set title 'Sales Trend'\n";
    gnuplotScript << "set xlabel 'Product'\n";
    gnuplotScript << "set ylabel 'Total Sales ($)'\n";
    gnuplotScript << "plot 'sales_data.txt' using 2:xtic(1) with linespoints title 'Sales'\n";
    gnuplotScript.close();

    system("gnuplot sales_trend.gnu");
    cout << "Sales trend graph saved as 'sales_trend.png'.\n";

    // Show graph and wait for Enter
    showSalesGraphinTerminal();
}



    // ✅ Generate Inventory Levels Graph
    void generateInventoryLevelsGraph() {
        ofstream dataFile("inventory_data.txt");
        for (const auto& [category, subcategories] : categorizedInventory) {
            for (const auto& [subcategory, products] : subcategories) {
                for (const auto& p : products) {
                    dataFile << p.getName() << " " << p.getQuantity() << endl;
                }
            }
        }
        dataFile.close();

        ofstream gnuplotScript("inventory_levels.gnu");
    gnuplotScript << "set terminal png size 1500,600\n"; // Set the size of the output image
    gnuplotScript << "set output 'inventory_levels.png'\n"; // Set the output file name
    gnuplotScript << "set title 'Inventory Levels'\n"; // Set the graph title
    gnuplotScript << "set xlabel 'Product'\n"; // Set the x-axis label
    gnuplotScript << "set ylabel 'Quantity'\n"; // Set the y-axis label
    gnuplotScript << "set xtics rotate by -45\n"; // Rotate x-axis labels by 45 degrees
    gnuplotScript << "set xtics font \",8\"\n"; // Set a smaller font size for x-axis labels
    gnuplotScript << "plot 'inventory_data.txt' using 2:xtic(1) with boxes title 'Inventory'\n";
        gnuplotScript.close();

        system("gnuplot inventory_levels.gnu");
        cout << "Inventory levels graph saved as 'inventory_levels.png'.\n";

        showInventoryGraphinTerminal();

    }

    // ✅ Generate Customer Spending Graph
void generateCustomerSpendingGraph() {
    ofstream dataFile("customer_spending.txt"); // Open in append mode

    if (customers.empty()) {
        dataFile << "No_Customers 0" << endl; // Write placeholder if no customers
    } else {
        for (const auto& customer : customers) {
            double totalSpending = 0;
            for (const auto& purchase : customer.getPurchaseHistory()) {
                totalSpending += purchase.second; // Assuming purchase.second is the amount spent
            }
            dataFile << customer.getName() << " " << totalSpending << endl;
        }
    }

    dataFile.close();

    ofstream gnuplotScript("customer_spending.gnu");
    gnuplotScript << "set terminal png size 800,600\n";
    gnuplotScript << "set output 'customer_spending.png'\n";
    gnuplotScript << "set title 'Customer Spending'\n";
    gnuplotScript << "set xlabel 'Customer'\n";
    gnuplotScript << "set ylabel 'Total Spending ($)'\n";
    gnuplotScript << "plot 'customer_spending.txt' using 2:xtic(1) with boxes title 'Spending'\n";
    gnuplotScript.close();

    system("gnuplot customer_spending.gnu");
    cout << "Customer spending graph saved as 'customer_spending.png'.\n";

    // Show graph and wait for Enter
    showCustomerSpendingGraphinTerminal();
}


void applySeasonalDiscount(const string& category, double discount) {
        for (auto& [subcategory, products] : categorizedInventory[category]) {
            for (auto& p : products) {
                p.setPrice(p.getPrice() * (1 - discount));
            }
        }
        cout << "Seasonal discount applied to " << category << " category!\n";
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after applying discount
    }

};



// ✅ Main Function
int main() {
    Store myStore;

    // Load admin and cashier users
    unordered_map<string, string> adminUsers = Admin::loadUsers("admin.txt");
    unordered_map<string, string> cashierUsers = Cashier::loadUsers("cashier.txt");

    // Simulate a login
    string userType, username, password;
    cout << "Login as (admin/cashier): ";
    cin >> userType;
    clearScreen();
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (userType == "admin" && adminUsers.find(username) != adminUsers.end() && adminUsers[username] == password) {
        cout << "Admin login successful!\n";
        pause();
        clearScreen();
        Admin admin(username, password); // Create Admin object
        int choice;
        do {
            admin.menu();
            cout << "Enter choice: ";
            cin >> choice;
            clearScreen(); // Clear screen after input

            switch (choice) {
                case 1:
                    myStore.manageInventory();
                    break;
                case 2:
                    myStore.viewInventory();
                    break;
                case 3:
                    myStore.restockAlert();
                    break;
                case 4: {
                    string category;
                    double discount;
                    cout << "Enter category to apply discount: ";
                    cin >> category;
                    clearScreen(); // Clear screen after input

                    cout << "Enter discount rate (e.g., 0.1 for 10%): ";
                    cin >> discount;
                    clearScreen(); // Clear screen after input

                    myStore.applySeasonalDiscount(category, discount);
                    break;
                }
                case 5:
                    myStore.generateSalesTrendGraph();
                    break;
                case 6:
                    myStore.generateInventoryLevelsGraph();
                    break;
                case 7:
                    cout << "Exiting Admin Menu.\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
                    pause(); // Wait for user to press Enter
                    clearScreen(); // Clear screen after invalid input
            }
        } while (choice != 7);
    }
    else if (userType == "cashier" && cashierUsers.find(username) != cashierUsers.end() && cashierUsers[username] == password) {
        cout << "Cashier login successful!\n";
        pause();
        clearScreen();
        Cashier cashier(username, password); // Create Cashier object
        int choice;
        do {
            cashier.menu();
            cout << "Enter choice: ";
            cin >> choice;
            clearScreen(); // Clear screen after input

            switch (choice) {
                case 1: {
                    string customerName, barcode, paymentMethod;
                    int qty;
                    cout << "Enter customer name: ";
                    cin >> customerName;
                    clearScreen(); // Clear screen after input

                    cout << "Enter product barcode: ";
                    cin >> barcode;
                    clearScreen(); // Clear screen after input

                    cout << "Enter quantity: ";
                    cin >> qty;
                    clearScreen(); // Clear screen after input

                    cout << "Enter payment method (Cash/Credit): ";
                    cin >> paymentMethod;
                    clearScreen(); // Clear screen after input

                    myStore.processSale(customerName, barcode, qty, paymentMethod);
                    break;
                }
                case 2: {
                    string customerName;
                    cout << "Enter customer name: ";
                    cin >> customerName;
                    clearScreen(); // Clear screen after input

                    myStore.viewCustomerProfile(customerName);
                    break;
                }
                case 3: {
                    string customerName;
                    cout << "Enter customer name: ";
                    cin >> customerName;
                    clearScreen(); // Clear screen after input

                    myStore.editCustomerContact(customerName);
                    break;
                }
                case 4: {
                    string query;
                    cout << "Enter product name or barcode to search: ";
                    cin >> query;
                    clearScreen(); // Clear screen after input

                    myStore.searchProduct(query);
                    break;
                }
                case 5:
                    myStore.viewSalesHistory();
                    break;
                case 6: {
                    string barcode;
                    int quantity;
                    cout << "Enter product barcode: ";
                    cin >> barcode;
                    clearScreen(); // Clear screen after input

                    cout << "Enter quantity to refund: ";
                    cin >> quantity;
                    clearScreen(); // Clear screen after input

                    myStore.processRefund(barcode, quantity);
                    break;
                }
                case 7:
                    myStore.generateCustomerSpendingGraph();
                    break;
                case 8:
                    cout << "Exiting Cashier Menu.\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
                    pause(); // Wait for user to press Enter
                    clearScreen(); // Clear screen after invalid input
            }
        } while (choice != 8);
    }
    else {
        cout << "Invalid login credentials!\n";
    }

    return 0;
}
