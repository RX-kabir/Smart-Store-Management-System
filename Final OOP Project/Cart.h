
#ifndef CART_H
#define CART_H

#include <iostream>
#include <vector>
#include "Product.h"  // Required for Product class

using namespace std;

class Cart {
private:
    vector<pair<Product*, int>> items; // Stores products and their quantities
public:
    // Add a product to the cart
    void addItem(Product* product, int quantity);

    // Remove a product from the cart
    void removeItem(const string& barcode);

    // View the cart contents
    void viewCart() const;

    // Clear the cart
    void clearCart();

    // Get items in the cart
    const vector<pair<Product*, int>>& getItems() const;
};

#endif // CART_H
