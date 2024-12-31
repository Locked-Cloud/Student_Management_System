#include "main.h"


bool autoSaveEnabled = true; // Default to enabled


int main() {
    Student* head = nullptr;
    Teacher* teacherHead = nullptr;
    User currentUser;
    int choice;
    char loadChoice;

    clearConsole();
    cout << "\n=== System Initialization ===\n";
    cout << "Do you want to load previous data? (y/n): ";
    cin >> loadChoice;

    // Validate load choice
    while (tolower(loadChoice) != 'y' && tolower(loadChoice) != 'n') {
        cout << "Invalid choice. Please enter 'y' or 'n': ";
        cin >> loadChoice;
    }

    if (tolower(loadChoice) == 'y') {
        loadFromFile(head, "students.csv");
        loadCoursesForStudents(head, "course_enrollments.csv");
        loadTeachers(teacherHead, "teachers.csv");
        cout << "Previous data loaded successfully!\n";
    }

    do {
        clearConsole();
        cout << "\n=== Student Management System ===\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Validate main menu choice
        while (cin.fail() || choice < 1 || choice > 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1 or 2: ";
            cin >> choice;
        }

        switch (choice) {
            case 1:
                if (login(currentUser)) {
                    // Use currentUser directly
                    switch (currentUser.userType) {
                        case 1:
                            handleAdminActions(head);
                            break;
                        case 2:
                            handleTeacherActions(head);
                            break;
                        case 3:
                            handleStudentActions(head, currentUser); // Pass currentUser
                            break;
                        default:
                            cout << "Invalid user type.\n";
                    }
                }
                break;
            
            case 2:
                cout << "\nThank you for using the system. Goodbye!\n";
                cleanupMemory(head);
                return 0;
            
            default:
                cout << "Invalid choice. Please try again.\n";
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
    cout << "3. Toggle Auto-Save\n";
    cout << "4. Sort Students\n";
    cout << "5. Filter Students\n";
    cout << "6. Generate Test Cases\n";
    cout << "7. Save to File\n";
    cout << "8. Load from File\n";
    cout << "9. Exit\n";
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
    cout << "7. Manage Student Courses and Grades\n";
    cout << "8. View Student GPAs\n";
    cout << "9. Save Changes\n";
    cout << "10. Exit\n";
}

// Display Student Menu
void displayStudentMenu() {
    clearConsole();
    cout << "\n=== Student Menu ===\n";
    cout << "1. View My Information\n";
    cout << "2. View My Courses\n";
    cout << "3. View My Grades and GPA\n";
    cout << "4. Exit\n";
    cout << "Enter your choice (1-4): ";
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

        // Validate input
        while (cin.fail() || choice < 1 || choice > 9) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 9.\n";
            cin >> choice;
        }

        switch (choice) {
            case 1: { // Student Management
                int studentChoice;
                do {
                    displayStudentManagementMenu();
                    cout << "Enter your choice: ";
                    cin >> studentChoice;

                    // Validate input
                    while (cin.fail() || studentChoice < 1 || studentChoice > 11) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number between 1 and 11.\n";
                        cin >> studentChoice;
                    }

                    switch (studentChoice) {
                        case 1: 
                            addStudent(head); 
                            if (autoSaveEnabled) {
                                saveToFile(head, "students.csv", true); // Auto-save if enabled
                            }
                            break;
                        case 2: {
                            int id;
                            cout << "Enter ID to delete: ";
                            cin >> id;
                            deleteStudent(head, id);
                            if (autoSaveEnabled) {
                                saveToFile(head, "students.csv", true); // Auto-save if enabled
                            }
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
                        case 7: { // Sort Students
                            sortStudents(head);
                            break;
                        }
                        case 8: { // Filter Students
                            int filterChoice;
                            cout << "Filter by:\n1. GPA range\n2. Age range\nEnter choice: ";
                            cin >> filterChoice;

                            // Validate input
                            while (cin.fail() || (filterChoice != 1 && filterChoice != 2)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid input. Please enter 1 or 2.\n";
                                cin >> filterChoice;
                            }

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
                        case 9: // Generate Test Cases
                            int count;
                            cout << "Enter number of test cases to generate: ";
                            cin >> count;
                            generateTestCases(head, count);
                            break;
                        case 10: // Manage Courses and Degrees
                            manageCourseAndDegrees(head);
                            break;
                        case 11: // Back to Admin Menu
                            cout << "Returning to main menu...\n";
                            break;
                        default:
                            cout << "Invalid choice. Try again.\n";
                    }
                    
                    if (studentChoice != 11) {
                        cout << "\nPress Enter to continue...";
                        cin.ignore();
                        cin.get();
                    }
                } while (studentChoice != 11);
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
                
                // Validate input
                while (cin.fail() || teacherChoice < 1 || teacherChoice > 4) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number between 1 and 4.\n";
                    cin >> teacherChoice;
                }

                switch (teacherChoice) {
                    case 1:
                        addTeacher(teacherHead);
                        break;
                    case 2:
                        int id;
                        cout << "Enter teacher ID to delete: ";
                        cin >> id;
                        deleteTeacher(teacherHead, id);
                        if (autoSaveEnabled) {
                            saveTeachers(teacherHead, "teachers.csv"); // Auto-save if enabled
                        }
                        break;
                    case 3:
                        displayTeachers(teacherHead);
                        break;
                    case 4:
                        cout << "Returning to Admin Menu...\n";
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                }
                break;
            }
            case 3: { // Toggle Auto-Save
                cout << "Auto-save is currently " << (autoSaveEnabled ? "enabled" : "disabled") << ".\n";
                cout << "Do you want to toggle it? (y/n): ";
                char toggleChoice;
                cin >> toggleChoice;
                if (tolower(toggleChoice) == 'y') {
                    autoSaveEnabled = !autoSaveEnabled;
                    cout << "Auto-save has been " << (autoSaveEnabled ? "enabled." : "disabled.") << "\n";
                }
                break;
            }
            case 4: { // Sort Students
                sortStudents(head);
                break;
            }
            case 5: // filter students
                int filterChoice;
                cout << "Filter by:\n1. GPA range\n2. Age range\nEnter choice: ";
                cin >> filterChoice;
                float minGPA, maxGPA;
                int minAge, maxAge;
                switch (filterChoice) {
                    case 1: // Filter by GPA range
                        cout << "Enter minimum GPA: ";
                        cin >> minGPA;
                        cout << "Enter maximum GPA: ";
                        cin >> maxGPA;
                        filterStudentsByGPA(head, minGPA, maxGPA);
                        break;
                    case 2: // Filter by Age range
                        cout << "Enter minimum Age: ";
                        cin >> minAge;
                        cout << "Enter maximum Age: ";
                        cin >> maxAge;
                        filterStudentsByAge(head, minAge, maxAge);
                        break;
                    default:
                        cout << "Invalid choice.\n";
                        break;
                }
                
                break;
            case 6: // generate test cases
                int count;
                cout << "Enter number of test cases to generate: ";
                cin >> count;
                generateTestCases(head, count);
                break;
            case 7: // save to file
                saveToFile(head, "students.csv", true);
                break;
            case 8: // load from file
                loadFromFile(head, "students.csv");
                loadTeachers(teacherHead,"teachers.csv");
                break;
            case 9: // exit
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
        
        if (choice != 9) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 9);
}

// Handle Teacher Actions
void handleTeacherActions(Student*& head) {
    int choice;
    do {
        displayTeacherMenu();
        cout << "Enter your choice (1-10): ";
        cin >> choice;

        while (cin.fail() || choice < 1 || choice > 10) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 10: ";
            cin >> choice;
        }

        switch (choice) {
            case 1: // View Students
                displayStudents(head);
                break;
            case 2: // Add Student
                addStudent(head);
                if (autoSaveEnabled) {
                    saveToFile(head, "students.csv", true);
                }
                break;
            case 3: { // Update Student
                int id;
                cout << "Enter student ID to update: ";
                cin >> id;
                if (Student* student = searchStudentByID(head, id)) {
                    updateStudent(student);
                    if (autoSaveEnabled) {
                        saveToFile(head, "students.csv", true);
                    }
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
                    if (Student* student = searchStudentByID(head, id)) {
                        cout << "\nFound Student:\n";
                        cout << "ID: " << student->id << ", Name: " << student->name 
                             << ", GPA: " << student->gpa << endl;
                    } else {
                        cout << "Student not found.\n";
                    }
                } else if (searchChoice == 2) {
                    string name;
                    cout << "Enter name to search: ";
                    cin.ignore();
                    getline(cin, name);
                    if (Student* student = searchStudentByName(head, name)) {
                        cout << "\nFound Student:\n";
                        cout << "ID: " << student->id << ", Name: " << student->name 
                             << ", GPA: " << student->gpa << endl;
                    } else {
                        cout << "Student not found.\n";
                    }
                }
                break;
            }
            case 5: // Filter Students
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
            case 6: // Sort Students
                sortStudents(head);
                break;
            case 7: { // Manage Student Courses and Grades
                int id;
                cout << "Enter student ID: ";
                cin >> id;
                Student* student = searchStudentByID(head, id);
                if (student) {
                    cout << "\nStudent found: " << student->name << "\n";
                    int subChoice;
                    do {
                        cout << "\n=== Course Management ===\n";
                        cout << "1. Add Course and Grade\n";
                        cout << "2. Update Course Grade\n";
                        cout << "3. View Courses and Grades\n";
                        cout << "4. Remove Course\n";
                        cout << "5. Back\n";
                        cout << "Enter choice: ";
                        cin >> subChoice;

                        switch (subChoice) {
                            case 1:
                                addCourseWithDegree(student);
                                break;
                            case 2: {
                                if (student->courses.empty()) {
                                    cout << "No courses found.\n";
                                    break;
                                }
                                displayCoursesAndDegrees(student);
                                cout << "Enter course number to update: ";
                                int index;
                                cin >> index;
                                if (index > 0 && index <= static_cast<int>(student->courses.size())) {
                                    cout << "Enter new grade (0-100): ";
                                    float newGrade;
                                    cin >> newGrade;
                                    while (cin.fail() || newGrade < 0 || newGrade > 100) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cout << "Invalid grade. Please enter a number between 0 and 100: ";
                                        cin >> newGrade;
                                    }
                                    student->courses[index-1].degree = newGrade;
                                    cout << "Grade updated successfully!\n";
                                } else {
                                    cout << "Invalid course number.\n";
                                }
                                break;
                            }
                            case 3:
                                displayCoursesAndDegrees(student);
                                break;
                            case 4: {
                                if (student->courses.empty()) {
                                    cout << "No courses to remove.\n";
                                    break;
                                }
                                displayCoursesAndDegrees(student);
                                cout << "Enter course number to remove: ";
                                int index;
                                cin >> index;
                                if (index > 0 && index <= static_cast<int>(student->courses.size())) {
                                    student->courses.erase(student->courses.begin() + index - 1);
                                    cout << "Course removed successfully.\n";
                                } else {
                                    cout << "Invalid course number.\n";
                                }
                                break;
                            }
                        }
                    } while (subChoice != 5);
                } else {
                    cout << "Student not found.\n";
                }
                break;
            }
            case 8: { // View Student GPAs
                if (!head) {
                    cout << "No students in the system.\n";
                    break;
                }
                cout << "\n=== Student GPAs ===\n";
                Student* current = head;
                while (current) {
                    float gpa = calculateGPA(current->courses);
                    cout << "ID: " << current->id 
                         << ", Name: " << current->name 
                         << ", GPA: " << fixed << setprecision(2) << gpa;
                    
                    // عرض التقدير
                    cout << " (";
                    if (gpa >= 3.7) cout << "Excellent";
                    else if (gpa >= 3.0) cout << "Very Good";
                    else if (gpa >= 2.3) cout << "Good";
                    else if (gpa >= 1.7) cout << "Pass";
                    else cout << "Fail";
                    cout << ")\n";
                    
                    current = current->next;
                }
                break;
            }
            case 9: // Save Changes
                saveToFile(head, "students.csv", true);
                cout << "Changes saved successfully!\n";
                break;
            case 10: // Exit
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearConsole();
    } while (choice != 10);
}

// Handle Student Actions
void handleStudentActions(Student* head, User& currentUser) {
    int choice;
    string studentEmail = currentUser.username;
    Student* currentStudent = nullptr;

    while (head) {
        if (head->email == studentEmail) {
            currentStudent = head;
            break;
        }
        head = head->next;
    }

    if (!currentStudent) {
        cout << "Error: Student information not found.\n";
        return;
    }

    do {
        displayStudentMenu();
        cin >> choice;

        switch (choice) {
            case 1: { // View My Information
                cout << "\n=== My Information ===\n";
                cout << "Name: " << currentStudent->name << endl;
                cout << "ID: " << currentStudent->id << endl;
                cout << "Age: " << currentStudent->age << endl;
                cout << "GPA: " << currentStudent->gpa << endl;
                cout << "Email: " << currentStudent->email << endl;
                cout << "Department: " << currentStudent->department << endl;
                cout << "Year of Study: " << currentStudent->yearOfStudy << endl;
                break;
            }
            case 2: { // View My Courses
                cout << "\n=== My Courses ===\n";
                if (currentStudent->courses.empty()) {
                    cout << "You are not enrolled in any courses.\n";
                } else {
                    for (size_t i = 0; i < currentStudent->courses.size(); i++) {
                        cout << i + 1 << ". " << currentStudent->courses[i].name << endl;
                    }
                }
                break;
            }
            case 3: { // View My Grades and GPA
                cout << "\n=== My Grades and GPA ===\n";
                displayCoursesAndDegrees(currentStudent);
                
                float currentGPA = calculateGPA(currentStudent->courses);
                cout << "\nCurrent GPA: " << fixed << setprecision(2) << currentGPA << "\n";
                
                // عرض التقدير
                cout << "Grade: ";
                if (currentGPA >= 3.7) cout << "Excellent (A)";
                else if (currentGPA >= 3.0) cout << "Very Good (B)";
                else if (currentGPA >= 2.3) cout << "Good (C)";
                else if (currentGPA >= 1.7) cout << "Pass (D)";
                else cout << "Fail (F)";
                cout << endl;
                break;
            }
            case 4: // Exit
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        clearConsole();
    } while (choice != 4);
}

// Login Function
bool login(User& user) {
    string username, password;
    int attempts = 3;
    
    while (attempts > 0) {
        clearConsole();
        cout << "\n=== Login System ===\n";
        cout << "Attempts remaining: " << attempts << "\n\n";
        
        // Read username
        cout << "Enter username: ";
        if (attempts == 3) {  // Only for first attempt
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        getline(cin, username);
        
        // Read password with hiding
        cout << "Enter password: ";
        password = "";
        char ch;
        while (true) {
            ch = _getch();
            if (ch == 13) // Enter key
                break;
            if (ch == 8) { // Backspace
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

        // Validate credentials
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
                case 2: {
                    // Find teacher name from file
                    ifstream file("teachers.csv");
                    string line;
                    string teacherName;
                    while (getline(file, line)) {
                        stringstream ss(line);
                        string id, name, email;
                        getline(ss, id, ',');
                        getline(ss, name, ',');
                        getline(ss, email, ',');
                        if (email == username) {
                            teacherName = name;
                            break;
                        }
                    }
                    cout << "Welcome, Teacher " << teacherName << "!\n";
                    break;
                }
                case 3: {
                    // Find student name from file
                    ifstream file("students.csv");
                    string line;
                    string studentName;
                    while (getline(file, line)) {
                        stringstream ss(line);
                        string id, name, age, gpa, email;
                        getline(ss, id, ',');
                        getline(ss, name, ',');
                        getline(ss, age, ',');
                        getline(ss, gpa, ',');
                        getline(ss, email, ',');
                        if (email == username) {
                            studentName = name;
                            break;
                        }
                    }
                    cout << "Welcome, Student " << studentName << "!\n";
                    break;
                }
            }
            
            Sleep(1500);
            return true;
        }
        
        attempts--;
        cout << "\nError: Invalid username or password.";
        if (attempts > 0) {
            cout << " Please try again.\n";
            Sleep(1500);
        }
    }
    
    cout << "\nMaximum login attempts reached. Please try again later.\n";
    Sleep(2000);
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

// Function to check for duplicate student in the CSV file
bool isDuplicateStudent(int id, const string& email) {
    ifstream file("students.csv");
    if (!file.is_open()) {
        return false; // If the file doesn't exist, no duplicates
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string tempID, tempEmail;

        getline(ss, tempID, ','); // Read ID
        getline(ss, tempEmail, ','); // Read Email

        // Check for duplicate ID or Email
        if (stoi(tempID) == id || tempEmail == email) {
            file.close();
            return true; // Duplicate found
        }
    }
    file.close();
    return false; // No duplicates found
}

// Add Student
void addStudent(Student*& head) {
    Student* newStudent = new Student();
    
    cout << "\n=== Add New Student ===\n";
    cout << "Enter ID: ";
    cin >> newStudent->id;

    // Check for duplicate ID in the linked list
    Student* current = head;
    while (current) {
        if (current->id == newStudent->id) {
            cout << "Error: A student with ID " << newStudent->id << " already exists in the system.\n";
            delete newStudent;
            return;
        }
        current = current->next;
    }

    cout << "Enter Email: ";
    cin >> newStudent->email;
    if (isDuplicateStudent(newStudent->id, newStudent->email)) {
        cout << "Error: A student with ID " << newStudent->id << " or Email " << newStudent->email << " already exists in the CSV file.\n";
        delete newStudent;
        return;
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

    cout << "Enter Department: ";
    cin.ignore();
    getline(cin, newStudent->department);
    
    cout << "Enter Year of Study: ";
    getline(cin, newStudent->yearOfStudy);

    // إضافة تعيين كلمة المرور
    string password, confirmPassword;
    do {
        cout << "Set Password: ";
        password = "";
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
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

        cout << "Confirm Password: ";
        confirmPassword = "";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (confirmPassword.length() > 0) {
                    confirmPassword.pop_back();
                    cout << "\b \b";
                }
            } else {
                confirmPassword += ch;
                cout << '*';
            }
        }
        cout << endl;

        if (password != confirmPassword) {
            cout << "Passwords do not match! Please try again.\n";
        }
    } while (password != confirmPassword);

    newStudent->password = password;

    newStudent->next = head;
    head = newStudent;
    cout << "Student added successfully!\n";

    if (autoSaveEnabled) {
        saveToFile(head, "students.csv", true);
        cout << "Data saved to students.csv successfully.\n";
    }
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
        saveToFile(head, "students.csv", true); // Pass true to include password if needed
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

    cout << "\n=== Student List ===\n";
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Age" 
         << setw(10) << "GPA" << setw(25) << "Email" << setw(20) << "Department" << endl;
    cout << string(90, '-') << endl;

    Student* current = head;
    while (current) {
        cout << setw(5) << current->id 
             << setw(20) << current->name 
             << setw(10) << current->age 
             << setw(10) << fixed << setprecision(2) << current->gpa 
             << setw(25) << current->email 
             << setw(20) << current->department << endl;
        current = current->next;
    }
    cout << string(90, '-') << endl;
}

// Sort by Name
void swapNodes(Student*& a, Student*& b) {
    Student* temp = a;
    a = b;
    b = temp;
}

void sortStudentsByName(Student*& head, bool ascending) {
    if (!head || !head->next) return;

    Student* current = head;
    while (current) {
        Student* min = current;
        Student* r = current->next;
        
        while (r) {
            bool shouldSwap = ascending ? (r->name < min->name) : (r->name > min->name);
            if (shouldSwap) {
                min = r;
            }
            r = r->next;
        }
        
        if (min != current) {
            // تبديل البيانات
            swap(current->id, min->id);
            swap(current->name, min->name);
            swap(current->age, min->age);
            swap(current->gpa, min->gpa);
            swap(current->email, min->email);
            swap(current->department, min->department);
            swap(current->yearOfStudy, min->yearOfStudy);
            swap(current->password, min->password);
            swap(current->courses, min->courses);
        }
        
        current = current->next;
    }
}

// Sort by GPA
void sortStudentsByGPA(Student*& head, bool ascending) {
    if (!head || !head->next) return;

    Student* current = head;
    while (current) {
        Student* min = current;
        Student* r = current->next;
        
        while (r) {
            if ((ascending && r->gpa < min->gpa) ||
                (!ascending && r->gpa > min->gpa)) {
                min = r;
            }
            r = r->next;
        }
        
        if (min != current) {
            // Swap data between current and min
            swap(current->id, min->id);
            swap(current->name, min->name);
            swap(current->age, min->age);
            swap(current->gpa, min->gpa);
            swap(current->email, min->email);
            swap(current->department, min->department);
            swap(current->yearOfStudy, min->yearOfStudy);
            swap(current->password, min->password);
            swap(current->courses, min->courses);
        }
        
        current = current->next;
    }
}

// Sort by ID
void sortStudentsByID(Student*& head, bool ascending) {
    if (!head || !head->next) return;

    Student* current = head;
    while (current) {
        Student* min = current;
        Student* r = current->next;
        
        while (r) {
            if ((ascending && r->id < min->id) ||
                (!ascending && r->id > min->id)) {
                min = r;
            }
            r = r->next;
        }
        
        if (min != current) {
            // Swap data between current and min
            swap(current->id, min->id);
            swap(current->name, min->name);
            swap(current->age, min->age);
            swap(current->gpa, min->gpa);
            swap(current->email, min->email);
            swap(current->department, min->department);
            swap(current->yearOfStudy, min->yearOfStudy);
            swap(current->password, min->password);
            swap(current->courses, min->courses);
        }
        
        current = current->next;
    }
}

// Sort by Age
void sortStudentsByAge(Student*& head, bool ascending) {
    if (!head || !head->next) return;

    Student* current = head;
    while (current) {
        Student* min = current;
        Student* r = current->next;
        
        while (r) {
            if ((ascending && r->age < min->age) ||
                (!ascending && r->age > min->age)) {
                min = r;
            }
            r = r->next;
        }
        
        if (min != current) {
            // Swap data between current and min
            swap(current->id, min->id);
            swap(current->name, min->name);
            swap(current->age, min->age);
            swap(current->gpa, min->gpa);
            swap(current->email, min->email);
            swap(current->department, min->department);
            swap(current->yearOfStudy, min->yearOfStudy);
            swap(current->password, min->password);
            swap(current->courses, min->courses);
        }
        
        current = current->next;
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
void saveToFile(Student* head, const string& filename, bool includePassword) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening file.\n";
        return;
    }
    while (head) {
        file << head->id << "," << head->name << "," << head->age << ","
             << head->gpa << "," << head->email << "," << head->department << ","
             << head->yearOfStudy << "," << head->password << "\n";
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
        getline(ss, newStudent->password, ',');
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
    // Store the original list
    Student* originalHead = head;
    head = nullptr;

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

    // Display generated test cases
    displayStudents(head);

    // Cleanup generated test cases
    while (head) {
        Student* temp = head;
        head = head->next;
        delete temp;
    }

    // Restore original list
    head = originalHead;
    cout << "Test cases cleaned up. Original list restored.\n";
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
    // Admin credentials
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
            string id, name, age, gpa, email, department, yearOfStudy, storedPassword;
            
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, age, ',');
            getline(ss, gpa, ',');
            getline(ss, email, ',');
            getline(ss, department, ',');
            getline(ss, yearOfStudy, ',');
            getline(ss, storedPassword, ',');
            
            email = trim(email);
            storedPassword = trim(storedPassword);
            
            if (email == username && storedPassword == password) {
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

    // Check for duplicate ID in the linked list
    Teacher* current = head;
    while (current) {
        if (current->id == newTeacher->id) {
            cout << "Error: A teacher with ID " << newTeacher->id << " already exists in the system.\n";
            delete newTeacher; // Clean up the allocated memory
            return; // Exit the function
        }
        current = current->next;
    }

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newTeacher->name);
    
    cout << "Enter Email: ";
    getline(cin, newTeacher->email);
    
    cout << "Enter Department: ";
    getline(cin, newTeacher->department);
    
    string password, confirmPassword;
    do {
        cout << "Set Password: ";
        password = "";
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
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

        cout << "Confirm Password: ";
        confirmPassword = "";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (confirmPassword.length() > 0) {
                    confirmPassword.pop_back();
                    cout << "\b \b";
                }
            } else {
                confirmPassword += ch;
                cout << '*';
            }
        }
        cout << endl;

        if (password != confirmPassword) {
            cout << "Passwords do not match! Please try again.\n";
        }
    } while (password != confirmPassword);

    newTeacher->password = password;

    newTeacher->next = head;
    head = newTeacher;
    cout << "Teacher added successfully!\n";

    if (autoSaveEnabled) {
        saveTeachers(head, "teachers.csv"); // Use saveTeachers instead of saveToFile
        cout << "Data saved to teachers.csv successfully.\n";
    }
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
    cout << "10. Manage Courses and Degrees\n";
    cout << "11. Back to Main Menu\n";
}

void addCourseWithDegree(Student* student) {
    cout << "\n=== Add Course and Degree ===\n";
    Course newCourse;
    
    cout << "Enter course name: ";
    cin.ignore();
    getline(cin, newCourse.name);
    
    cout << "Enter course degree (0-100): ";
    cin >> newCourse.degree;
    while (cin.fail() || newCourse.degree < 0 || newCourse.degree > 100) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid degree. Please enter a number between 0 and 100: ";
        cin >> newCourse.degree;
    }
    
    student->courses.push_back(newCourse);
    cout << "Course and degree added successfully!\n";
    
    // Save the course enrollment to a new CSV file
    ofstream file("course_enrollments.csv", ios::app);
    if (file.is_open()) {
        file << student->id << "," << newCourse.name << "," << newCourse.degree << "\n";
        file.close();
        cout << "Course enrollment saved to CSV file successfully.\n";
    } else {
        cout << "Error opening file to save course enrollment.\n";
    }
}
void loadCoursesForStudents(Student* head, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file.\n";
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, courseName, degree;
        getline(ss, id, ',');
        getline(ss, courseName, ',');
        getline(ss, degree, ',');
        Student* current = head;
        while (current) {
            if (current->id == stoi(id)) {
                Course newCourse;
                newCourse.name = courseName;
                newCourse.degree = stof(degree);
                current->courses.push_back(newCourse);
                break;
            }
            current = current->next;
        }
    }
    file.close();
    cout << "Courses loaded from " << filename << " successfully.\n";
}


void displayCoursesAndDegrees(Student* student) {
    if (student->courses.empty()) {
        cout << "No courses found.\n";
        return;
    }
    
    cout << "\n=== Courses and Degrees ===\n";
    for (size_t i = 0; i < student->courses.size(); i++) {
        cout << i + 1 << ". Course: " << student->courses[i].name 
             << ", Degree: " << student->courses[i].degree << "\n";
    }
}

// تحديث دالة updateCourses لتشمل الدرجات
void updateCourses(Student* student) {
    cout << "\n=== Course Management ===\n";
    cout << "1. Add Course with Degree\n";
    cout << "2. Remove Course\n";
    cout << "3. Display Courses and Degrees\n";
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            addCourseWithDegree(student);
            break;
        case 2: {
            if (student->courses.empty()) {
                cout << "No courses to remove.\n";
                break;
            }
            displayCoursesAndDegrees(student);
            cout << "Enter course number to remove: ";
            int index;
            cin >> index;
            if (index > 0 && index <= static_cast<int>(student->courses.size())) {
                student->courses.erase(student->courses.begin() + index - 1);
                cout << "Course removed successfully.\n";
            } else {
                cout << "Invalid course number.\n";
            }
            break;
        }
        case 3:
            displayCoursesAndDegrees(student);
            break;
        default:
            cout << "Invalid choice.\n";
    }
}

// إضافة دالة جديدة لإدارة المواد والدرجات
void manageCourseAndDegrees(Student*& head) {
    int id;
    cout << "Enter student ID: ";
    cin >> id;
    
    Student* student = searchStudentByID(head, id);
    if (!student) {
        cout << "Student not found.\n";
        return;
    }

    int choice;
    do {
        cout << "\n=== Course and Degree Management ===\n";
        cout << "1. Add Course and Degree\n";
        cout << "2. Update Course Degree\n";
        cout << "3. Display Courses and Degrees\n";
        cout << "4. Remove Course\n";
        cout << "5. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addCourseWithDegree(student);
                break;
            case 2: {
                if (student->courses.empty()) {
                    cout << "No courses found.\n";
                    break;
                }
                displayCoursesAndDegrees(student);
                cout << "Enter course number to update: ";
                int index;
                cin >> index;
                
                if (index > 0 && index <= static_cast<int>(student->courses.size())) {
                    cout << "Enter new degree (0-100): ";
                    float newDegree;
                    cin >> newDegree;
                    while (cin.fail() || newDegree < 0 || newDegree > 100) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid degree. Please enter a number between 0 and 100: ";
                        cin >> newDegree;
                    }
                    student->courses[index-1].degree = newDegree;
                    cout << "Degree updated successfully!\n";
                } else {
                    cout << "Invalid course number.\n";
                }
                break;
            }
            case 3:
                displayCoursesAndDegrees(student);
                break;
            case 4: {
                if (student->courses.empty()) {
                    cout << "No courses to remove.\n";
                    break;
                }
                displayCoursesAndDegrees(student);
                cout << "Enter course number to remove: ";
                int index;
                cin >> index;
                if (index > 0 && index <= static_cast<int>(student->courses.size())) {
                    student->courses.erase(student->courses.begin() + index - 1);
                    cout << "Course removed successfully.\n";
                } else {
                    cout << "Invalid course number.\n";
                }
                break;
            }
            case 5:
                cout << "Returning to previous menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void sortStudents(Student*& head) {
    if (!head) {
        cout << "No students to sort.\n";
        return;
    }

    int sortChoice;
    cout << "\n=== Sort Students ===\n";
    cout << "Sort by:\n";
    cout << "1. Name\n";
    cout << "2. GPA\n";
    cout << "3. ID\n";
    cout << "4. Age\n";
    cout << "Enter choice: ";
    cin >> sortChoice;

    while (cin.fail() || sortChoice < 1 || sortChoice > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between 1 and 4: ";
        cin >> sortChoice;
    }

    bool ascending;
    cout << "Sort in ascending order (1 for Yes, 0 for No): ";
    cin >> ascending;

    // تنفيذ الترتيب
    switch (sortChoice) {
        case 1:
            sortStudentsByName(head, ascending);
            cout << "\nSorted by Name (" << (ascending ? "Ascending" : "Descending") << "):\n";
            break;
        case 2:
            sortStudentsByGPA(head, ascending);
            cout << "\nSorted by GPA (" << (ascending ? "Ascending" : "Descending") << "):\n";
            break;
        case 3:
            sortStudentsByID(head, ascending);
            cout << "\nSorted by ID (" << (ascending ? "Ascending" : "Descending") << "):\n";
            break;
        case 4:
            sortStudentsByAge(head, ascending);
            cout << "\nSorted by Age (" << (ascending ? "Ascending" : "Descending") << "):\n";
            break;
    }

    // عرض القائمة المرتبة
    displayStudents(head);

    // حفظ التغييرات
    if (autoSaveEnabled) {
        saveToFile(head, "students.csv", true);
        cout << "\nSorted list saved to file.\n";
    }
}

// إضافة دالة لحساب GPA
float calculateGPA(const vector<Course>& courses) {
    if (courses.empty()) {
        return 0.0f;
    }

    float totalPoints = 0.0f;
    for (const Course& course : courses) {
        // تحويل الدرجة من 100 إلى نظام 4.0
        float gradePoints;
        if (course.degree >= 90) gradePoints = 4.0;
        else if (course.degree >= 85) gradePoints = 3.7;
        else if (course.degree >= 80) gradePoints = 3.3;
        else if (course.degree >= 75) gradePoints = 3.0;
        else if (course.degree >= 70) gradePoints = 2.7;
        else if (course.degree >= 65) gradePoints = 2.3;
        else if (course.degree >= 60) gradePoints = 2.0;
        else if (course.degree >= 55) gradePoints = 1.7;
        else if (course.degree >= 50) gradePoints = 1.3;
        else gradePoints = 0.0;

        totalPoints += gradePoints;
    }

    return totalPoints / courses.size();
}
