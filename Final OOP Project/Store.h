
#ifndef STORE_H
#define STORE_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include "Product.h"
#include "Customer.h"
#include "Cart.h"
#include "Utility.h" // For pause() and clearScreen()

using namespace std;

class Store {
private:
    map<string, map<string, vector<Product>>> categorizedInventory;
    vector<Customer> customers;
    vector<pair<string, double>> transactions; // Sales history

public:
    // Constructor
    Store();

    // Product Management
    void loadProductsFromFile(const string& filename, const string& category);
    void saveProductsToFile(const string& filename, const string& category);
    Product* findProductByBarcode(string barcode);

    // Customer Management
    void loadCustomers();
    void saveCustomers();
    Customer* findCustomer(string name);
    void addCustomer(string name, string contact, double discount = 0.0);

    // Inventory Management
    void viewInventory();
    void manageInventory();
    void restockAlert();

    // Sales Processing
    void processSale(string customerName, Cart& cart, string paymentMethod);
    void viewSalesHistory();
    void processRefund(const string& barcode, int quantity);

    // Customer Interactions
    void viewCustomerProfile(string customerName);
    void editCustomerContact(string customerName);
    void searchProduct(const string& query);

    // Customer Login & Registration
    Customer* customerLogin();
    void customerRegistration();
    void customerMenu(Customer* customer);
    void processOnlineSale(Cart& cart, Customer* customer, const string& paymentMethod, const string& deliveryAddress);

    // Graphs & Reports
    void generateSalesTrendGraph();
    void generateInventoryLevelsGraph();
    void generateCustomerSpendingGraph();
    void applySeasonalDiscount(const string& category, double discount);

    // Open Graphs in Terminal
    void showSalesGraphinTerminal();
    void showInventoryGraphinTerminal();
    void showCustomerSpendingGraphinTerminal();
};

#endif // STORE_H
