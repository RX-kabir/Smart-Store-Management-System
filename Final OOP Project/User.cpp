
#include "User.h"
#include "Utility.h"


// ✅ User Class Implementation
User::User(string u, string p) : username(u), password(p) {}

// Authenticate user
bool User::authenticate(string u, string p) {
    return (username == u && password == p);
}

// ✅ Admin Class Implementation
Admin::Admin(string u, string p) : User(u, p) {}

// Load Admin Users from file
unordered_map<string, string> Admin::loadUsers(const string& filename) {
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

// Admin Menu
void Admin::menu() {
    cout << "Admin Menu: \n";
    cout << "1) Manage Inventory \n";
    cout << "2) View Inventory \n";
    cout << "3) Restock Alert \n";
    cout << "4) Apply Seasonal Discount \n"; // Added new feature
    cout << "5) Generate Sales Trend Graph \n"; // New option
    cout << "6) Generate Inventory Levels Graph \n"; // New option
    cout << "7) Exit\n";
}

// ✅ Cashier Class Implementation
Cashier::Cashier(string u, string p) : User(u, p) {}

// Load Cashier Users from file
unordered_map<string, string> Cashier::loadUsers(const string& filename) {
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

// Cashier Menu
void Cashier::menu() {
    cout << "Cashier Menu: \n";
    cout << "1) Add Product to Cart \n";
    cout << "2) Remove Product from Cart \n";
    cout << "3) View Cart \n";
    cout << "4) Process Sale \n";
    cout << "5) View Customer Profile \n";
    cout << "6) Edit Customer Contact \n";
    cout << "7) Search Product \n";
    cout << "8) View Sales History \n";
    cout << "9) Process Refund \n";
    cout << "10) Generate Customer Spending Graph \n";
    cout << "11) Exit\n";
}
