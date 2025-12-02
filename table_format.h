#ifndef TABLE_FORMAT_H
#define TABLE_FORMAT_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

string centerText(const string& text, int width);
vector<string> splitText(const string& text, int width);
string formatDouble(double value, int width);
void drawTableLine();
void drawTableHeader();

#endif