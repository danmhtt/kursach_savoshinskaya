#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Date {
private:
    int day, month, year;
public:
    Date(int d = 1, int m = 1, int y = 2000);
    string toString() const;
    static Date fromString(const string& dateStr);
    int calculateExperience() const;
};

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
};

class User {
protected:
    string username, password, fullName, role;
    bool isApproved;
public:
    User(string uname = "", string pwd = "", string name = "", string r = "user", bool approved = false);
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
};

class Admin : public User {
public:
    Admin(string uname = "admin", string pwd = "admin123", string name = "Администратор Системы");
    void showMenu() override;
    string toFileString() const override;
};

class BonusSystem {
private:
    vector<User*> users;
    vector<Employee*> employees;
    vector<User*> pendingRegistrations;
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
    User* authenticate(const string& username, const string& password);
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
    vector<Employee*>& getEmployees();
    vector<User*>& getPendingRegistrations();
};

#endif