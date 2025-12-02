#include <iostream>
#include <locale.h>
#include "classes.h"
#include "menu.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");
    system("chcp 1251");

    BonusSystem system;
    mainMenu(system);

    cout << "\nПрограмма завершена. До свидания!" << endl;
    return 0;
}