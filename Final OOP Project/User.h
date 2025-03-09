#ifndef USER_H
#define USER_H

#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;

class User {
protected:
    string username;
    string password;
public:
    // Constructor
    User(string u, string p);

    // Authentication
    virtual bool authenticate(string u, string p);

    // Pure virtual function for menu
    virtual void menu() = 0;
};

// ✅ Admin Class
class Admin : public User {
public:
    Admin(string u, string p);

    // Load Admin Users from file
    static unordered_map<string, string> loadUsers(const string& filename);

    // Admin Menu
    void menu() override;
};

// ✅ Cashier Class
class Cashier : public User {
public:
    Cashier(string u, string p);

    // Load Cashier Users from file
    static unordered_map<string, string> loadUsers(const string& filename);

    // Cashier Menu
    void menu() override;
};

#endif // USER_H

