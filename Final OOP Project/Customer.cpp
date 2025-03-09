
#include "Customer.h"
#include "Utility.h"


// Constructor
Customer::Customer(string n, string c, double d) : name(n), contact(c), discountRate(d), loyaltyPoints(0) {}

// Add a purchase to the history
void Customer::addPurchase(string product, int quantity) {
    purchaseHistory.push_back({product, quantity});
}

// Get purchase history (✅ Now Included)
const vector<pair<string, int>>& Customer::getPurchaseHistory() const {
    return purchaseHistory;
}

// Display customer profile
void Customer::displayProfile() const {
    cout << "\n--- Customer Profile ---\n";
    cout << "Name: " << name << "\n";
    cout << "Contact: " << contact << "\n";
    cout << "Discount Rate: " << discountRate << "%\n";
    cout << "Loyalty Points: " << loyaltyPoints << "\n"; // Loyalty program
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

// Update contact information
void Customer::updateContact(string newContact) {
    contact = newContact;
}

// Set and get discount rate
void Customer::setDiscount(double discount) {
    discountRate = discount;
}
double Customer::getDiscount() const {
    return discountRate;
}

// Get customer name and contact
string Customer::getName() const {
    return name;
}
string Customer::getContact() const {
    return contact;
}

// Add and redeem loyalty points
void Customer::addLoyaltyPoints(int points) {
    loyaltyPoints += points;
}
void Customer::redeemPoints(int points) {
    if (loyaltyPoints >= points) {
        loyaltyPoints -= points;
        cout << "Points redeemed successfully!\n";
    } else {
        cout << "Insufficient points!\n";
    }
}

// Serialize customer data to a string
string Customer::serialize() const {
    stringstream ss;
    ss << name << " " << contact << " " << discountRate << " " << loyaltyPoints;
    for (const auto& item : purchaseHistory) {
        ss << " " << item.first << " " << item.second;
    }
    return ss.str();
}

// Deserialize customer data from a string
Customer Customer::deserialize(const string& line) {
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
