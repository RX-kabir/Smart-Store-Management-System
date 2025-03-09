
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <vector>
#include <sstream>  // For serialization
#include <fstream>  // For file handling

using namespace std;

class Customer {
private:
    string name;
    string contact;
    double discountRate;
    int loyaltyPoints; // For loyalty program
    vector<pair<string, int>> purchaseHistory;

public:
    // Constructor
    Customer(string n, string c, double d = 0.0);

    // Purchase History Management
    void addPurchase(string product, int quantity);
    const vector<pair<string, int>>& getPurchaseHistory() const; // ✅ No changes needed!

    // Display Customer Profile
    void displayProfile() const;

    // Contact & Discount Management
    void updateContact(string newContact);
    void setDiscount(double discount);
    double getDiscount() const;
    string getName() const;
    string getContact() const;

    // Loyalty Program
    void addLoyaltyPoints(int points);
    void redeemPoints(int points);

    // Serialization & Deserialization
    string serialize() const;
    static Customer deserialize(const string& line);
};

#endif // CUSTOMER_H
