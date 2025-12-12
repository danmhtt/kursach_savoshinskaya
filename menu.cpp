#include "menu.h"
#include "input.h"
#include <iostream>
#include <memory>
using namespace std;

void manageAccountsMenu(BonusSystem& system) {
    int choice;
    do {
        cout << "\n-- Управление учетными записями --" << endl;
        cout << "1. Одобрить заявки на регистрацию" << endl;
        cout << "2. Добавить пользователя" << endl;
        cout << "3. Удалить пользователя" << endl;
        cout << "0. Назад" << endl;
        cout << "Выберите действие: ";

        choice = getIntInput("", 0, 3);

        switch (choice) {
        case 1:
            system.approveRegistration();
            break;
        case 2:
            system.addUser();
            break;
        case 3:
            system.deleteUser();
            break;
        case 0:
            cout << "Возврат в главное меню..." << endl;
            break;
        }
    } while (choice != 0);
}

void viewDataMenu(BonusSystem& system) {
    int choice;
    do {
        cout << "\n-- Просмотр данных --" << endl;
        cout << "1. Просмотр детальной информации сотрудника" << endl;
        cout << "2. Поиск пользователей" << endl;
        cout << "3. Сортировка пользователей" << endl;
        cout << "4. Просмотр всех пользователей" << endl;
        cout << "0. Назад" << endl;
        cout << "Выберите действие: ";

        choice = getIntInput("", 0, 4);

        switch (choice) {
        case 1:
            system.viewEmployeeDetails();
            break;
        case 2:
            system.searchUsers();
            break;
        case 3:
            system.sortUsers();
            break;
        case 4:
            system.viewAllUsers();
            break;
        case 0:
            cout << "Возврат в главное меню..." << endl;
            break;
        }
    } while (choice != 0);
}

void adminMenu(BonusSystem& system) {
    int choice;
    do {
        cout << "\n-- Панель администратора --" << endl;
        cout << "1. Управление учетными записями" << endl;
        cout << "2. Просмотр данных" << endl;
        cout << "3. Редактирование данных сотрудников" << endl;
        cout << "4. Расчет и анализ премий" << endl;
        cout << "5. Настройка формулы расчета премий" << endl;
        cout << "6. Демонстрация возможностей C++" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";

        choice = getIntInput("", 0, 6);

        switch (choice) {
        case 1:
            manageAccountsMenu(system);
            break;
        case 2:
            viewDataMenu(system);
            break;
        case 3:
            system.editEmployeeData();
            break;
        case 4:
            system.calculateAndViewBonuses();
            break;
        case 5:
            system.configureBonusFormula();
            break;
        case 6: {
            cout << "\n-- Демонстрация возможностей C++ --" << endl;

            Repository<int> repo;
            repo.add(100);
            repo.add(200);
            cout << "Шаблонный репозиторий: размер = " << repo.size() << endl;

            auto testEmp = make_shared<Employee>("demo", "demo", "Демо Сотрудник");
            cout << "Умный указатель: " << testEmp->getFullName() << endl;

            cout << "Всего пользователей: " << User::getUserCount() << endl;

            Date d(10, 10, 2020);
            cout << "Перегрузка оператора << для Date: " << d << endl;

            Employee emp("test", "test", "Тест");
            printEmployeeInfo(emp);
            cout << endl;

            break;
        }
        case 0:
            cout << "Выход из системы..." << endl;
            break;
        }
    } while (choice != 0);
}

void mainMenu(BonusSystem& system) {
    int choice;
    do {
        cout << "\n-- Система расчета премиальных выплат --" << endl;
        cout << "1. Авторизация" << endl;
        cout << "2. Регистрация" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";

        choice = getIntInput("", 0, 2);

        switch (choice) {
        case 1: {
            cout << "\n-- Авторизация --" << endl;
            string username, password;
            cout << "Логин: ";
            getline(cin >> ws, username);
            cout << "Пароль: ";
            password = system.getHiddenPassword();

            auto user = system.authenticate(username, password);
            if (user) {
                cout << "\nДобро пожаловать, " << user->getFullName() << "!" << endl;
                if (user->getRole() == "admin") {
                    adminMenu(system);
                }
                else {
                    auto emp = dynamic_pointer_cast<Employee>(user);
                    if (emp) {
                        emp->showMenu();
                    }
                }
            }
            else {
                cout << "Ошибка авторизации!" << endl;
            }
            break;
        }
        case 2:
            system.registerUser();
            break;
        case 0:
            cout << "Выход из программы..." << endl;
            break;
        }
    } while (choice != 0);
}