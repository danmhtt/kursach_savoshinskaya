#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <ctime>
#include <memory>
#include <iostream>
#include <sstream>
using namespace std;

namespace Encryption {
    string hashPassword(const string& password);
    bool verifyPassword(const string& password, const string& hashedPassword);
}

template<typename T>
class Repository {
private:
    vector<T> items;
public:
    void add(const T& item) { items.push_back(item); }
    void remove(const T& item) {
        auto it = find(items.begin(), items.end(), item);
        if (it != items.end()) items.erase(it);
    }
    vector<T> getAll() const { return items; }
    size_t size() const { return items.size(); }
};

class Date;
ostream& operator<<(ostream& os, const Date& date);

class Date {
private:
    int day, month, year;

    friend ostream& operator<<(ostream& os, const Date& date);

public:
    Date(int d = 1, int m = 1, int y = 2000);
    string toString() const;
    static Date fromString(const string& dateStr);
    int calculateExperience() const;

    static const int MIN_YEAR;
};

ostream& operator<<(ostream& os, const Date& date);

class KPI {
private:
    double projectCompletion, codeQuality, teamwork, innovation;
public:
    KPI(double pc = 0, double cq = 0, double tw = 0, double in = 0);
    double getTotalKPI() const;
    double getProjectCompletion() const;
    double getCodeQuality() const;
    double getTeamwork() const;
    double getInnovation() const;
    void setProjectCompletion(double value);
    void setCodeQuality(double value);
    void setTeamwork(double value);
    void setInnovation(double value);
    string toString() const;

    double operator[](int index) const {
        switch (index) {
        case 0: return projectCompletion;
        case 1: return codeQuality;
        case 2: return teamwork;
        case 3: return innovation;
        default: throw out_of_range("Неверный индекс KPI");
        }
    }
};

class BonusFormula {
private:
    double kpiCoefficient;
    double experienceCoefficient;
    double maxExperienceBonus;
public:
    BonusFormula(double kpiCoeff = 0.2, double expCoeff = 0.005, double maxExpBonus = 0.05);
    double getKpiCoefficient() const;
    double getExperienceCoefficient() const;
    double getMaxExperienceBonus() const;
    void setKpiCoefficient(double coeff);
    void setExperienceCoefficient(double coeff);
    void setMaxExperienceBonus(double bonus);
    double calculateBonus(double salary, double kpiScore, int experience) const;
    void displayFormula() const;
    string toString() const;
    static BonusFormula fromString(const string& str);

    static BonusFormula getDefault();
};

class User {
protected:
    string username;
    string password;
    string fullName;
    string role;
    bool isApproved;

    static int userCount;

public:
    User(string uname = "", string pwd = "", string name = "",
        string r = "user", bool approved = false);
    virtual ~User();

    string getUsername() const;
    string getPassword() const;
    string getFullName() const;
    string getRole() const;
    bool getIsApproved() const;

    void setUsername(const string& uname);
    void setPassword(const string& pwd);
    void setFullName(const string& name);
    void setRole(const string& r);
    void setIsApproved(bool approved);

    virtual bool verifyPassword(const string& pwd) const;
    virtual void showMenu() = 0;
    virtual string toFileString() const = 0;

    static int getUserCount();
};

class Employee : public User {
private:
    string department, position;
    double salary;
    Date hireDate;
    KPI kpi;

public:
    Employee(string uname = "", string pwd = "", string name = "",
        string dept = "", string pos = "", double sal = 0, Date hire = Date());

    string getDepartment() const;
    string getPosition() const;
    double getSalary() const;
    Date getHireDate() const;
    KPI getKPI() const;

    void setDepartment(const string& dept);
    void setPosition(const string& pos);
    void setSalary(double sal);
    void setHireDate(Date hire);
    void setKPI(const KPI& k);

    double calculateBonus(const BonusFormula& formula) const;
    int getExperience() const;
    void showMenu() override;
    string toFileString() const override;
    void displayDetailedInfo(const BonusFormula& formula) const;

    void updateSalary(double& newSalary, const string& reason) {
        cout << "Изменение зарплаты: " << reason << endl;
        salary = newSalary;
    }

    template<typename T>
    T getSalaryAs() const {
        return static_cast<T>(salary);
    }

    friend void printEmployeeInfo(const Employee& emp);
};

inline void printEmployeeInfo(const Employee& emp) {
    cout << "Информация: " << emp.fullName << " (" << emp.department << ")";
}

class Admin : public User {
public:
    Admin(string uname = "admin", string pwd = "admin123",
        string name = "Администратор Системы");
    void showMenu() override;
    string toFileString() const override;
};

class AdminHelper {
public:
    static void showAdminInfo(const Admin& admin) {
        cout << "Админ: " << admin.getFullName() << endl;
    }
};

template<typename T>
T calculateAverage(const vector<T>& items) {
    if (items.empty()) return T();
    T sum = T();
    for (const auto& item : items) sum += item;
    return sum / items.size();
}

class BonusSystem {
private:
    vector<shared_ptr<User>> users;
    vector<shared_ptr<Employee>> employees;
    vector<shared_ptr<User>> pendingRegistrations;
    string dataFile;
    string formulaFile;
    BonusFormula formula;

public:
    BonusSystem(string filename = "users.txt", string formulaFilename = "formula.txt");
    ~BonusSystem();

    void createDefaultAdmin();
    void loadFormula();
    void saveFormula();
    BonusFormula& getFormula();
    void loadData();
    void saveData();

    shared_ptr<User> authenticate(const string& username, const string& password);
    bool usernameExists(const string& username);
    void registerUser();
    void approveRegistration();
    void addUser();
    void deleteUser();
    void viewEmployeeDetails();
    void searchUsers();
    void sortUsers();
    void viewAllUsers();
    void editEmployeeData();
    void calculateAndViewBonuses();
    void configureBonusFormula();

    string getHiddenPassword();
    void displayAllEmployees();

    vector<shared_ptr<Employee>>& getEmployees();
    vector<shared_ptr<User>>& getPendingRegistrations();

    vector<shared_ptr<Employee>> operator()(const string& dept) {
        vector<shared_ptr<Employee>> result;
        for (const auto& emp : employees) {
            if (emp->getDepartment() == dept) {
                result.push_back(emp);
            }
        }
        return result;
    }
};

#endif