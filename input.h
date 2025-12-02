#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <iostream>
#include <limits>
using namespace std;

int getIntInput(const string& prompt, int min = INT_MIN, int max = INT_MAX);
double getDoubleInput(const string& prompt, double min = -DBL_MAX, double max = DBL_MAX);

#endif