#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <limits>
#include <vector>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <conio.h>

using namespace std;


struct Course {
    string name;
    float degree; // Assuming degree is a float
};


struct Student {
    int id;
    string name;
    int age;
    float gpa;
    string email;
    string password;
    string department;
    string yearOfStudy;
    vector<Course> courses;
    Student* next;
    
};

struct User {
    string username;
    string password;
    int userType; // 1 for Admin, 2 for Teacher, 3 for Student

};

struct Teacher {
    int id;
    string name;
    string email;
    string department;
    string password;
    Teacher* next;

};

// Function Prototypes
void addStudent(Student*& head);
Student* searchStudentByID(Student* head, int id);
Student* searchStudentByName(Student* head, const string& name);
void updateStudent(Student* student);
void displayStudents(Student* head);
void sortStudentsByName(Student*& head, bool ascending);
void sortStudentsByGPA(Student*& head, bool ascending);
void sortStudentsByID(Student*& head, bool ascending);
void sortStudentsByAge(Student*& head, bool ascending);
void filterStudentsByGPA(Student* head, float minGPA, float maxGPA);
void filterStudentsByAge(Student* head, int minAge, int maxAge);
void saveToFile(Student* head, const string& filename, bool includePassword);
void loadFromFile(Student*& head, const string& filename);
void generateTestCases(Student*& head, int count);
void cleanupMemory(Student*& head);
void displayAdminMenu();
void displayTeacherMenu();
void displayStudentMenu();
void displayMainMenu();
void handleAdminActions(Student*& head);
void handleTeacherActions(Student*& head);
void handleStudentActions(Student* head, User& currentUser);
bool isValidID(int id);
bool isValidAge(int age);
bool isValidGPA(float gpa);
void clearInputBuffer();
bool login(User& user);
void clearConsole();
void deleteAllStudents(Student*& head);
void addTeacher(Teacher*& head);
void deleteTeacher(Teacher*& head, int id);
void displayTeachers(Teacher* head);
bool validateUser(const string& username, const string& password, int& userType);
void saveTeachers(Teacher* head, const string& filename);
void loadTeachers(Teacher*& head, const string& filename);
void displayStudentManagementMenu();
void initializeDefaultTeachers();
string trim(const string& str);
void sortStudents(Student*& head) ;
void addCourseWithDegree(Student* student);
void displayCoursesAndDegrees(Student* student);
void removeCourse(Student* student);
void saveCourses(Student* head, const string& filename);
void loadCourses(Student*& head, const string& filename);
void displayCourseManagementMenu();
void manageCourseAndDegrees(Student*& head);
float calculateGPA(const vector<Course>& courses);
void loadCoursesForStudents(Student* head, const string& filename);
void deleteStudent(Student*& head, int id);
#endif
