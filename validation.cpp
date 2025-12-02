#include "validation.h"

string toLowerRussian(const string& str) {
    string result = str;
    for (char& c : result) {
        if (c >= 'А' && c <= 'Я') c = c + 32;
        else if (c >= 'A' && c <= 'Z') c = c + 32;
        else if (c >= 'a' && c <= 'z') c = tolower(c);
    }
    return result;
}

bool equalsIgnoreCase(const string& str1, const string& str2) {
    string s1 = toLowerRussian(str1);
    string s2 = toLowerRussian(str2);
    return s1 == s2;
}

bool isValidName(const string& name) {
    if (name.empty()) {
        cout << "ОШИБКА: Строка не должна быть пустой.\n";
        return false;
    }

    for (char c : name) {
        if ((c >= '0' && c <= '9') ||
            !((c >= 'а' && c <= 'я') ||
                (c >= 'А' && c <= 'Я') ||
                (c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                c == ' ' || c == '-' || c == '.')) {
            cout << "ОШИБКА: В ФИО могут быть только буквы, пробелы, дефисы и точки.\n";
            return false;
        }
    }

    if (name[0] == '-' || name[name.length() - 1] == '-') {
        cout << "ОШИБКА: Дефис не может быть в конце или в начале.\n";
        return false;
    }

    for (size_t i = 0; i < name.length() - 1; ++i) {
        if (name[i] == '-' && name[i + 1] == '-') {
            cout << "ОШИБКА: Не может быть два дефиса подряд.\n";
            return false;
        }
    }
    return true;
}

bool isValidYear(int year) {
    if (year < 1900 || year > 2100) {
        cout << "ОШИБКА: Год должен быть между 1900 и 2100.\n";
        return false;
    }
    return true;
}

bool isValidMonth(int month) {
    if (month < 1 || month > 12) {
        cout << "ОШИБКА: Месяц должен быть между 1 и 12.\n";
        return false;
    }
    return true;
}

bool isValidDay(int day, int month, int year) {
    if (day < 1) {
        cout << "ОШИБКА: День не может быть меньше 1.\n";
        return false;
    }

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap && day > 29) {
            cout << "ОШИБКА: В феврале високосного года максимум 29 дней.\n";
            return false;
        }
        else if (!isLeap && day > 28) {
            cout << "ОШИБКА: В феврале невисокосного года максимум 28 дней.\n";
            return false;
        }
    }
    else if (day > daysInMonth[month - 1]) {
        cout << "ОШИБКА: В этом месяце максимум " << daysInMonth[month - 1] << " дней.\n";
        return false;
    }

    return true;
}

bool isValidSalary(double salary) {
    if (salary <= 0 || salary > 1000000) {
        cout << "ОШИБКА: Зарплата должна быть между 0 и 1000000.\n";
        return false;
    }
    return true;
}

bool isValidKPI(double kpi) {
    if (kpi < 0 || kpi > 100) {
        cout << "ОШИБКА: KPI должен быть между 0 и 100%.\n";
        return false;
    }
    return true;
}

bool isValidCoefficient(double coeff) {
    if (coeff < 0 || coeff > 1) {
        cout << "ОШИБКА: Коэффициент должен быть между 0 и 1.\n";
        return false;
    }
    return true;
}

bool isValidUsername(const string& username) {
    if (username.empty()) {
        cout << "ОШИБКА: Логин не может быть пустым.\n";
        return false;
    }

    for (char c : username) {
        if (!((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_' || c == '-')) {
            cout << "ОШИБКА: Логин может содержать только буквы, цифры, подчеркивания и дефисы.\n";
            return false;
        }
    }

    if (username.length() < 3) {
        cout << "ОШИБКА: Логин должен быть не менее 3 символов.\n";
        return false;
    }

    if (username.length() > 20) {
        cout << "ОШИБКА: Логин должен быть не более 20 символов.\n";
        return false;
    }

    return true;
}

bool isValidPassword(const string& password) {
    if (password.empty()) {
        cout << "ОШИБКА: Пароль не может быть пустым.\n";
        return false;
    }

    if (password.length() < 4) {
        cout << "ОШИБКА: Пароль должен быть не менее 4 символов.\n";
        return false;
    }

    if (password.length() > 20) {
        cout << "ОШИБКА: Пароль должен быть не более 20 символов.\n";
        return false;
    }

    return true;
}

bool isValidDepartment(const string& department) {
    if (department.empty()) {
        cout << "ОШИБКА: Отдел не может быть пустым.\n";
        return false;
    }
    return true;
}

bool isValidPosition(const string& position) {
    if (position.empty()) {
        cout << "ОШИБКА: Должность не может быть пустой.\n";
        return false;
    }
    return true;
}