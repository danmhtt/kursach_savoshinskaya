#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <iostream>
using namespace std;

string toLowerRussian(const string& str);
bool equalsIgnoreCase(const string& str1, const string& str2);
bool isValidName(const string& name);
bool isValidYear(int year);
bool isValidMonth(int month);
bool isValidDay(int day, int month, int year);
bool isValidSalary(double salary);
bool isValidKPI(double kpi);
bool isValidCoefficient(double coeff);
bool isValidUsername(const string& username);
bool isValidPassword(const string& password);
bool isValidDepartment(const string& department);
bool isValidPosition(const string& position);

#endif