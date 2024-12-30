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
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <conio.h>

using namespace std;

struct Student {
    int id;
    string name;
    int age;
    float gpa;
    string email;
    string department;
    string yearOfStudy;
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
void deleteStudent(Student*& head, int id);
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
void saveToFile(Student* head, const string& filename);
void loadFromFile(Student*& head, const string& filename);
void generateTestCases(Student*& head, int count);
void cleanupMemory(Student*& head);
void displayAdminMenu();
void displayTeacherMenu();
void displayStudentMenu();
void displayMainMenu();
void handleAdminActions(Student*& head);
void handleTeacherActions(Student*& head);
void handleStudentActions(Student* head);
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

int main() {
    Student* head = nullptr;
    User currentUser;
    int choice;
    char loadChoice;

    clearConsole();
    cout << "\n=== System Initialization ===\n";
    cout << "Do you want to load previous data? (y/n): ";
    cin >> loadChoice;

    if (tolower(loadChoice) == 'y') {
        // Load existing data
        loadFromFile(head, "students.csv");
        cout << "Previous data loaded successfully!\n";
    }

    do {
        clearConsole();
        cout << "\n=== Student Management System ===\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (login(currentUser)) {
                    switch (currentUser.userType) {
                        case 1:
                            handleAdminActions(head);
                            break;
                        case 2:
                            handleTeacherActions(head);
                            break;
                        case 3:
                            handleStudentActions(head);
                            break;
                        default:
                            cout << "Invalid user type.\n";
                    }
                }
                break;
            
            case 2:
                cout << "\nThank you for using the system. Goodbye!\n";
                cleanupMemory(head);
                Sleep(1500);
                return 0;
            
            default:
                cout << "Invalid choice. Please try again.\n";
                Sleep(1000);
        }
    } while (true);

    return 0;
}

// Clear Console Function
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Display Main Menu
void displayMainMenu() {
    clearConsole(); // Clear the console only when displaying the menu
    cout << "\n--- Main Menu ---\n";
    cout << "1. Add Student\n";
    cout << "2. Delete Student\n";
    cout << "3. Search Student by ID\n";
    cout << "4. Search Student by Name\n";
    cout << "5. Update Student\n";
    cout << "6. Display Students\n";
    cout << "7. Sort Students\n";
    cout << "8. Filter Students\n";
    cout << "9. Save to File\n";
    cout << "10. Load from File\n";
    cout << "11. Generate Test Cases\n";
    cout << "12. Exit\n";
}

// Display Admin Menu
void displayAdminMenu() {
    clearConsole();
    cout << "\n--- Admin Menu ---\n";
    cout << "1. Student Management\n";
    cout << "2. Teacher Management\n";
    cout << "3. Sort Students\n";
    cout << "4. Filter Students\n";
    cout << "5. Generate Test Cases\n";
    cout << "6. Save to File\n";
    cout << "7. Load from File\n";
    cout << "8. Exit\n";
}

// Display Teacher Menu
void displayTeacherMenu() {
    clearConsole();
    cout << "\n=== Teacher Menu ===\n";
    cout << "1. View Students\n";
    cout << "2. Add Student\n";
    cout << "3. Update Student\n";
    cout << "4. Search Student\n";
    cout << "5. Filter Students\n";
    cout << "6. Sort Students\n";
    cout << "7. Save Changes\n";
    cout << "8. Exit\n";
}

// Display Student Menu
void displayStudentMenu() {
    clearConsole();
    cout << "\n=== Student Menu ===\n";
    cout << "1. View My Information\n";
    cout << "2. Update My Information\n";
    cout << "3. View My Grades\n";
    cout << "4. Exit\n";
}

// Handle Admin Actions
void handleAdminActions(Student*& head) {
    Teacher* teacherHead = nullptr;
    loadTeachers(teacherHead, "teachers.csv");
    int choice;
    
    do {
        displayAdminMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { // Student Management
                int studentChoice;
                do {
                    displayStudentManagementMenu();
                    cout << "Enter your choice: ";
                    cin >> studentChoice;

                    switch (studentChoice) {
                        case 1: 
                            addStudent(head); 
                            break;
                        case 2: {
                            int id;
                            cout << "Enter ID to delete: ";
                            cin >> id;
                            deleteStudent(head, id);
                            break;
                        }
                        case 3: {
                            int id;
                            cout << "Enter ID to search: ";
                            cin >> id;
                            if (Student* student = searchStudentByID(head, id))
                                cout << "Found: " << student->name << "\n";
                            else
                                cout << "Student not found.\n";
                            break;
                        }
                        case 4: {
                            string name;
                            cout << "Enter name to search: ";
                            cin.ignore();
                            getline(cin, name);
                            if (Student* student = searchStudentByName(head, name))
                                cout << "Found: " << student->name << "\n";
                            else
                                cout << "Student not found.\n";
                            break;
                        }
                        case 5: {
                            int id;
                            cout << "Enter ID to update: ";
                            cin >> id;
                            if (Student* student = searchStudentByID(head, id))
                                updateStudent(student);
                            else
                                cout << "Student not found.\n";
                            break;
                        }
                        case 6: 
                            displayStudents(head); 
                            break;
                        case 7: {
                            int sortChoice;
                            cout << "Sort by:\n1. Name\n2. GPA\n3. ID\n4. Age\nEnter your choice: ";
                            cin >> sortChoice;
                            bool ascending;
                            cout << "Sort in ascending order (1 for Yes, 0 for No): ";
                            cin >> ascending;
                            switch (sortChoice) {
                                case 1: sortStudentsByName(head, ascending); break;
                                case 2: sortStudentsByGPA(head, ascending); break;
                                case 3: sortStudentsByID(head, ascending); break;
                                case 4: sortStudentsByAge(head, ascending); break;
                                default: cout << "Invalid choice.\n"; break;
                            }
                            break;
                        }
                        case 8: {
                            int filterChoice;
                            cout << "Filter by:\n1. GPA range\n2. Age range\nEnter your choice: ";
                            cin >> filterChoice;
                            if (filterChoice == 1) {
                                float minGPA, maxGPA;
                                cout << "Enter minimum GPA: ";
                                cin >> minGPA;
                                cout << "Enter maximum GPA: ";
                                cin >> maxGPA;
                                filterStudentsByGPA(head, minGPA, maxGPA);
                            } else if (filterChoice == 2) {
                                int minAge, maxAge;
                                cout << "Enter minimum age: ";
                                cin >> minAge;
                                cout << "Enter maximum age: ";
                                cin >> maxAge;
                                filterStudentsByAge(head, minAge, maxAge);
                            }
                            break;
                        }
                        case 9:
                            int count;
                            cout << "Enter number of test cases to generate: ";
                            cin >> count;
                            generateTestCases(head, count);
                            break;
                        case 10:
                            cout << "Returning to main menu...\n";
                            break;
                        default:
                            cout << "Invalid choice. Try again.\n";
                    }
                    
                    if (studentChoice != 10) {
                        cout << "\nPress Enter to continue...";
                        cin.ignore();
                        cin.get();
                    }
                } while (studentChoice != 10);
                break;
            }
            case 2: { // Teacher Management
                int teacherChoice;
                cout << "\n=== Teacher Management ===\n";
                cout << "1. Add Teacher\n";
                cout << "2. Delete Teacher\n";
                cout << "3. Display Teachers\n";
                cout << "4. Back to Main Menu\n";
                cout << "Enter choice: ";
                cin >> teacherChoice;
                
                switch (teacherChoice) {
                    case 1:
                        addTeacher(teacherHead);
                        break;
                    case 2:
                        int id;
                        cout << "Enter teacher ID to delete: ";
                        cin >> id;
                        deleteTeacher(teacherHead, id);
                        break;
                    case 3:
                        displayTeachers(teacherHead);
                        break;
                }
                break;
            }
            case 3: { // Sort Students
                int sortChoice;
                cout << "Sort by:\n1. Name\n2. GPA\n3. ID\n4. Age\nEnter choice: ";
                cin >> sortChoice;
                bool ascending;
                cout << "Sort in ascending order (1 for Yes, 0 for No): ";
                cin >> ascending;
                switch (sortChoice) {
                    case 1: sortStudentsByName(head, ascending); break;
                    case 2: sortStudentsByGPA(head, ascending); break;
                    case 3: sortStudentsByID(head, ascending); break;
                    case 4: sortStudentsByAge(head, ascending); break;
                }
                break;
            }
            case 4: { // Filter Students
                int filterChoice;
                cout << "Filter by:\n1. GPA range\n2. Age range\nEnter choice: ";
                cin >> filterChoice;
                if (filterChoice == 1) {
                    float minGPA, maxGPA;
                    cout << "Enter minimum GPA: ";
                    cin >> minGPA;
                    cout << "Enter maximum GPA: ";
                    cin >> maxGPA;
                    filterStudentsByGPA(head, minGPA, maxGPA);
                } else if (filterChoice == 2) {
                    int minAge, maxAge;
                    cout << "Enter minimum age: ";
                    cin >> minAge;
                    cout << "Enter maximum age: ";
                    cin >> maxAge;
                    filterStudentsByAge(head, minAge, maxAge);
                }
                break;
            }
            case 5: { // Generate Test Cases
                int count;
                cout << "Enter number of test cases to generate: ";
                cin >> count;
                generateTestCases(head, count);
                break;
            }
            case 6: // Save to File
                saveToFile(head, "students.csv");
                saveTeachers(teacherHead, "teachers.csv");
                break;
            case 7: // Load from File
                loadFromFile(head, "students.csv");
                loadTeachers(teacherHead, "teachers.csv");
                break;
            case 8: // Exit
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
        
        if (choice != 8) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 8);
}

// Handle Teacher Actions
void handleTeacherActions(Student*& head) {
    int choice;
    do {
        displayTeacherMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // View Students
                displayStudents(head);
                break;
            case 2: // Add Student
                addStudent(head);
                saveToFile(head, "students.csv");
                break;
            case 3: { // Update Student
                int id;
                cout << "Enter student ID to update: ";
                cin >> id;
                if (Student* student = searchStudentByID(head, id)) {
                    updateStudent(student);
                    saveToFile(head, "students.csv");
                } else {
                    cout << "Student not found.\n";
                }
                break;
            }
            case 4: { // Search Student
                int searchChoice;
                cout << "\n1. Search by ID\n2. Search by Name\nEnter choice: ";
                cin >> searchChoice;
                if (searchChoice == 1) {
                    int id;
                    cout << "Enter ID to search: ";
                    cin >> id;
                    if (Student* student = searchStudentByID(head, id))
                        cout << "Found: " << student->name << "\n";
                    else
                        cout << "Student not found.\n";
                } else {
                    string name;
                    cout << "Enter name to search: ";
                    cin.ignore();
                    getline(cin, name);
                    if (Student* student = searchStudentByName(head, name))
                        cout << "Found: " << student->name << "\n";
                    else
                        cout << "Student not found.\n";
                }
                break;
            }
            case 5: { // Filter Students
                int filterChoice;
                cout << "Filter by:\n1. GPA range\n2. Age range\nEnter choice: ";
                cin >> filterChoice;
                if (filterChoice == 1) {
                    float minGPA, maxGPA;
                    cout << "Enter minimum GPA: ";
                    cin >> minGPA;
                    cout << "Enter maximum GPA: ";
                    cin >> maxGPA;
                    filterStudentsByGPA(head, minGPA, maxGPA);
                } else if (filterChoice == 2) {
                    int minAge, maxAge;
                    cout << "Enter minimum age: ";
                    cin >> minAge;
                    cout << "Enter maximum age: ";
                    cin >> maxAge;
                    filterStudentsByAge(head, minAge, maxAge);
                }
                break;
            }
            case 6: { // Sort Students
                int sortChoice;
                cout << "Sort by:\n1. Name\n2. GPA\n3. ID\n4. Age\nEnter choice: ";
                cin >> sortChoice;
                bool ascending;
                cout << "Sort in ascending order (1 for Yes, 0 for No): ";
                cin >> ascending;
                switch (sortChoice) {
                    case 1: sortStudentsByName(head, ascending); break;
                    case 2: sortStudentsByGPA(head, ascending); break;
                    case 3: sortStudentsByID(head, ascending); break;
                    case 4: sortStudentsByAge(head, ascending); break;
                }
                break;
            }
            case 7: // Save Changes
                saveToFile(head, "students.csv");
                cout << "Changes saved successfully!\n";
                break;
            case 8: // Exit
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
        
        if (choice != 8) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            clearConsole();
        }
    } while (choice != 8);
}

// Handle Student Actions
void handleStudentActions(Student* head) {
    int choice;
    string studentUsername = "student"; // يمكن تحديثه لاحقاً للحصول على اسم المستخدم الفعلي

    do {
        displayStudentMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { // View My Information
                Student* currentStudent = searchStudentByName(head, studentUsername);
                if (currentStudent) {
                    cout << "\n=== Your Information ===\n";
                    cout << "ID: " << currentStudent->id << "\n";
                    cout << "Name: " << currentStudent->name << "\n";
                    cout << "Age: " << currentStudent->age << "\n";
                    cout << "Email: " << currentStudent->email << "\n";
                    cout << "Department: " << currentStudent->department << "\n";
                    cout << "Year of Study: " << currentStudent->yearOfStudy << "\n";
                } else {
                    cout << "Error: Could not find your information.\n";
                }
                break;
            }
            case 2: { // Update My Information
                Student* currentStudent = searchStudentByName(head, studentUsername);
                if (currentStudent) {
                    cout << "\n=== Update Your Information ===\n";
                    cout << "Enter new email: ";
                    cin >> currentStudent->email;
                    cout << "Information updated successfully!\n";
                } else {
                    cout << "Error: Could not find your information.\n";
                }
                break;
            }
            case 3: { // View My Grades
                Student* currentStudent = searchStudentByName(head, studentUsername);
                if (currentStudent) {
                    cout << "\n=== Your Grades ===\n";
                    cout << "GPA: " << currentStudent->gpa << "\n";
                } else {
                    cout << "Error: Could not find your grades.\n";
                }
                break;
            }
            case 4: // Exit
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
        
        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            clearConsole();
        }
    } while (choice != 4);
}

// Login Function
bool login(User& user) {
    string username, password;
    int attempts = 3;
    
    while (attempts > 0) {
        clearConsole();  // Clear screen for security
        cout << "\n=== Login System ===\n";
        cout << "Attempts remaining: " << attempts << "\n\n";
        cout << "Enter username: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, username);
        
        cout << "Enter password: ";
        // Hide password while typing
        password = "";
        char ch;
        while ((ch = _getch()) != '\r') {  // Read until Enter is pressed
            if (ch == '\b') {  // Handle backspace
                if (password.length() > 0) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password += ch;
                cout << '*';
            }
        }
        cout << endl;

        int userType;
        if (validateUser(username, password, userType)) {
            user.username = username;
            user.password = password;
            user.userType = userType;
            
            clearConsole();
            cout << "\nLogin successful!\n";
            switch (userType) {
                case 1:
                    cout << "Welcome, Administrator!\n";
                    break;
                case 2:
                    cout << "Welcome, Teacher " << username << "!\n";
                    break;
                case 3:
                    cout << "Welcome, Student " << username << "!\n";
                    break;
            }
            Sleep(1500);  // Pause briefly to show welcome message
            return true;
        }
        
        attempts--;
        cout << "\nError: Invalid username or password.";
        if (attempts > 0) {
            cout << "\nPlease try again.\n";
            Sleep(1500);  // Add delay between attempts
        }
    }
    
    cout << "\nMaximum login attempts reached. Please try again later.\n";
    Sleep(2000);  // Pause before exiting
    return false;
}

// Input Validation Functions
bool isValidID(int id) {
    return id > 0; // ID must be positive
}

bool isValidAge(int age) {
    return age >= 18 && age <= 100; // Age must be between 18 and 100
}

bool isValidGPA(float gpa) {
    return gpa >= 0.0 && gpa <= 4.0; // GPA must be between 0.0 and 4.0
}

void clearInputBuffer() {
    cin.clear(); // Clear the error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
}

// Add Student
void addStudent(Student*& head) {
    Student* newStudent = new Student();
    
    cout << "\n=== Add New Student ===\n";
    cout << "Enter ID: ";
    while (!(cin >> newStudent->id) || !isValidID(newStudent->id)) {
        cout << "Invalid ID. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newStudent->name);
    
    cout << "Enter Age: ";
    cin >> newStudent->age;
    while (!isValidAge(newStudent->age)) {
        cout << "Invalid age. Please enter a valid age: ";
        cin >> newStudent->age;
    }

    cout << "Enter GPA: ";
    cin >> newStudent->gpa;
    while (!isValidGPA(newStudent->gpa)) {
        cout << "Invalid GPA. Please enter a valid GPA: ";
        cin >> newStudent->gpa;
    }

    cout << "Enter Email: ";
    cin >> newStudent->email;
    
    cout << "Enter Department: ";
    cin.ignore();
    getline(cin, newStudent->department);
    
    cout << "Enter Year of Study: ";
    getline(cin, newStudent->yearOfStudy);

    newStudent->next = nullptr;
    
    // Add to linked list
    if (!head) {
        head = newStudent;
    } else {
        Student* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newStudent;
    }

    // Save to CSV file
    saveToFile(head, "students.csv");
    cout << "Student added successfully and saved to file!\n";
}

// Delete Student
void deleteStudent(Student*& head, int id) {
    if (!head) {
        cout << "List is empty.\n";
        return;
    }

    bool found = false;
    if (head->id == id) {
        Student* temp = head;
        head = head->next;
        delete temp;
        found = true;
    } else {
        Student* temp = head;
        while (temp->next && temp->next->id != id)
            temp = temp->next;
        if (temp->next) {
            Student* toDelete = temp->next;
            temp->next = toDelete->next;
            delete toDelete;
            found = true;
        }
    }

    if (found) {
        // Update CSV file after deletion
        saveToFile(head, "students.csv");
        cout << "Student deleted successfully and file updated.\n";
    } else {
        cout << "Student not found.\n";
    }
}

// Search by ID
Student* searchStudentByID(Student* head, int id) {
    while (head) {
        if (head->id == id)
            return head;
        head = head->next;
    }
    return nullptr;
}

// Search by Name (Case-Insensitive)
Student* searchStudentByName(Student* head, const string& name) {
    while (head) {
        string studentName = head->name;
        transform(studentName.begin(), studentName.end(), studentName.begin(), ::tolower);
        string searchName = name;
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        if (studentName.find(searchName) != string::npos)
            return head;
        head = head->next;
    }
    return nullptr;
}

// Update Student
void updateStudent(Student* student) {
    cout << "Updating Student: " << student->name << "\n";
    cout << "Enter new Name: ";
    cin.ignore();
    getline(cin, student->name);
    cout << "Enter new Age: ";
    cin >> student->age;
    while (!isValidAge(student->age)) {
        cout << "Invalid age. Please enter a valid age: ";
        cin >> student->age;
    }
    cout << "Enter new GPA: ";
    cin >> student->gpa;
    while (!isValidGPA(student->gpa)) {
        cout << "Invalid GPA. Please enter a valid GPA: ";
        cin >> student->gpa;
    }
    cout << "Enter new Email: ";
    cin >> student->email;
    cout << "Enter new Department: ";
    cin.ignore();
    getline(cin, student->department);
    cout << "Enter new Year of Study: ";
    getline(cin, student->yearOfStudy);
    cout << "Student updated successfully.\n";
}

// Display Students
void displayStudents(Student* head) {
    if (!head) {
        cout << "No students to display.\n";
        return;
    }
    while (head) {
        cout << "ID: " << head->id << ", Name: " << head->name << ", Age: " << head->age
             << ", GPA: " << head->gpa << ", Email: " << head->email
             << ", Department: " << head->department << ", Year: " << head->yearOfStudy << "\n";
        head = head->next;
    }
}

// Sort by Name
void sortStudentsByName(Student*& head, bool ascending) {
    if (!head || !head->next) return;
    for (Student* i = head; i; i = i->next) {
        for (Student* j = i->next; j; j = j->next) {
            if ((ascending && i->name > j->name) || (!ascending && i->name < j->name)) {
                swap(i->id, j->id);
                swap(i->name, j->name);
                swap(i->age, j->age);
                swap(i->gpa, j->gpa);
                swap(i->email, j->email);
                swap(i->department, j->department);
                swap(i->yearOfStudy, j->yearOfStudy);
            }
        }
    }
}

// Sort by GPA
void sortStudentsByGPA(Student*& head, bool ascending) {
    if (!head || !head->next) return;
    for (Student* i = head; i; i = i->next) {
        for (Student* j = i->next; j; j = j->next) {
            if ((ascending && i->gpa > j->gpa) || (!ascending && i->gpa < j->gpa)) {
                swap(i->id, j->id);
                swap(i->name, j->name);
                swap(i->age, j->age);
                swap(i->gpa, j->gpa);
                swap(i->email, j->email);
                swap(i->department, j->department);
                swap(i->yearOfStudy, j->yearOfStudy);
            }
        }
    }
}

// Sort by ID
void sortStudentsByID(Student*& head, bool ascending) {
    if (!head || !head->next) return;
    for (Student* i = head; i; i = i->next) {
        for (Student* j = i->next; j; j = j->next) {
            if ((ascending && i->id > j->id) || (!ascending && i->id < j->id)) {
                swap(i->id, j->id);
                swap(i->name, j->name);
                swap(i->age, j->age);
                swap(i->gpa, j->gpa);
                swap(i->email, j->email);
                swap(i->department, j->department);
                swap(i->yearOfStudy, j->yearOfStudy);
            }
        }
    }
}

// Sort by Age
void sortStudentsByAge(Student*& head, bool ascending) {
    if (!head || !head->next) return;
    for (Student* i = head; i; i = i->next) {
        for (Student* j = i->next; j; j = j->next) {
            if ((ascending && i->age > j->age) || (!ascending && i->age < j->age)) {
                swap(i->id, j->id);
                swap(i->name, j->name);
                swap(i->age, j->age);
                swap(i->gpa, j->gpa);
                swap(i->email, j->email);
                swap(i->department, j->department);
                swap(i->yearOfStudy, j->yearOfStudy);
            }
        }
    }
}

// Filter by GPA Range
void filterStudentsByGPA(Student* head, float minGPA, float maxGPA) {
    bool found = false;
    while (head) {
        if (head->gpa >= minGPA && head->gpa <= maxGPA) {
            cout << "ID: " << head->id << ", Name: " << head->name << ", GPA: " << head->gpa << "\n";
            found = true;
        }
        head = head->next;
    }
    if (!found) {
        cout << "No students found in the specified GPA range.\n";
    }
}

// Filter by Age Range
void filterStudentsByAge(Student* head, int minAge, int maxAge) {
    bool found = false;
    while (head) {
        if (head->age >= minAge && head->age <= maxAge) {
            cout << "ID: " << head->id << ", Name: " << head->name << ", Age: " << head->age << "\n";
            found = true;
        }
        head = head->next;
    }
    if (!found) {
        cout << "No students found in the specified age range.\n";
    }
}

// Save to File
void saveToFile(Student* head, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening file.\n";
        return;
    }
    while (head) {
        file << head->id << "," << head->name << "," << head->age << ","
             << head->gpa << "," << head->email << "," << head->department << "," << head->yearOfStudy << "\n";
        head = head->next;
    }
    file.close();
    cout << "Data saved to " << filename << " successfully.\n";
}

// Load from File
void loadFromFile(Student*& head, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file.\n";
        return;
    }
    head = nullptr;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Student* newStudent = new Student();
        string temp;
        getline(ss, temp, ','); newStudent->id = stoi(temp);
        getline(ss, newStudent->name, ',');
        getline(ss, temp, ','); newStudent->age = stoi(temp);
        getline(ss, temp, ','); newStudent->gpa = stof(temp);
        getline(ss, newStudent->email, ',');
        getline(ss, newStudent->department, ',');
        getline(ss, newStudent->yearOfStudy, ',');
        newStudent->next = nullptr;

        if (!head) {
            head = newStudent;
        } else {
            Student* tempHead = head;
            while (tempHead->next) {
                tempHead = tempHead->next;
            }
            tempHead->next = newStudent;
        }
    }
    file.close();
    cout << "Data loaded from " << filename << " successfully.\n";
}

// Function to delete all students in the list
void deleteAllStudents(Student*& head) {
    while (head) {
        Student* temp = head;
        head = head->next;
        delete temp;
    }
    
    // Clear the CSV file
    ofstream file("students.csv", ios::trunc);
    file.close();
    
    cout << "All students deleted and file cleared.\n";
}

// Generate Test Cases
void generateTestCases(Student*& head, int count) {
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < count; ++i) {
        Student* newStudent = new Student();
        newStudent->id = i + 1;
        newStudent->name = "Student" + to_string(i + 1);
        newStudent->age = rand() % 10 + 18; // Age between 18 and 27
        newStudent->gpa = static_cast<float>(rand() % 401) / 100; // GPA between 0.0 and 4.0
        newStudent->email = "student" + to_string(i + 1) + "@example.com";
        newStudent->department = "Department" + to_string(rand() % 5 + 1); // 5 departments
        newStudent->yearOfStudy = "Year " + to_string(rand() % 4 + 1); // Random year of study
        newStudent->next = head;
        head = newStudent;
    }
    cout << count << " test cases generated successfully.\n";

    // Display all generated test cases
    displayStudents(head); // Call the display function to show all students
}

// Cleanup Memory
void cleanupMemory(Student*& head) {
    while (head) {
        Student* temp = head;
        head = head->next;
        delete temp;
    }
    cout << "Memory cleaned up successfully.\n";
}

bool validateUser(const string& username, const string& password, int& userType) {
    // Admin credentials only as default
    if (username == "admin" && password == "admin123") {
        userType = 1;
        return true;
    }
    
    // Check teacher credentials from file
    ifstream teacherFile("teachers.csv");
    if (teacherFile.is_open()) {
        string line;
        while (getline(teacherFile, line)) {
            stringstream ss(line);
            string id, name, email, department, storedPassword;
            
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, department, ',');
            getline(ss, storedPassword, ',');
            
            email = trim(email);
            storedPassword = trim(storedPassword);
            
            if (email == username && storedPassword == password) {
                userType = 2;
                teacherFile.close();
                return true;
            }
        }
        teacherFile.close();
    }
    
    // Check student credentials from file
    ifstream studentFile("students.csv");
    if (studentFile.is_open()) {
        string line;
        while (getline(studentFile, line)) {
            stringstream ss(line);
            string id, name, email, password;
            
            // Read until we get to the email field
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, email, ',');
            
            email = trim(email);
            
            // For students, use email as both username and password
            if (email == username && email == password) {
                userType = 3;
                studentFile.close();
                return true;
            }
        }
        studentFile.close();
    }
    
    return false;
}

// Add this helper function for string trimming
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void addTeacher(Teacher*& head) {
    Teacher* newTeacher = new Teacher();
    
    cout << "\n=== Add New Teacher ===\n";
    cout << "Enter Teacher ID: ";
    cin >> newTeacher->id;
    
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newTeacher->name);
    
    cout << "Enter Email: ";
    getline(cin, newTeacher->email);
    
    cout << "Enter Department: ";
    getline(cin, newTeacher->department);
    
    cout << "Set Password: ";
    getline(cin, newTeacher->password);
    
    newTeacher->next = head;
    head = newTeacher;
    
    // Save to file
    saveTeachers(head, "teachers.csv");
    cout << "Teacher added successfully!\n";
}

void deleteTeacher(Teacher*& head, int id) {
    if (!head) {
        cout << "No teachers in the system.\n";
        return;
    }

    bool found = false;
    if (head->id == id) {
        Teacher* temp = head;
        head = head->next;
        delete temp;
        found = true;
    } else {
        Teacher* current = head;
        while (current->next && current->next->id != id) {
            current = current->next;
        }
        if (current->next) {
            Teacher* temp = current->next;
            current->next = temp->next;
            delete temp;
            found = true;
        }
    }

    if (found) {
        // Update CSV file after deletion
        saveTeachers(head, "teachers.csv");
        cout << "Teacher deleted successfully and file updated.\n";
    } else {
        cout << "Teacher not found.\n";
    }
}

void displayTeachers(Teacher* head) {
    if (!head) {
        cout << "No teachers in the system.\n";
        return;
    }
    
    cout << "\n=== Teachers List ===\n";
    while (head) {
        cout << "ID: " << head->id 
             << ", Name: " << head->name 
             << ", Email: " << head->email 
             << ", Department: " << head->department << "\n";
        head = head->next;
    }
}

void saveTeachers(Teacher* head, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening file for saving teachers.\n";
        return;
    }
    
    while (head) {
        file << head->id << "," 
             << head->name << "," 
             << head->email << "," 
             << head->department << "," 
             << head->password << "\n";
        head = head->next;
    }
    file.close();
}

void loadTeachers(Teacher*& head, const string& filename) {
    ifstream file(filename);
    if (!file) {
        return; // Silent return if file doesn't exist
    }
    
    // Clear existing list
    while (head) {
        Teacher* temp = head;
        head = head->next;
        delete temp;
    }
    
    string line;
    while (getline(file, line)) {
        Teacher* newTeacher = new Teacher();
        stringstream ss(line);
        string temp;
        
        getline(ss, temp, ',');
        newTeacher->id = stoi(temp);
        getline(ss, newTeacher->name, ',');
        getline(ss, newTeacher->email, ',');
        getline(ss, newTeacher->department, ',');
        getline(ss, newTeacher->password, ',');
        
        newTeacher->next = head;
        head = newTeacher;
    }
    file.close();
}

// Add new function for student management menu
void displayStudentManagementMenu() {
    clearConsole();
    cout << "\n--- Student Management Menu ---\n";
    cout << "1. Add Student\n";
    cout << "2. Delete Student\n";
    cout << "3. Search Student by ID\n";
    cout << "4. Search Student by Name\n";
    cout << "5. Update Student\n";
    cout << "6. Display All Students\n";
    cout << "7. Sort Students\n";
    cout << "8. Filter Students\n";
    cout << "9. Generate Test Cases\n";
    cout << "10. Back to Main Menu\n";
}

void initializeDefaultTeachers() {
    Teacher* head = nullptr;
    
    // Create default teachers
    vector<Teacher> defaultTeachers = {
        {1, "John Smith", "john", "Computer Science", "pass123"},
        {2, "Mary Johnson", "mary", "Mathematics", "pass456"},
        {3, "David Wilson", "david", "Physics", "pass789"}
    };
    
    for (const auto& teacher : defaultTeachers) {
        Teacher* newTeacher = new Teacher();
        newTeacher->id = teacher.id;
        newTeacher->name = teacher.name;
        newTeacher->email = teacher.email;
        newTeacher->department = teacher.department;
        newTeacher->password = teacher.password;
        newTeacher->next = head;
        head = newTeacher;
    }
    
    // Save to file
    saveTeachers(head, "teachers.csv");
    
    // Cleanup
    while (head) {
        Teacher* temp = head;
        head = head->next;
        delete temp;
    }
}
