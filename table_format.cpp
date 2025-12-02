#include "table_format.h"

string centerText(const string& text, int width) {
    if (text.length() >= width) return text.substr(0, width);
    int padding = width - text.length();
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;
    return string(leftPadding, ' ') + text + string(rightPadding, ' ');
}

vector<string> splitText(const string& text, int width) {
    vector<string> lines;
    stringstream ss(text);
    string word;
    string currentLine;

    while (ss >> word) {
        if (currentLine.empty()) {
            currentLine = word;
        }
        else if (currentLine.length() + word.length() + 1 <= width) {
            currentLine += " " + word;
        }
        else {
            lines.push_back(currentLine);
            currentLine = word;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

string formatDouble(double value, int width) {
    stringstream stream;
    stream << fixed << setprecision(2) << value << " BYN";
    string formattedValue = stream.str();

    if (formattedValue.length() > width) {
        return formattedValue.substr(0, width);
    }

    int padding = width - formattedValue.length();
    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;
    return string(leftPadding, ' ') + formattedValue + string(rightPadding, ' ');
}

void drawTableLine() {
    cout << "+-----+---------------------+---------------------+----------------------+-------+----------------+" << endl;
}

void drawTableHeader() {
    cout << "|  №  |        Логин        |        ФИО          |       Должность      |  KPI  |     Премия     |" << endl;
}