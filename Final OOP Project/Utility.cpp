
#include "Utility.h"
using namespace std;

// ✅ Function to clear the console screen
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux/macOS
    #endif
}

// ✅ Function to pause execution until the user presses Enter
void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}
