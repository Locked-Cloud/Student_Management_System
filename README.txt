StudentManagementSystem/
│
├── main.cpp                                // Contains the main function and program flow
│
├── User.h                                  // Header file for the User structure (Admin, Teacher, Student)
│
├── User.cpp                                // Implementation file for User-related functions
│
├── Admin.h                                 // Header file for the Admin class
│
├── Admin.cpp                               // Implementation file for Admin-related functions
│
├── Teacher.h                               // Header file for the Teacher class
│
├── Teacher.cpp                             // Implementation file for Teacher-related functions
│
├── Student.h                               // Header file for the Student structure
│
├── Student.cpp                             // Implementation file for Student-related functions
│   ├── encryptStudentData()                // Encrypts student data before saving it
│   └── decryptStudentData()                // Decrypts student data when loading it
│
├── StudentManagementSystem.h               // Header file for system functions
│
├── StudentManagementSystem.cpp             // Implementation file for system functions
│   ├── addStudent()                       // Adds a new student to the system
│   ├── editStudent()                      // Edits details of an existing student
│   ├── deleteStudent()                    // Deletes a student from the system
│   ├── displayStudents()                  // Displays the list of all students
│   ├── sortStudents()                     // Sorts students by chosen attribute
│   ├── saveToCSV()                        // Saves student data to a CSV file
│   ├── loadFromCSV()                      // Loads student data from a CSV file
│   ├── backupData()                       // Backs up the student data to a file
│   ├── restoreData()                      // Restores student data from a backup file
│   └── colorCustomization()                // Allows color customization of the interface
│
├── Makefile                               // (Optional) Build configuration for the project
│
└── data/
    ├── students.csv                       // Stores encrypted student data
    ├── backup.csv                         // Stores backup data
    ├── users.csv                          // Stores user data (Admin, Teacher, Student)
    └── grades.csv                         // Stores student grades for GPA calculation
