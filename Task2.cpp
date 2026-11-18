#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
using namespace std;

struct InvalidMarks : exception {
    const char* what() const noexcept override { return "Invalid marks (must be 0-100)"; }
};

class Student {
protected:
    int roll; string name;
public:
    Student(int r, string n) : roll(r), name(n) {}
    virtual ~Student() = default;
    virtual void display() const { cout << roll << " " << name << endl; }
    virtual double total() const { return 0; }
    virtual string record() const { return to_string(roll) + "," + name; }
    const string& getName() const { return name; }
};

class Result : public Student {
    vector<double> marks;
public:
    Result(int r, string n, vector<double> m) : Student(r, n), marks(m) {
        if (any_of(m.begin(), m.end(), [](double x) { return x < 0 || x > 100; })) throw InvalidMarks();
    }
    double total() const override { return accumulate(marks.begin(), marks.end(), 0.0); }
    void display() const override {
        Student::display();
        cout << "   Total: " << total() << "  " << (all_of(marks.begin(), marks.end(),
            [](double x) { return x >= 40; }) ? "PASS" : "FAIL") << endl;
    }
    string record() const override {
        string s = Student::record();
        for (double m : marks) s += "," + to_string(m);
        return s;
    }
};

template <typename T>   // prints any container of student pointers
void showAll(const T& c) { for (auto& s : c) s->display(); }

int main() {
    vector<shared_ptr<Student>> v;
    auto add = [&](int r, string n, vector<double> m) {
        try { v.push_back(make_shared<Result>(r, n, m)); }
        catch (const exception& e) { cout << "Skipped " << n << ": " << e.what() << endl; }
    };
    add(101, "Asha", {88, 92, 79});
    add(102, "Ravi", {55, 38, 61});
    add(103, "Meera", {95, 98, 91});
    add(104, "Kiran", {72, 150, 68});   // exception

    ofstream("students.txt") << [&] { string s; for (auto& p : v) s += p->record() + "\n"; return s; }();

    v.clear();                           // reload from file
    ifstream in("students.txt");
    string line;
    while (getline(in, line)) {
        stringstream ss(line); string t; vector<string> p;
        while (getline(ss, t, ',')) p.push_back(t);
        add(stoi(p[0]), p[1], {stod(p[2]), stod(p[3]), stod(p[4])});
    }
    cout << "Loaded " << v.size() << " records\n";
    showAll(v);

    double limit = 220;
    cout << "\nTotal above " << limit << ":\n";
    for_each(v.begin(), v.end(), [&](auto& s) {
        if (s->total() > limit) cout << "  " << s->getName() << " - " << s->total() << endl;
    });
}
