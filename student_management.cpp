#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;


struct Student {
    int    id;
    string name;
    int    age;
    string course;
    double gpa;
};


const string FILE_NAME = "students.txt";

void saveToFile(const vector<Student>& students) {
    ofstream file(FILE_NAME);
    for (const auto& s : students)
        file << s.id << "|" << s.name << "|" << s.age << "|"
             << s.course << "|" << s.gpa << "\n";
}

vector<Student> loadFromFile() {
    vector<Student> students;
    ifstream file(FILE_NAME);
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        Student s;
        getline(ss, token, '|'); s.id     = stoi(token);
        getline(ss, s.name,   '|');
        getline(ss, token,    '|'); s.age    = stoi(token);
        getline(ss, s.course, '|');
        getline(ss, token,    '|'); s.gpa    = stod(token);
        students.push_back(s);
    }
    return students;
}


void printHeader() {
    cout << "\n"
         << string(58, '=') << "\n"
         << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(6)  << "Age"
         << setw(18) << "Course"
         << "GPA\n"
         << string(58, '-') << "\n";
}

void printStudent(const Student& s) {
    cout << left
         << setw(6)  << s.id
         << setw(20) << s.name
         << setw(6)  << s.age
         << setw(18) << s.course
         << fixed << setprecision(2) << s.gpa << "\n";
}

void displayAll(const vector<Student>& students) {
    if (students.empty()) { cout << "  No records found.\n"; return; }
    printHeader();
    for (const auto& s : students) printStudent(s);
    cout << string(58, '=') << "\n";
}


int nextId(const vector<Student>& v) {
    if (v.empty()) return 1;
    return max_element(v.begin(), v.end(),
        [](const Student& a, const Student& b){ return a.id < b.id; })->id + 1;
}


void addStudent(vector<Student>& students) {
    Student s;
    s.id = nextId(students);
    cout << "\n  --- Add Student ---\n";
    cout << "  Name   : "; cin.ignore(); getline(cin, s.name);
    cout << "  Age    : "; cin >> s.age;
    cout << "  Course : "; cin.ignore(); getline(cin, s.course);
    cout << "  GPA    : "; cin >> s.gpa;
    students.push_back(s);
    saveToFile(students);
    cout << "  Student added (ID " << s.id << ").\n";
}

void updateStudent(vector<Student>& students) {
    int id;
    cout << "\n  Enter student ID to update: "; cin >> id;
    for (auto& s : students) {
        if (s.id == id) {
            cout << "  New name   : "; cin.ignore(); getline(cin, s.name);
            cout << "  New age    : "; cin >> s.age;
            cout << "  New course : "; cin.ignore(); getline(cin, s.course);
            cout << "  New GPA    : "; cin >> s.gpa;
            saveToFile(students);
            cout << "  Record updated.\n";
            return;
        }
    }
    cout << "  ID not found.\n";
}

void deleteStudent(vector<Student>& students) {
    int id;
    cout << "\n  Enter student ID to delete: "; cin >> id;
    auto it = remove_if(students.begin(), students.end(),
                        [id](const Student& s){ return s.id == id; });
    if (it == students.end()) { cout << "  ID not found.\n"; return; }
    students.erase(it, students.end());
    saveToFile(students);
    cout << "  Student deleted.\n";
}

void searchStudent(const vector<Student>& students) {
    string query;
    cout << "\n  Search (name or ID): "; cin.ignore(); getline(cin, query);
    printHeader();
    bool found = false;
    for (const auto& s : students) {
        string low = s.name;
        transform(low.begin(), low.end(), low.begin(), ::tolower);
        string q   = query;
        transform(q.begin(),   q.end(),   q.begin(),   ::tolower);
        if (low.find(q) != string::npos || to_string(s.id) == query) {
            printStudent(s); found = true;
        }
    }
    if (!found) cout << "  No match found.\n";
    else cout << string(58, '=') << "\n";
}


void showMenu() {
    cout << "\n  ╔══════════════════════════════╗\n"
         << "  ║  Student Management System  ║\n"
         << "  ╠══════════════════════════════╣\n"
         << "  ║  1. Add Student              ║\n"
         << "  ║  2. Display All Students     ║\n"
         << "  ║  3. Search Student           ║\n"
         << "  ║  4. Update Student           ║\n"
         << "  ║  5. Delete Student           ║\n"
         << "  ║  6. Exit                     ║\n"
         << "  ╚══════════════════════════════╝\n"
         << "  Choice: ";
}

int main() {
    vector<Student> students = loadFromFile();
    int choice;
    do {
        showMenu();
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Try again: ";
        }
        switch (choice) {
            case 1: addStudent(students);     break;
            case 2: displayAll(students);     break;
            case 3: searchStudent(students);  break;
            case 4: updateStudent(students);  break;
            case 5: deleteStudent(students);  break;
            case 6: cout << "  Goodbye!\n";       break;
            default: cout << "  Invalid choice.\n";
        }
    } while (choice != 6);
    return 0;
}