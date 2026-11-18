#include <iostream>
#include <cstring>
using namespace std;

class Student {
private:                       // Encapsulation: data is hidden
    char* name;                // Dynamically allocated
    int   rollNo;
    float marks;
    static int count;          // Shared by all objects

public:
    // Default constructor
    Student() : rollNo(0), marks(0.0f) {
        name = new char[8];
        strcpy(name, "Unknown");
        ++count;
    }

    // Parameterized constructor
    Student(const char* n, int r, float m) : rollNo(r), marks(m) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        ++count;
    }

    // Copy constructor (deep copy, needed because of dynamic memory)
    Student(const Student& other) : rollNo(other.rollNo), marks(other.marks) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        ++count;
    }

    // Copy assignment operator (deep copy)
    Student& operator=(const Student& other) {
        if (this != &other) {
            delete[] name;
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
            rollNo = other.rollNo;
            marks  = other.marks;
        }
        return *this;
    }

    // Destructor
    ~Student() {
        delete[] name;
        --count;
    }

    // Overloaded functions: setData
    void setData(int r, float m) {                 // update roll no and marks
        rollNo = r;
        marks  = m;
    }
    void setData(const char* n, int r, float m) {  // update everything
        delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        rollNo = r;
        marks  = m;
    }

    // Overloaded functions: display
    void display() const {
        cout << "Roll No: " << rollNo << ", Name: " << name
             << ", Marks: " << marks << endl;
    }
    void display(bool detailed) const {
        display();
        if (detailed)
            cout << "  Status: " << (marks >= 40 ? "Pass" : "Fail") << endl;
    }

    // Getters (controlled access)
    float getMarks() const { return marks; }

    // Static member function
    static int getCount() { return count; }

    // Overloaded > operator: compare by marks
    bool operator>(const Student& other) const {
        return marks > other.marks;
    }

    // Friend function: accesses private members directly
    friend void showGrade(const Student& s);
};

// Static member definition
int Student::count = 0;

// Friend function definition
void showGrade(const Student& s) {
    char grade;
    if      (s.marks >= 90) grade = 'A';
    else if (s.marks >= 75) grade = 'B';
    else if (s.marks >= 60) grade = 'C';
    else if (s.marks >= 40) grade = 'D';
    else                    grade = 'F';
    cout << s.name << " (Roll " << s.rollNo << ") -> Grade: " << grade << endl;
}

int main() {
    cout << "Students at start: " << Student::getCount() << "\n\n";

    Student s1("Asha", 101, 88.5f);
    Student s2("Ravi", 102, 72.0f);
    Student* s3 = new Student("Meera", 103, 95.0f);   // dynamic object

    cout << "--- Display (overloaded) ---\n";
    s1.display();
    s2.display(true);
    s3->display(true);

    cout << "\nTotal students: " << Student::getCount() << "\n";

    cout << "\n--- Comparison using overloaded > ---\n";
    if (s1 > s2) cout << "Asha has higher marks than Ravi\n";
    else         cout << "Ravi has higher marks than Asha\n";
    if (*s3 > s1) cout << "Meera has higher marks than Asha\n";

    cout << "\n--- Friend function ---\n";
    showGrade(s1);
    showGrade(s2);
    showGrade(*s3);

    cout << "\n--- Overloaded setData ---\n";
    s2.setData(102, 35.0f);
    s2.display(true);
    s2.setData("Ravi Kumar", 102, 80.0f);
    s2.display(true);

    Student s4 = s1;   // copy constructor
    cout << "\nAfter copy, total students: " << Student::getCount() << "\n";

    delete s3;         // frees memory, count decreases
    cout << "After deleting Meera, total students: " << Student::getCount() << "\n";

    return 0;
}
