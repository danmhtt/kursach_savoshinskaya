#include "classes.h"
#include "validation.h"
#include "input.h"
#include "table_format.h"
#include <fstream>
#include <algorithm>
#include <conio.h>
#include <locale.h>
#include <iomanip>
#include <stdexcept>

using namespace std;

int User::userCount = 0;
const int Date::MIN_YEAR = 1900;

namespace Encryption {
    string hashPassword(const string& password) {
        string hashed = password;
        for (char& c : hashed) c ^= 0x55;
        return hashed;
    }

    bool verifyPassword(const string& password, const string& hashedPassword) {
        return hashPassword(password) == hashedPassword;
    }
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

string Date::toString() const {
    return to_string(day) + "." + to_string(month) + "." + to_string(year);
}

Date Date::fromString(const string& dateStr) {
    int d, m, y;
    char dot;
    stringstream ss(dateStr);
    ss >> d >> dot >> m >> dot >> y;
    return Date(d, m, y);
}

int Date::calculateExperience() const {
    time_t now = time(0);
    tm currentTime;
    localtime_s(&currentTime, &now);
    int currentYear = currentTime.tm_year + 1900;
    int currentMonth = currentTime.tm_mon + 1;
    int experience = currentYear - year;
    if (currentMonth < month) experience--;
    return max(0, experience);
}

ostream& operator<<(ostream& os, const Date& date) {
    os << date.day << "." << date.month << "." << date.year;
    return os;
}

KPI::KPI(double pc, double cq, double tw, double in)
    : projectCompletion(pc), codeQuality(cq), teamwork(tw), innovation(in) {}

double KPI::getTotalKPI() const {
    return (projectCompletion * 0.4 + codeQuality * 0.3 + teamwork * 0.2 + innovation * 0.1);
}

double KPI::getProjectCompletion() const { return projectCompletion; }
double KPI::getCodeQuality() const { return codeQuality; }
double KPI::getTeamwork() const { return teamwork; }
double KPI::getInnovation() const { return innovation; }

void KPI::setProjectCompletion(double value) { projectCompletion = value; }
void KPI::setCodeQuality(double value) { codeQuality = value; }
void KPI::setTeamwork(double value) { teamwork = value; }
void KPI::setInnovation(double value) { innovation = value; }

string KPI::toString() const {
    return "Проекты: " + to_string((int)projectCompletion) + "%, " +
        "Качество: " + to_string((int)codeQuality) + "%, " +
        "Команда: " + to_string((int)teamwork) + "%, " +
        "Инновации: " + to_string((int)innovation) + "%";
}

BonusFormula::BonusFormula(double kpiCoeff, double expCoeff, double maxExpBonus)
    : kpiCoefficient(kpiCoeff), experienceCoefficient(expCoeff), maxExperienceBonus(maxExpBonus) {}

double BonusFormula::getKpiCoefficient() const { return kpiCoefficient; }
double BonusFormula::getExperienceCoefficient() const { return experienceCoefficient; }
double BonusFormula::getMaxExperienceBonus() const { return maxExperienceBonus; }

void BonusFormula::setKpiCoefficient(double coeff) { kpiCoefficient = coeff; }
void BonusFormula::setExperienceCoefficient(double coeff) { experienceCoefficient = coeff; }
void BonusFormula::setMaxExperienceBonus(double bonus) { maxExperienceBonus = bonus; }

double BonusFormula::calculateBonus(double salary, double kpiScore, int experience) const {
    double experienceBonus = min(experience * experienceCoefficient, maxExperienceBonus);
    double kpiBonus = kpiScore / 100 * kpiCoefficient;
    return salary * (kpiBonus + experienceBonus);
}

void BonusFormula::displayFormula() const {
    cout << "\n-- Формула расчета премий --" << endl;
    cout << "Формула: Премия = Зарплата * (KPI_бонус + Стаж_бонус)" << endl;
    cout << "Где:" << endl;
    cout << "  • KPI_бонус = (KPI / 100) * " << (kpiCoefficient * 100) << "%" << endl;
    cout << "  • Стаж_бонус = min(Стаж * " << experienceCoefficient << ", " << (maxExperienceBonus * 100) << "%)" << endl;
    cout << "\nКоэффициенты:" << endl;
    cout << "  • Коэффициент KPI: " << kpiCoefficient << " (" << (kpiCoefficient * 100) << "%)" << endl;
    cout << "  • Коэффициент стажа: " << experienceCoefficient << " (" << (experienceCoefficient * 100) << "% за год)" << endl;
    cout << "  • Максимальный бонус за стаж: " << maxExperienceBonus << " (" << (maxExperienceBonus * 100) << "%)" << endl;
}

string BonusFormula::toString() const {
    return to_string(kpiCoefficient) + "," + to_string(experienceCoefficient) + "," + to_string(maxExperienceBonus);
}

BonusFormula BonusFormula::fromString(const string& str) {
    stringstream ss(str);
    string token;
    vector<double> values;

    while (getline(ss, token, ',')) {
        values.push_back(stod(token));
    }

    if (values.size() == 3) {
        return BonusFormula(values[0], values[1], values[2]);
    }

    return BonusFormula();
}

BonusFormula BonusFormula::getDefault() {
    return BonusFormula();
}

User::User(string uname, string pwd, string name, string r, bool approved)
    : username(uname), fullName(name), role(r), isApproved(approved) {
    setPassword(pwd);
    userCount++;
}

User::~User() {
    userCount--;
}

string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
string User::getFullName() const { return fullName; }
string User::getRole() const { return role; }
bool User::getIsApproved() const { return isApproved; }

void User::setUsername(const string& uname) { username = uname; }
void User::setPassword(const string& pwd) { password = Encryption::hashPassword(pwd); }
void User::setFullName(const string& name) { fullName = name; }
void User::setRole(const string& r) { role = r; }
void User::setIsApproved(bool approved) { isApproved = approved; }

bool User::verifyPassword(const string& pwd) const {
    return Encryption::verifyPassword(pwd, password);
}

int User::getUserCount() {
    return userCount;
}

Employee::Employee(string uname, string pwd, string name,
    string dept, string pos, double sal, Date hire)
    : User(uname, pwd, name, "user", true), department(dept), position(pos),
    salary(sal), hireDate(hire) {}

string Employee::getDepartment() const { return department; }
string Employee::getPosition() const { return position; }
double Employee::getSalary() const { return salary; }
Date Employee::getHireDate() const { return hireDate; }
KPI Employee::getKPI() const { return kpi; }

void Employee::setDepartment(const string& dept) { department = dept; }
void Employee::setPosition(const string& pos) { position = pos; }
void Employee::setSalary(double sal) { salary = sal; }
void Employee::setHireDate(Date hire) { hireDate = hire; }
void Employee::setKPI(const KPI& k) { kpi = k; }

double Employee::calculateBonus(const BonusFormula& formula) const {
    double kpiScore = kpi.getTotalKPI();
    int experience = hireDate.calculateExperience();
    return formula.calculateBonus(salary, kpiScore, experience);
}

int Employee::getExperience() const {
    return hireDate.calculateExperience();
}

string Employee::toFileString() const {
    return username + "," + password + "," + fullName + "," + role + ",1," +
        department + "," + position + "," + to_string((int)salary) + "," +
        hireDate.toString() + "," + to_string((int)kpi.getProjectCompletion()) + "," +
        to_string((int)kpi.getCodeQuality()) + "," + to_string((int)kpi.getTeamwork()) + "," +
        to_string((int)kpi.getInnovation());
}

void Employee::displayDetailedInfo(const BonusFormula& formula) const {
    cout << "\n-- Детальная информация о сотруднике --" << endl;
    cout << "ФИО: " << fullName << endl;
    cout << "Логин: " << username << endl;
    cout << "Отдел: " << department << endl;
    cout << "Должность: " << position << endl;
    cout << "Зарплата: " << salary << " BYN" << endl;
    cout << "Дата приема: " << hireDate.toString() << endl;
    cout << "Стаж: " << getExperience() << " лет" << endl;
    cout << "KPI: " << kpi.toString() << endl;
    cout << "Общий KPI: " << (int)kpi.getTotalKPI() << "%" << endl;
    cout << "Расчетная премия: " << calculateBonus(formula) << " BYN" << endl;
}

void Employee::showMenu() {
    BonusFormula defaultFormula;
    int choice;
    do {
        cout << "\n-- Панель сотрудника --" << endl;
        cout << "1. Просмотр моих данных" << endl;
        cout << "2. Просмотр моих KPI" << endl;
        cout << "3. Расчет моей премии" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";

        choice = getIntInput("", 0, 3);

        switch (choice) {
        case 1:
            displayDetailedInfo(defaultFormula);
            break;
        case 2:
            cout << "\n-- Мои KPI --" << endl;
            cout << kpi.toString() << endl;
            cout << "Общий показатель: " << (int)kpi.getTotalKPI() << "%" << endl;
            break;
        case 3:
            cout << "\n-- Расчет премии --" << endl;
            cout << "Расчетная премия: " << calculateBonus(defaultFormula) << " BYN" << endl;
            break;
        case 0:
            cout << "Выход из системы..." << endl;
            break;
        }
    } while (choice != 0);
}

Admin::Admin(string uname, string pwd, string name)
    : User(uname, pwd, name, "admin", true) {}

void Admin::showMenu() {}

string Admin::toFileString() const {
    return username + "," + password + "," + fullName + "," + role + ",1,2024-01-01";
}

BonusSystem::BonusSystem(string filename, string formulaFilename)
    : dataFile(filename), formulaFile(formulaFilename) {
    createDefaultAdmin();
    loadFormula();
    loadData();
}

BonusSystem::~BonusSystem() {
    saveData();
    saveFormula();
}

void BonusSystem::createDefaultAdmin() {
    users.push_back(make_shared<Admin>());
}

void BonusSystem::loadFormula() {
    ifstream file(formulaFile);
    if (!file.is_open()) {
        cout << "Файл с формулой не найден. Используются значения по умолчанию." << endl;
        formula = BonusFormula();
        return;
    }

    string line;
    getline(file, line);
    if (!line.empty()) {
        formula = BonusFormula::fromString(line);
    }
    file.close();
}

void BonusSystem::saveFormula() {
    ofstream file(formulaFile);
    file << formula.toString();
    file.close();
    cout << "Формула расчета премий сохранена." << endl;
}

BonusFormula& BonusSystem::getFormula() { return formula; }

void BonusSystem::loadData() {
    ifstream file(dataFile);
    if (!file.is_open()) {
        cout << "Файл данных не найден. Будет создан новый при сохранении." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> tokens;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 5) {
            string username = tokens[0];
            string password = tokens[1];
            string fullName = tokens[2];
            string role = tokens[3];
            bool approved = (tokens[4] == "1");

            if (role == "admin") {
                continue;
            }
            else if (tokens.size() >= 10) {
                auto emp = make_shared<Employee>(username, "", fullName);
                emp->setPassword(password);
                emp->setIsApproved(approved);
                emp->setDepartment(tokens[5]);
                emp->setPosition(tokens[6]);
                emp->setSalary(stod(tokens[7]));
                emp->setHireDate(Date::fromString(tokens[8]));

                KPI kpi(stod(tokens[9]), stod(tokens[10]), stod(tokens[11]), stod(tokens[12]));
                emp->setKPI(kpi);

                employees.push_back(emp);
                users.push_back(emp);
            }
        }
    }
    file.close();
}

void BonusSystem::saveData() {
    ofstream file(dataFile);
    for (const auto& user : users) {
        file << user->toFileString() << endl;
    }
    file.close();
    cout << "Данные сохранены в файл." << endl;
}

shared_ptr<User> BonusSystem::authenticate(const string& username, const string& password) {
    for (const auto& user : users) {
        if (user->getUsername() == username && user->verifyPassword(password) && user->getIsApproved()) {
            return user;
        }
    }
    return nullptr;
}

bool BonusSystem::usernameExists(const string& username) {
    for (const auto& user : users) {
        if (user->getUsername() == username) return true;
    }
    return false;
}

void BonusSystem::registerUser() {
    cout << "\n-- Регистрация нового пользователя --" << endl;

    string username, password, fullName, department, position;

    while (true) {
        cout << "Логин: ";
        getline(cin >> ws, username);

        if (!isValidUsername(username)) continue;
        if (usernameExists(username)) {
            cout << "Пользователь с таким логином уже существует!" << endl;
            continue;
        }
        break;
    }

    while (true) {
        cout << "Пароль: ";
        getline(cin >> ws, password);
        if (isValidPassword(password)) break;
    }

    while (true) {
        cout << "ФИО: ";
        getline(cin, fullName);
        if (isValidName(fullName)) break;
    }

    while (true) {
        cout << "Отдел: ";
        getline(cin, department);
        if (isValidDepartment(department)) break;
    }

    while (true) {
        cout << "Должность: ";
        getline(cin, position);
        if (isValidPosition(position)) break;
    }

    auto emp = make_shared<Employee>(username, password, fullName, department, position, 0, Date());
    emp->setIsApproved(false);
    pendingRegistrations.push_back(emp);

    cout << "\nЗаявка на регистрацию отправлена!" << endl;
    cout << "Ожидайте одобрения администратора." << endl;
}

void BonusSystem::approveRegistration() {
    cout << "\n-- Одобрение заявок на регистрацию --" << endl;

    if (pendingRegistrations.empty()) {
        cout << "Нет заявок на одобрение." << endl;
        return;
    }

    for (size_t i = 0; i < pendingRegistrations.size(); i++) {
        auto emp = dynamic_pointer_cast<Employee>(pendingRegistrations[i]);
        cout << i + 1 << ". " << emp->getFullName() << " - " << emp->getDepartment() << ", " << emp->getPosition() << endl;
    }

    cout << "\nВведите номер заявки для одобрения (0 для отмены): ";
    int index = getIntInput("", 0, pendingRegistrations.size());

    if (index == 0) {
        cout << "Отмена операции." << endl;
        return;
    }

    auto emp = dynamic_pointer_cast<Employee>(pendingRegistrations[index - 1]);
    cout << "\nНастройка данных сотрудника: " << emp->getFullName() << endl;

    double salary = getDoubleInput("Зарплата: ", 0, 1000000);
    emp->setSalary(salary);

    int day, month, year;
    while (true) {
        cout << "\nДата приема на работу:" << endl;
        year = getIntInput("Год: ", 1900, 2100);
        month = getIntInput("Месяц: ", 1, 12);
        day = getIntInput("День: ", 1, 31);

        if (isValidYear(year) && isValidMonth(month) && isValidDay(day, month, year)) {
            break;
        }
    }

    emp->setHireDate(Date(day, month, year));

    cout << "\n-- Ввод показателей KPI для сотрудника --" << endl;
    cout << "Введите показатели от 0 до 100%:" << endl;

    double pc, cq, tw, in;

    while (true) {
        pc = getDoubleInput("Выполнение проектов (%): ", 0, 100);
        cq = getDoubleInput("Качество кода (%): ", 0, 100);
        tw = getDoubleInput("Работа в команде (%): ", 0, 100);
        in = getDoubleInput("Инновации (%): ", 0, 100);

        if (isValidKPI(pc) && isValidKPI(cq) && isValidKPI(tw) && isValidKPI(in)) {
            break;
        }
        cout << "Пожалуйста, проверьте введенные значения KPI." << endl;
    }

    KPI newKPI(pc, cq, tw, in);
    emp->setKPI(newKPI);

    cout << "\n-- Итоговые данные сотрудника --" << endl;
    cout << "ФИО: " << emp->getFullName() << endl;
    cout << "Отдел: " << emp->getDepartment() << endl;
    cout << "Должность: " << emp->getPosition() << endl;
    cout << "Зарплата: " << salary << " BYN" << endl;
    cout << "Дата приема: " << day << "." << month << "." << year << endl;
    cout << "KPI: " << newKPI.toString() << endl;
    cout << "Общий KPI: " << (int)newKPI.getTotalKPI() << "%" << endl;

    cout << "\nПодтвердить добавление сотрудника? (1 - да, 0 - нет): ";
    int confirm = getIntInput("", 0, 1);

    if (confirm == 1) {
        emp->setIsApproved(true);
        employees.push_back(emp);
        users.push_back(emp);
        pendingRegistrations.erase(pendingRegistrations.begin() + index - 1);

        saveData();
        cout << "\nСотрудник успешно одобрен и добавлен в систему!" << endl;
    }
    else {
        cout << "Добавление сотрудника отменено." << endl;
    }
}

void BonusSystem::addUser() {
    cout << "\n-- Добавление пользователя --" << endl;
    string username, password, fullName, department, position;
    double salary;

    while (true) {
        cout << "Логин: ";
        getline(cin >> ws, username);
        if (!isValidUsername(username)) continue;
        if (usernameExists(username)) {
            cout << "Пользователь с таким логином уже существует!" << endl;
            continue;
        }
        break;
    }

    while (true) {
        cout << "Пароль: ";
        getline(cin >> ws, password);
        if (isValidPassword(password)) break;
    }

    while (true) {
        cout << "ФИО: ";
        getline(cin, fullName);
        if (isValidName(fullName)) break;
    }

    while (true) {
        cout << "Отдел: ";
        getline(cin, department);
        if (isValidDepartment(department)) break;
    }

    while (true) {
        cout << "Должность: ";
        getline(cin, position);
        if (isValidPosition(position)) break;
    }

    salary = getDoubleInput("Зарплата: ", 0, 1000000);

    int day, month, year;
    while (true) {
        cout << "\nДата приема на работу:" << endl;
        year = getIntInput("Год: ", 1900, 2100);
        month = getIntInput("Месяц: ", 1, 12);
        day = getIntInput("День: ", 1, 31);

        if (isValidYear(year) && isValidMonth(month) && isValidDay(day, month, year)) {
            break;
        }
    }

    cout << "\n-- Ввод показателей KPI --" << endl;
    cout << "Введите показатели от 0 до 100%:" << endl;

    double pc, cq, tw, in;

    while (true) {
        pc = getDoubleInput("Выполнение проектов (%): ", 0, 100);
        cq = getDoubleInput("Качество кода (%): ", 0, 100);
        tw = getDoubleInput("Работа в команде (%): ", 0, 100);
        in = getDoubleInput("Инновации (%): ", 0, 100);

        if (isValidKPI(pc) && isValidKPI(cq) && isValidKPI(tw) && isValidKPI(in)) {
            break;
        }
        cout << "Пожалуйста, проверьте введенные значения KPI." << endl;
    }

    auto emp = make_shared<Employee>(username, password, fullName, department, position, salary, Date(day, month, year));
    emp->setKPI(KPI(pc, cq, tw, in));
    employees.push_back(emp);
    users.push_back(emp);

    saveData();
    cout << "Пользователь успешно добавлен!" << endl;
}

void BonusSystem::deleteUser() {
    if (employees.empty()) {
        cout << "Нет пользователей для удаления." << endl;
        return;
    }

    displayAllEmployees();
    cout << "\nВведите номер пользователя для удаления (0 для отмены): ";
    int index = getIntInput("", 0, employees.size());

    if (index == 0) {
        cout << "Отмена операции." << endl;
        return;
    }

    auto emp = employees[index - 1];
    auto it = find(employees.begin(), employees.end(), emp);
    if (it != employees.end()) employees.erase(it);

    auto it2 = find(users.begin(), users.end(), emp);
    if (it2 != users.end()) users.erase(it2);

    saveData();
    cout << "Пользователь успешно удален!" << endl;
}

void BonusSystem::viewEmployeeDetails() {
    if (employees.empty()) {
        cout << "Нет сотрудников для просмотра." << endl;
        return;
    }

    displayAllEmployees();
    cout << "\nВведите номер сотрудника для просмотра детальной информации (0 для отмены): ";
    int index = getIntInput("", 0, employees.size());

    if (index == 0) {
        cout << "Отмена операции." << endl;
        return;
    }

    employees[index - 1]->displayDetailedInfo(formula);
}

void BonusSystem::searchUsers() {
    if (employees.empty()) {
        cout << "Нет пользователей для поиска." << endl;
        return;
    }

    cout << "\n-- Поиск пользователей --" << endl;
    cout << "1. Поиск по ФИО" << endl;
    cout << "2. Поиск по должности" << endl;
    cout << "3. Поиск по отделу" << endl;
    cout << "0. Отмена" << endl;
    cout << "Выберите тип поиска: ";

    int choice = getIntInput("", 0, 3);

    if (choice == 0) {
        cout << "Отмена операции." << endl;
        return;
    }

    cin.ignore();
    string searchTerm;
    cout << "Введите поисковый запрос: ";
    getline(cin, searchTerm);

    string searchTermLower = toLowerRussian(searchTerm);
    vector<shared_ptr<Employee>> results;

    for (const auto& emp : employees) {
        bool match = false;
        string compareString;

        switch (choice) {
        case 1:
            compareString = toLowerRussian(emp->getFullName());
            match = (compareString.find(searchTermLower) != string::npos);
            break;
        case 2:
            compareString = toLowerRussian(emp->getPosition());
            match = (compareString.find(searchTermLower) != string::npos);
            break;
        case 3:
            compareString = toLowerRussian(emp->getDepartment());
            match = (compareString.find(searchTermLower) != string::npos);
            break;
        }
        if (match) results.push_back(emp);
    }

    if (results.empty()) {
        cout << "Пользователи не найдены." << endl;
    }
    else {
        cout << "\nРезультаты поиска:" << endl;
        for (size_t i = 0; i < results.size(); i++) {
            cout << i + 1 << ". " << results[i]->getFullName() << " - "
                << results[i]->getPosition() << " (" << results[i]->getDepartment()
                << ") Премия: " << results[i]->calculateBonus(formula) << " BYN" << endl;
        }
    }
}

void BonusSystem::sortUsers() {
    if (employees.empty()) {
        cout << "Нет пользователей для сортировки." << endl;
        return;
    }

    cout << "\n-- Сортировка пользователей --" << endl;
    cout << "1. По ФИО" << endl;
    cout << "2. По размеру премии" << endl;
    cout << "3. По стажу" << endl;
    cout << "4. По отделу" << endl;
    cout << "0. Отмена" << endl;
    cout << "Выберите тип сортировки: ";

    int choice = getIntInput("", 0, 4);

    if (choice == 0) {
        cout << "Отмена операции." << endl;
        return;
    }

    vector<shared_ptr<Employee>> sortedEmployees = employees;

    switch (choice) {
    case 1:
        sort(sortedEmployees.begin(), sortedEmployees.end(),
            [](const shared_ptr<Employee>& a, const shared_ptr<Employee>& b) {
                return toLowerRussian(a->getFullName()) < toLowerRussian(b->getFullName());
            });
        break;
    case 2:
        sort(sortedEmployees.begin(), sortedEmployees.end(),
            [this](const shared_ptr<Employee>& a, const shared_ptr<Employee>& b) {
                return a->calculateBonus(formula) > b->calculateBonus(formula);
            });
        break;
    case 3:
        sort(sortedEmployees.begin(), sortedEmployees.end(),
            [](const shared_ptr<Employee>& a, const shared_ptr<Employee>& b) {
                return a->getExperience() > b->getExperience();
            });
        break;
    case 4:
        sort(sortedEmployees.begin(), sortedEmployees.end(),
            [](const shared_ptr<Employee>& a, const shared_ptr<Employee>& b) {
                return toLowerRussian(a->getDepartment()) < toLowerRussian(b->getDepartment());
            });
        break;
    }

    cout << "\nОтсортированный список:" << endl;
    for (size_t i = 0; i < sortedEmployees.size(); i++) {
        auto emp = sortedEmployees[i];
        cout << i + 1 << ". " << emp->getFullName() << " - " << emp->getDepartment()
            << ", " << emp->getPosition() << " (Премия: " << emp->calculateBonus(formula)
            << " BYN, Стаж: " << emp->getExperience() << " лет)" << endl;
    }
}

void BonusSystem::viewAllUsers() {
    if (employees.empty()) {
        cout << "Нет пользователей в системе." << endl;
        return;
    }

    cout << "\nВсе пользователи системы:" << endl;
    drawTableLine();
    drawTableHeader();
    drawTableLine();

    for (size_t i = 0; i < employees.size(); i++) {
        auto emp = employees[i];
        double bonus = emp->calculateBonus(formula);
        double kpi = emp->getKPI().getTotalKPI();

        vector<string> nameLines = splitText(emp->getFullName(), 19);

        for (size_t j = 0; j < nameLines.size(); j++) {
            if (j == 0) {
                cout << "| " << centerText(to_string(i + 1), 3) << " | "
                    << centerText(emp->getUsername(), 19) << " | "
                    << centerText(nameLines[j], 19) << " | "
                    << centerText(emp->getPosition(), 20) << " | "
                    << centerText(to_string((int)kpi), 5) << " | "
                    << formatDouble(bonus, 14) << " |" << endl;
            }
            else {
                cout << "| " << centerText("", 3) << " | "
                    << centerText("", 19) << " | "
                    << centerText(nameLines[j], 19) << " | "
                    << centerText("", 20) << " | "
                    << centerText("", 5) << " | "
                    << centerText("", 14) << " |" << endl;
            }
        }

        if (nameLines.size() > 1 && i < employees.size() - 1) {
            drawTableLine();
        }
    }
    drawTableLine();
}

void BonusSystem::editEmployeeData() {
    if (employees.empty()) {
        cout << "Нет сотрудников для редактирования." << endl;
        return;
    }

    displayAllEmployees();
    cout << "\nВведите номер сотрудника для редактирования (0 для отмены): ";
    int index = getIntInput("", 0, employees.size());

    if (index == 0) {
        cout << "Отмена операции." << endl;
        return;
    }

    auto emp = employees[index - 1];

    int choice;
    do {
        cout << "\n-- Редактирование данных: " << emp->getFullName() << " --" << endl;
        cout << "1. Редактировать личные данные" << endl;
        cout << "2. Редактировать показатели KPI" << endl;
        cout << "3. Изменить зарплату" << endl;
        cout << "4. Изменить дату приема" << endl;
        cout << "5. Изменить отдел и должность" << endl;
        cout << "0. Назад" << endl;
        cout << "Выберите действие: ";

        choice = getIntInput("", 0, 5);

        switch (choice) {
        case 1: {
            string newName;
            while (true) {
                cout << "Новое ФИО: ";
                getline(cin >> ws, newName);
                if (isValidName(newName)) break;
            }
            emp->setFullName(newName);
            saveData();
            cout << "ФИО успешно изменено!" << endl;
            break;
        }
        case 2: {
            double pc, cq, tw, in;
            cout << "Текущие KPI: " << emp->getKPI().toString() << endl;

            while (true) {
                pc = getDoubleInput("Выполнение проектов (%): ", 0, 100);
                cq = getDoubleInput("Качество кода (%): ", 0, 100);
                tw = getDoubleInput("Работа в команде (%): ", 0, 100);
                in = getDoubleInput("Инновации (%): ", 0, 100);

                if (isValidKPI(pc) && isValidKPI(cq) && isValidKPI(tw) && isValidKPI(in)) {
                    break;
                }
            }

            KPI newKPI(pc, cq, tw, in);
            emp->setKPI(newKPI);
            saveData();
            cout << "KPI успешно обновлены!" << endl;
            break;
        }
        case 3: {
            double newSalary = getDoubleInput("Новая зарплата: ", 0, 1000000);
            emp->setSalary(newSalary);
            saveData();
            cout << "Зарплата успешно изменена!" << endl;
            break;
        }
        case 4: {
            int day, month, year;
            cout << "Текущая дата приема: " << emp->getHireDate().toString() << endl;

            while (true) {
                cout << "Новая дата приема:" << endl;
                year = getIntInput("Год: ", 1900, 2100);
                month = getIntInput("Месяц: ", 1, 12);
                day = getIntInput("День: ", 1, 31);

                if (isValidYear(year) && isValidMonth(month) && isValidDay(day, month, year)) {
                    break;
                }
            }

            emp->setHireDate(Date(day, month, year));
            saveData();
            cout << "Дата приема успешно изменена!" << endl;
            break;
        }
        case 5: {
            string newDept, newPos;
            while (true) {
                cout << "Новый отдел: ";
                getline(cin >> ws, newDept);
                if (isValidDepartment(newDept)) break;
            }
            while (true) {
                cout << "Новая должность: ";
                getline(cin >> ws, newPos);
                if (isValidPosition(newPos)) break;
            }
            emp->setDepartment(newDept);
            emp->setPosition(newPos);
            saveData();
            cout << "Отдел и должность успешно изменены!" << endl;
            break;
        }
        case 0:
            cout << "Возврат в меню..." << endl;
            break;
        }
    } while (choice != 0);
}

void BonusSystem::calculateAndViewBonuses() {
    if (employees.empty()) {
        cout << "Нет сотрудников для расчета премий." << endl;
        return;
    }

    cout << "\n-- Расчет и анализ премий --" << endl;

    formula.displayFormula();

    double totalBonus = 0;
    double maxBonus = 0;
    double minBonus = numeric_limits<double>::max();
    string bestEmployee, worstEmployee;

    cout << "\nДетальный расчет премий:" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "| ФИО                     | Зарплата | KPI  | Стаж | Премия  |" << endl;
    cout << "-------------------------------------------------------------" << endl;

    for (const auto& emp : employees) {
        double bonus = emp->calculateBonus(formula);
        double kpi = emp->getKPI().getTotalKPI();
        int experience = emp->getExperience();

        totalBonus += bonus;

        if (bonus > maxBonus) {
            maxBonus = bonus;
            bestEmployee = emp->getFullName();
        }
        if (bonus < minBonus) {
            minBonus = bonus;
            worstEmployee = emp->getFullName();
        }

        string name = emp->getFullName();
        if (name.length() > 22) name = name.substr(0, 19) + "...";

        cout << "| " << left << setw(23) << name
            << "| " << setw(9) << emp->getSalary()
            << "| " << setw(4) << (int)kpi << "%"
            << "| " << setw(4) << experience
            << "| " << setw(5) << (int)bonus << " BYN |" << endl;
    }
    cout << "-------------------------------------------------------------" << endl;

    cout << "\nСтатистика премий:" << endl;
    cout << "Общая сумма премий: " << totalBonus << " BYN" << endl;
    cout << "Средняя премия: " << totalBonus / employees.size() << " BYN" << endl;
    cout << "Максимальная премия: " << maxBonus << " BYN (" << bestEmployee << ")" << endl;
    cout << "Минимальная премия: " << minBonus << " BYN (" << worstEmployee << ")" << endl;

    cout << "\nРекомендации:" << endl;
    bool hasRecommendations = false;
    for (const auto& emp : employees) {
        double kpi = emp->getKPI().getTotalKPI();
        if (kpi < 70) {
            cout << "• " << emp->getFullName() << ": низкий KPI (" << (int)kpi << "%). Рекомендуется повышение показателей." << endl;
            hasRecommendations = true;
        }
    }
    if (!hasRecommendations) {
        cout << "Все сотрудники имеют хорошие показатели KPI!" << endl;
    }
}

void BonusSystem::configureBonusFormula() {
    int choice;
    do {
        cout << "\n-- Настройка формулы расчета премий --" << endl;
        cout << "Текущая формула:" << endl;
        formula.displayFormula();

        cout << "\nВыберите действие:" << endl;
        cout << "1. Изменить коэффициент KPI" << endl;
        cout << "2. Изменить коэффициент стажа" << endl;
        cout << "3. Изменить максимальный бонус за стаж" << endl;
        cout << "4. Сбросить к значениям по умолчанию" << endl;
        cout << "5. Пример расчета с текущей формулой" << endl;
        cout << "0. Назад" << endl;
        cout << "Ваш выбор: ";

        choice = getIntInput("", 0, 5);

        switch (choice) {
        case 1: {
            cout << "\n-- Изменение коэффициента KPI --" << endl;
            cout << "Текущее значение: " << formula.getKpiCoefficient() << " (" << formula.getKpiCoefficient() * 100 << "%)" << endl;

            double newCoeff = getDoubleInput("\nВведите новый коэффициент (0.0 - 1.0): ", 0.0, 1.0);
            formula.setKpiCoefficient(newCoeff);
            saveFormula();
            cout << "Коэффициент KPI успешно изменен!" << endl;
            break;
        }
        case 2: {
            cout << "\n-- Изменение коэффициента стажа --" << endl;
            cout << "Текущее значение: " << formula.getExperienceCoefficient() << " (" << formula.getExperienceCoefficient() * 100 << "% за год)" << endl;

            double newCoeff = getDoubleInput("\nВведите новый коэффициент (0.0 - 0.1): ", 0.0, 0.1);
            formula.setExperienceCoefficient(newCoeff);
            saveFormula();
            cout << "Коэффициент стажа успешно изменен!" << endl;
            break;
        }
        case 3: {
            cout << "\n-- Изменение максимального бонус за стаж --" << endl;
            cout << "Текущее значение: " << formula.getMaxExperienceBonus() << " (" << formula.getMaxExperienceBonus() * 100 << "%)" << endl;

            double newMax = getDoubleInput("\nВведите новый максимальный бонус (0.0 - 0.5): ", 0.0, 0.5);
            formula.setMaxExperienceBonus(newMax);
            saveFormula();
            cout << "Максимальный бонус за стаж успешно изменен!" << endl;
            break;
        }
        case 4: {
            cout << "\n-- Сброс к значениям по умолчанию --" << endl;
            formula = BonusFormula();
            saveFormula();
            cout << "Формула сброшена к значениям по умолчанию:" << endl;
            formula.displayFormula();
            break;
        }
        case 5: {
            cout << "\n-- Пример расчета --" << endl;
            cout << "Введите данные для примера расчета:" << endl;
            double salary = getDoubleInput("Зарплата (BYN): ", 0, 1000000);
            double kpi = getDoubleInput("KPI сотрудника (%): ", 0, 100);
            int experience = getIntInput("Стаж сотрудника (лет): ", 0, 50);

            double bonus = formula.calculateBonus(salary, kpi, experience);
            double kpiBonus = (kpi / 100) * formula.getKpiCoefficient() * salary;
            double expBonus = min(experience * formula.getExperienceCoefficient(), formula.getMaxExperienceBonus()) * salary;

            cout << "\nРасчет премии:" << endl;
            cout << "Зарплата: " << salary << " BYN" << endl;
            cout << "KPI: " << kpi << "%" << endl;
            cout << "Стаж: " << experience << " лет" << endl;
            cout << "----------------------------------------" << endl;
            cout << "Бонус за KPI: " << kpiBonus << " BYN (" << (kpi / 100) * formula.getKpiCoefficient() * 100 << "%)" << endl;
            cout << "Бонус за стаж: " << expBonus << " BYN (" << min(experience * formula.getExperienceCoefficient(), formula.getMaxExperienceBonus()) * 100 << "%)" << endl;
            cout << "----------------------------------------" << endl;
            cout << "Общая премия: " << bonus << " BYN (" << (bonus / salary) * 100 << "% от зарплаты)" << endl;
            break;
        }
        case 0:
            cout << "Возврат в меню..." << endl;
            break;
        }
    } while (choice != 0);
}

string BonusSystem::getHiddenPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            cout << '*';
            password.push_back(ch);
        }
    }
    cout << endl;
    return password;
}

void BonusSystem::displayAllEmployees() {
    viewAllUsers();
}

vector<shared_ptr<Employee>>& BonusSystem::getEmployees() { return employees; }
vector<shared_ptr<User>>& BonusSystem::getPendingRegistrations() { return pendingRegistrations; }