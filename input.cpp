#include "input.h"

int getIntInput(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ОШИБКА: Пожалуйста, введите целое число.\n";
            continue;
        }

        if (value < min || value > max) {
            cout << "ОШИБКА: Число должно быть между " << min << " и " << max << ".\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

double getDoubleInput(const string& prompt, double min, double max) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ОШИБКА: Пожалуйста, введите число.\n";
            continue;
        }

        if (value < min || value > max) {
            cout << "ОШИБКА: Число должно быть между " << min << " и " << max << ".\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}