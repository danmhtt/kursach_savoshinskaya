#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <iostream>
#include <limits>
using namespace std;

int getIntInput(const string& prompt, int min, int max);
double getDoubleInput(const string& prompt, double min, double max);

#endif