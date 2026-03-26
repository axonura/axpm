#include <iostream>
#include <limits>

using namespace std;
int main() {
    double a, b, result;
    char op;

    cout << "Simple Calculator v1.0" << endl;
    cout << "Copyright (c) 2026 First Person" << endl << endl;

    while (true) {
        cout << "Enter Simple Formula To Calculate: ";

        if (!(cin >> a >> op >> b)) {
            cout << "Invalid input!" << endl;
            cin.clear();  // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            continue;
        }

        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    cout << "Cannot divide by zero!" << endl;
                    continue;
                }
                result = a / b;
                break;
            default:
                cout << "Unknown operator!" << endl;
                continue;
        }

        cout << "Result: " << result << endl;
    }

    return 0;
}