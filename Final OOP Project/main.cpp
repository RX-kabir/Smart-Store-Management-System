#include "Store.h"
#include "User.h"  // For Admin and Cashier
#include "Utility.h" // For clearScreen() and pause()
#include "Utility.h"


int main() {
    Store myStore;

    // Load admin and cashier users
    unordered_map<string, string> adminUsers = Admin::loadUsers("admin.txt");
    unordered_map<string, string> cashierUsers = Cashier::loadUsers("cashier.txt");

    cout << "\n======================================\n";
    cout << "   Welcome to Our Smart Store System   \n";
    cout << "=======================================\n";

    int choice;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1) Login as Admin\n";
        cout << "2) Login as Cashier\n";
        cout << "3) Login as Customer\n";
        cout << "4) Register as Customer\n";
        cout << "5) Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearScreen();

        switch (choice) {
            case 1: { // Admin Login
                cout << "\n=====================================\n";
                cout << "            Admin Portal             \n";
                cout << "=====================================\n";
                cout << "Authorized personnel only. Please log in.\n";

                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (adminUsers.find(username) != adminUsers.end() && adminUsers[username] == password) {
                    cout << "Admin login successful!\n";
                    pause();
                    clearScreen();
                    Admin admin(username, password);
                    int adminChoice;
                    do {
                        admin.menu();
                        cout << "Enter choice: ";
                        cin >> adminChoice;
                        clearScreen();

                        switch (adminChoice) {
                            case 1: myStore.manageInventory(); break;
                            case 2: myStore.viewInventory(); break;
                            case 3: myStore.restockAlert(); break;
                            case 4: {
                                string category;
                                double discount;
                                cout << "Enter category to apply discount: ";
                                cin >> category;
                                clearScreen();
                                cout << "Enter discount rate (e.g., 0.1 for 10%): ";
                                cin >> discount;
                                clearScreen();
                                myStore.applySeasonalDiscount(category, discount);
                                break;
                            }
                            case 5: myStore.generateSalesTrendGraph(); break;
                            case 6: myStore.generateInventoryLevelsGraph(); break;
                            case 7: cout << "Exiting Admin Menu.\n"; break;
                            default:
                                cout << "Invalid choice! Please try again.\n";
                                pause();
                                clearScreen();
                        }
                    } while (adminChoice != 7);
                } else {
                    cout << "Invalid login credentials!\n";
                    pause();
                    clearScreen();
                }
                break;
            }

            case 2: { // Cashier Login
                cout << "\n=====================================\n";
                cout << "           Cashier Portal            \n";
                cout << "=====================================\n";
                cout << "Log in to start processing transactions.\n";

                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (cashierUsers.find(username) != cashierUsers.end() && cashierUsers[username] == password) {
                    cout << "Cashier login successful!\n";
                    pause();
                    clearScreen();
                    Cashier cashier(username, password);
                    int cashierChoice;
                    Cart cart;

                    do {
                        cashier.menu();
                        cout << "Enter choice: ";
                        cin >> cashierChoice;
                        clearScreen();

                        switch (cashierChoice) {
                            case 1: { // Add Product to Cart
                                string barcode;
                                int qty;
                                cout << "Enter product barcode: ";
                                cin >> barcode;
                                clearScreen();
                                cout << "Enter quantity: ";
                                cin >> qty;
                                clearScreen();
                                Product* product = myStore.findProductByBarcode(barcode);
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
                            case 2: { // Remove Product from Cart
                                string barcode;
                                cout << "Enter product barcode to remove: ";
                                cin >> barcode;
                                clearScreen();
                                cart.removeItem(barcode);
                                pause();
                                clearScreen();
                                break;
                            }
                            case 3: { // View Cart
                                cart.viewCart();
                                pause();
                                clearScreen();
                                break;
                            }
                            case 4: { // Process Sale
                                string customerName, paymentMethod;
                                cout << "Enter customer name: ";
                                cin >> customerName;
                                clearScreen();
                                cout << "Enter payment method (Cash/Credit): ";
                                cin >> paymentMethod;
                                clearScreen();
                                myStore.processSale(customerName, cart, paymentMethod);
                                break;
                            }
                            case 5: { // View Customer Profile
                                string customerName;
                                cout << "Enter customer name: ";
                                cin >> customerName;
                                clearScreen();
                                myStore.viewCustomerProfile(customerName);
                                break;
                            }
                            case 6: { // Edit Customer Contact
                                string customerName;
                                cout << "Enter customer name: ";
                                cin >> customerName;
                                clearScreen();
                                myStore.editCustomerContact(customerName);
                                break;
                            }
                            case 7: { // Search Product
                                string query;
                                cout << "Enter product name or barcode to search: ";
                                cin >> query;
                                clearScreen();
                                myStore.searchProduct(query);
                                break;
                            }
                            case 8: myStore.viewSalesHistory(); break;
                            case 9: { // Process Refund
                                string barcode;
                                int quantity;
                                cout << "Enter product barcode: ";
                                cin >> barcode;
                                clearScreen();
                                cout << "Enter quantity to refund: ";
                                cin >> quantity;
                                clearScreen();
                                myStore.processRefund(barcode, quantity);
                                break;
                            }
                            case 10: myStore.generateCustomerSpendingGraph(); break;
                            case 11: cout << "Exiting Cashier Menu.\n"; break;
                            default:
                                cout << "Invalid choice! Please try again.\n";
                                pause();
                                clearScreen();
                        }
                    } while (cashierChoice != 11);
                } else {
                    cout << "Invalid login credentials!\n";
                    pause();
                    clearScreen();
                }
                break;
            }

            case 3: { // Customer Login
                cout << "\n=====================================\n";
                cout << "           Customer Login            \n";
                cout << "=====================================\n";
                cout << "Enter your credentials to access your account.\n";

                Customer* customer = myStore.customerLogin();
                if (customer) {
                    myStore.customerMenu(customer);
                }
                break;
            }

            case 4: { // Customer Registration
                cout << "\n=====================================\n";
                cout << "        Customer Registration        \n";
                cout << "=====================================\n";
                cout << "Please fill in the details to create your account.\n";

                myStore.customerRegistration();
                pause();
                clearScreen();
                break;
            }

            case 5:
                cout << "Exiting the system.\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
                pause();
                clearScreen();
        }
    } while (choice != 5);

    return 0;
}
