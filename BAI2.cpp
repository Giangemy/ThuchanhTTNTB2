#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <stack>
#include <map>
#include <cctype>

using namespace std;

bool evaluate(bool a, bool b, char op) {
    switch (op) {
        case '&': return a && b; // AND
        case '|': return a || b; // OR
        case '>': return (!a || b); // keo theo (A -> B tuong duong !A || B)
        default: return false; 
    }
}

//ham kiem tra xem mot ki tu co phai là mot toan tu hay k
bool isOperator(char c) {
    return (c == '&' || c == '|' || c == '>');
}

//ham kiem tra ki tu hop le cho bien
bool isValidVariable(char c) {
    return isalpha(c);
}

bool evaluateExpression(const string& expression, const map<char, bool>& values) {
    stack<bool> operands;
    stack<char> operators;

    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (isspace(c)) continue;

        if (isValidVariable(c)) {
            if (values.find(c) == values.end()) {
                cerr << "Error: Variable " << c << " not found in input values." << endl;
                return false;
            }
            operands.push(values.at(c));
        } else if (isOperator(c)) {
            operators.push(c);
        } else if (c == '(') {
            operators.push(c);
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                if (operands.size() < 2) {
                    cerr << "Error: Invalid expression (too few operands)." << endl;
                    return false;
                }
                bool operand2 = operands.top(); operands.pop();
                bool operand1 = operands.top(); operands.pop();
                char op = operators.top(); operators.pop();
                operands.push(evaluate(operand1, operand2, op));
            }
            if (!operators.empty() && operators.top() == '(') {
                operators.pop(); 
            } else {
                cerr << "Error: Unmatched parentheses." << endl;
                return false;
            }
        } else if (c == '!') { 
            if (i + 1 < expression.length() && isValidVariable(expression[i+1])) {
                if (values.find(expression[i+1]) == values.end()) {
                    cerr << "Error: Variable " << expression[i+1] << " not found in input values." << endl;
                    return false; 
                }
                operands.push(!values.at(expression[i+1]));
                i++; 
            } else {
                cerr << "Error: Invalid NOT operator usage." << endl;
                return false;
            }
        } else {
            cerr << "Error: Invalid character '" << c << "' in expression." << endl;
            return false;
        }
    }

    while (!operators.empty()) {
        if (operands.size() < 2) {
            cerr << "Error: Invalid expression (too few operands)." << endl;
            return false;
        }
        bool operand2 = operands.top(); operands.pop();
        bool operand1 = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();
        operands.push(evaluate(operand1, operand2, op));
    }

    if (operands.size() != 1) {
        cerr << "Error: Invalid expression." << endl;
        return false;
    }

    return operands.top();
}

int main() {
    string expression;
    cout << "Nhap bieu thuc logic: ";
    getline(cin, expression);

    set<char> variables;
    for (char c : expression) {
        if (isValidVariable(c)) {
            variables.insert(c);
        }
    }

    int numVars = variables.size();
    vector<char> varList(variables.begin(), variables.end());
    sort(varList.begin(), varList.end()); 

    cout << "Bang chan li:\n";
    for (char var : varList) {
        cout << var << " ";
    }
    cout << "| Ket qua\n";
    string separator(numVars * 2 + 9, '-'); 
    cout << separator << endl;

    for (int i = 0; i < pow(2, numVars); ++i) {
        map<char, bool> values;
        for (int j = 0; j < numVars; ++j) {
            values[varList[j]] = (i >> j) & 1; 
            cout << (values[varList[j]] ? 'T' : 'F') << " ";
        }
        cout << "|   ";
        if (evaluateExpression(expression, values)) {
            cout << 'T' << endl;
        } else {
            cout << 'F' << endl;
        }
    }

    return 0;
}

