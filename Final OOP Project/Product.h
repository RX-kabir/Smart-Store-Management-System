
#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <iomanip>

using namespace std;

class Product {
protected:
    string name;
    string barcode;  // Unique product identifier
    double price;
    int quantity;
public:
    // Constructor
    Product(string b, string n, double p, int q);

    // Display product details
    void display() const;

    // Getters
    string getName() const;
    string getBarcode() const;
    double getPrice() const;
    int getQuantity() const;

    // Setters
    void updateQuantity(int q);
    void setPrice(double p); // Added for seasonal discounts
};

#endif // PRODUCT_H
