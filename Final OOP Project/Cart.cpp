
#include "Cart.h"
#include "Utility.h"

// Add a product to the cart
void Cart::addItem(Product* product, int quantity) {
    if (product) {
        items.push_back({product, quantity});
        cout << "Product added to cart: " << product->getName()
             << " (Barcode: " << product->getBarcode() << ")" << endl;
    } else {
        cout << "Cannot add product to cart: Product is null!" << endl;
    }
}

// Remove a product from the cart
void Cart::removeItem(const string& barcode) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->first->getBarcode() == barcode) {
            items.erase(it);
            cout << "Product removed from cart.\n";
            return;
        }
    }
    cout << "Product not found in cart.\n";
}

// View the cart
void Cart::viewCart() const {
    if (items.empty()) {
        cout << "Your cart is empty.\n";
        return;
    }
    cout << "\n--- Your Cart ---\n";
    double total = 0;
    for (const auto& item : items) {
        cout << item.first->getName() << " x " << item.second
             << " @ $" << item.first->getPrice() << " = $"
             << item.second * item.first->getPrice() << endl;
        total += item.second * item.first->getPrice();
    }
    cout << "Total: $" << total << "\n";
    cout << "-----------------\n";
}

// Clear the cart
void Cart::clearCart() {
    items.clear();
}

// Get items in the cart
const vector<pair<Product*, int>>& Cart::getItems() const {
    return items;
}
