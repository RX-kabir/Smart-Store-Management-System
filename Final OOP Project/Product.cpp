
#include "Product.h"

// Constructor
Product::Product(string b, string n, double p, int q)
    : barcode(b), name(n), price(p), quantity(q) {}

// Display product details
void Product::display() const {
    cout << left << setw(10) << barcode << setw(15) << name
         << setw(10) << price << setw(10) << quantity << endl;
}

// Getters
string Product::getName() const { return name; }
string Product::getBarcode() const { return barcode; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }

// Setters
void Product::updateQuantity(int q) { quantity = q; }
void Product::setPrice(double p) { price = p; } // Added for seasonal discounts
