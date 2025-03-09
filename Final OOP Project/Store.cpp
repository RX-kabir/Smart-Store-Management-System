
#include "Store.h"
#include "Utility.h"


// ✅ Constructor: Load inventory & customers
Store::Store() {
    loadProductsFromFile("food.txt", "Food");
    loadProductsFromFile("electronics.txt", "Electronics");
    loadProductsFromFile("pharmaceuticals.txt", "Pharmaceuticals");
    loadProductsFromFile("comics.txt", "Comics");
    loadCustomers();
}

// ✅ Load Products from File
void Store::loadProductsFromFile(const string& filename, const string& category) {
    ifstream file(filename);
    if (!file) {
        cout << "⚠️ ERROR: Unable to open file: " << filename << endl;
        return;
    }

    string subcategory, barcode, name;
    double price;
    int quantity;

    while (file >> barcode >> subcategory >> name >> price >> quantity) {
        categorizedInventory[category][subcategory].push_back(Product(barcode, name, price, quantity));
    }
    file.close();
}

// ✅ Save Inventory to File
void Store::saveProductsToFile(const string& filename, const string& category) {
    ofstream file(filename);
    if (!file) {
        cout << "Error: Unable to save to " << filename << endl;
        return;
    }

    for (auto& [subcategory, products] : categorizedInventory[category]) {
        for (auto& p : products) {
            file << p.getBarcode() << " " << subcategory << " " << p.getName() << " "
                 << p.getPrice() << " " << p.getQuantity() << endl;
        }
    }
    file.close();
}

// ✅ Load Customers from File
void Store::loadCustomers() {
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

// ✅ Save Customers to File
void Store::saveCustomers() {
    ofstream file("customers.txt");
    for (const auto& customer : customers) {
        file << customer.serialize() << endl;
    }
    file.close();
}

// ✅ Find Customer by Name
Customer* Store::findCustomer(string name) {
    for (auto& customer : customers) {
        if (customer.getName() == name) {
            return &customer;
        }
    }
    return nullptr;
}

// ✅ Find Product by Barcode
Product* Store::findProductByBarcode(string barcode) {
    for (auto& [category, subcategories] : categorizedInventory) {
        for (auto& [subcategory, products] : subcategories) {
            for (auto& p : products) {
                if (p.getBarcode() == barcode) {
                    return &p;
                }
            }
        }
    }
    return nullptr;
}

// ✅ Add Customer
void Store::addCustomer(string name, string contact, double discount) {
    customers.push_back(Customer(name, contact, discount));
    saveCustomers();
}

// ✅ View Inventory
void Store::viewInventory() {
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

// ✅ Manage Inventory
void Store::manageInventory() {
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
void Store::restockAlert() {
    for (auto& [category, subcategories] : categorizedInventory) {
        for (auto& [subcategory, products] : subcategories) {
            for (auto& p : products) {
                if (p.getQuantity() < 5) {
                    cout << "Low stock: " << p.getName() << " (Barcode: " << p.getBarcode() << ") - Only " << p.getQuantity() << " left!\n";
                }
            }
        }
    }
    pause();
    clearScreen();
}

// ✅ Process Sale
void Store::processSale(string customerName, Cart& cart, string paymentMethod) {
      Customer* customer = findCustomer(customerName);
    if (!customer) {
        string contact;
        cout << "New customer detected! Enter contact number: ";
        cin >> contact;
        clearScreen(); // Clear screen after input

        addCustomer(customerName, contact);
        customer = findCustomer(customerName);
    }

    double totalPrice = 0;
    for (const auto& item : cart.getItems()) {
        Product* product = item.first;
        int qty = item.second;

        if (product->getQuantity() < qty) {
            cout << "Sale failed! Insufficient stock for " << product->getName() << ".\n";
            return;
        }

        double itemTotal = qty * product->getPrice();
        totalPrice += itemTotal;
        product->updateQuantity(product->getQuantity() - qty);
        customer->addPurchase(product->getName(), qty);
        customer->addLoyaltyPoints(qty * 10); // Add 10 points per item purchased
    }

    double discountAmount = (customer->getDiscount() / 100) * totalPrice;
    totalPrice -= discountAmount;

    // Add transaction to sales history
    transactions.push_back({"Cart Purchase", totalPrice});

    cout << "\n--- Receipt ---\n";
    cout << "Customer: " << customerName << "\n";
    for (const auto& item : cart.getItems()) {
        cout << item.first->getName() << " x " << item.second << " @ $" << item.first->getPrice() << " = $" << item.second * item.first->getPrice() << endl;
    }
    cout << "Total Price: $" << totalPrice << "\n";
    cout << "Discount Applied: $" << discountAmount << "\n";
    cout << "Payment Method: " << paymentMethod << "\n";
    cout << "-------------------------\n";
    pause(); // Wait for user to press Enter
    clearScreen(); // Clear screen after processing sale

    cart.clearCart(); // Clear the cart after the sale
}

// ✅ View Sales History
void Store::viewSalesHistory() {
    cout << "\n--- Sales History ---\n";
    for (const auto& transaction : transactions) {
        cout << "Product: " << transaction.first << ", Total: $" << transaction.second << endl;
    }
    pause();
    clearScreen();
}

// ✅ Process Refund
void Store::processRefund(const string& barcode, int quantity) {
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

// ✅ View Customer Profile
void Store::viewCustomerProfile(string customerName) {
    Customer* customer = findCustomer(customerName);
    if (customer) {
        customer->displayProfile();
    } else {
        cout << "Customer not found!\n";
    }
    pause();
    clearScreen();
}

// ✅ Edit Customer Contact
void Store::editCustomerContact(string customerName) {
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

// ✅ Search Product
void Store::searchProduct(const string& query) {
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

// ✅ Generate Graphs


// ✅ Show Graphs in Terminal
void Store::showSalesGraphinTerminal() {
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

void Store::showInventoryGraphinTerminal() {
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

void Store::showCustomerSpendingGraphinTerminal() {
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
void Store::generateSalesTrendGraph() {
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



void Store::generateInventoryLevelsGraph() {
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

void Store::generateCustomerSpendingGraph() {
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

// ✅ Apply Seasonal Discount
void Store::applySeasonalDiscount(const string& category, double discount) {
    for (auto& [subcategory, products] : categorizedInventory[category]) {
            for (auto& p : products) {
                p.setPrice(p.getPrice() * (1 - discount));
            }
        }
        cout << "Seasonal discount applied to " << category << " category!\n";
        pause(); // Wait for user to press Enter
        clearScreen(); // Clear screen after applying discount
    }



// ✅ Customer Login & Registration
Customer* Store::customerLogin() {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;


    for (auto& customer : customers) {
        if (customer.getName() == username && customer.getContact() == password) {
            cout << "Login successful!\n";
            pause();
             clearScreen();
            return &customer;
        }
    }
    cout << "Invalid login credentials!\n";
    return nullptr;
}

void Store::customerRegistration() {
   string name, contact;
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your contact number: ";
    cin >> contact;

    // Check if customer already exists
    if (findCustomer(name)) {
        cout << "Customer already exists!\n";
        return;
    }

    // Add new customer
    addCustomer(name, contact);
    cout << "Registration successful!\n";
}

// ✅ Customer Menu
void Store::customerMenu(Customer* customer) {
    Cart cart; // Create a cart for the customer
    int choice;

    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1) View Profile\n";
        cout << "2) Add Product to Cart\n";
        cout << "3) View Cart\n";
        cout << "4) Place Order (Online)\n";
        cout << "5) View Purchase History\n";
        cout << "6) Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearScreen(); // Clear screen after input

        switch (choice) {
            case 1: {
                customer->displayProfile();
                pause();
                clearScreen();
                break;
            }
            case 2: {
                string barcode;
                int qty;
                cout << "Enter product barcode: ";
                cin >> barcode;
                clearScreen(); // Clear screen after input

                cout << "Enter quantity: ";
                cin >> qty;
                clearScreen(); // Clear screen after input

                Product* product = findProductByBarcode(barcode);
                if (product) {
                    cart.addItem(product, qty);
                    cout << "Product added to cart.\n";
                } else {
                    cout << "Product not found.\n";
                }
                pause();
                clearScreen();
                break;
            }
            case 3: {
                cart.viewCart();
                pause();
                clearScreen();
                break;
            }
            case 4: {
                string paymentMethod, deliveryAddress;
                cout << "Enter payment method (Credit/Debit/PayPal): ";
                cin >> paymentMethod;
                clearScreen(); // Clear screen after input

                cout << "Enter delivery address (leave blank for pickup): ";
                cin.ignore(); // Clear the input buffer
                getline(cin, deliveryAddress);
                clearScreen(); // Clear screen after input

                processOnlineSale(cart, customer, paymentMethod, deliveryAddress);
                pause();
                clearScreen();
                break;
            }
            case 5: {
                customer->displayProfile();
                pause();
                clearScreen();
                break;
            }
            case 6: {
                cout << "Exiting Customer Menu.\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please try again.\n";
                pause();
                clearScreen();
            }
        }
    } while (choice != 6);
}

// ✅ Process Online Sale
void Store::processOnlineSale(Cart& cart, Customer* customer, const string& paymentMethod, const string& deliveryAddress) {
    double totalPrice = 0;

    // Check stock availability for all items in the cart
    for (const auto& item : cart.getItems()) {
        Product* product = item.first;
        int qty = item.second;

        if (product->getQuantity() < qty) {
            cout << "Order failed! Insufficient stock for " << product->getName() << ".\n";
            return;
        }

        double itemTotal = qty * product->getPrice();
        totalPrice += itemTotal;
    }

    // Apply customer discount
    double discountAmount = (customer->getDiscount() / 100) * totalPrice;
    totalPrice -= discountAmount;

    // Add delivery fee (if applicable)
    double deliveryFee = 0.0;
    if (!deliveryAddress.empty()) {
        deliveryFee = 5.0; // Example delivery fee
        totalPrice += deliveryFee;
    }

    // Process payment (simulated)
    cout << "Processing payment via " << paymentMethod << "...\n";
    cout << "Payment successful!\n";

    // Update inventory and customer purchase history
    for (const auto& item : cart.getItems()) {
        Product* product = item.first;
        int qty = item.second;

        product->updateQuantity(product->getQuantity() - qty);
        customer->addPurchase(product->getName(), qty);
        customer->addLoyaltyPoints(qty * 10); // Add loyalty points
    }

    // Add transaction to sales history
    transactions.push_back({"Online Purchase", totalPrice});

    // Generate order confirmation
    cout << "\n--- Order Confirmation ---\n";
    cout << "Customer: " << customer->getName() << "\n";
    for (const auto& item : cart.getItems()) {
        cout << item.first->getName() << " x " << item.second << " @ $" << item.first->getPrice() << " = $" << item.second * item.first->getPrice() << endl;
    }
    if (!deliveryAddress.empty()) {
        cout << "Delivery Fee: $" << deliveryFee << "\n";
        cout << "Delivery Address: " << deliveryAddress << "\n";
    }
    cout << "Total Price: $" << totalPrice << "\n";
    cout << "Discount Applied: $" << discountAmount << "\n";
    cout << "Payment Method: " << paymentMethod << "\n";
    cout << "-------------------------\n";

    // Clear the cart after the sale
    cart.clearCart();
}
