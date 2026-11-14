#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 15
#define MAX_NAME_LEN 64
#define MAX_ID_LEN 20
#define MAX_DEPT_LEN 32
#define MAX_SUB_CODE_LEN 16
#define MAX_SUB_NAME_LEN 64

// Structure representing an individual academic subject / course
typedef struct {
    char code[MAX_SUB_CODE_LEN];        // e.g., "CS101", "MATH201"
    char name[MAX_SUB_NAME_LEN];        // e.g., "Data Structures"
    int credits;                        // Credit weight (1 to 6)
    int semester;                       // Semester number (1 to 8)
    float marks;                        // Score obtained (0.0 to 100.0)
    char grade[4];                      // Letter grade: "A+", "A", "B+", etc.
    int grade_point;                    // Numerical grade point (0 to 10)
} Subject;

// Structure representing an enrolled student record
typedef struct {
    char id[MAX_ID_LEN];                // Unique student ID (e.g., "STU-1001")
    char name[MAX_NAME_LEN];            // Full student name
    char department[MAX_DEPT_LEN];      // Department/Branch (e.g., "Computer Science")
    int academic_year;                  // Year of study (1 to 4)
    int current_semester;               // Current semester (1 to 8)
    int subject_count;                  // Number of registered subjects
    Subject subjects[MAX_SUBJECTS];     // Array of registered subjects
    float gpa;                          // Current semester GPA
    float cgpa;                         // Cumulative Grade Point Average
    char performance_level[32];         // "Excellent", "Good", "Average", "Needs Improvement"
} Student;

// Structure encapsulating the in-memory student database
typedef struct {
    Student students[MAX_STUDENTS];
    int count;
} StudentDatabase;

// Function declarations: Database Lifecycle
void init_student_database(StudentDatabase *db);

// Function declarations: Student Management Module (Member 1)
bool add_student(StudentDatabase *db);
void view_all_students(const StudentDatabase *db);
int find_student_by_id(const StudentDatabase *db, const char *id);
int find_students_by_name(const StudentDatabase *db, const char *keyword, int match_indices[], int max_matches);
void search_student_menu(const StudentDatabase *db);
bool update_student(StudentDatabase *db);
bool delete_student(StudentDatabase *db);

// Function declarations: Subject & Marks Management Module (Member 1 & 2)
bool add_subject_to_student(StudentDatabase *db);
void view_student_subjects(const Student *s);

// Function declarations: Terminal UI & Input Validation Helpers (Member 3)
void clear_screen(void);
void wait_for_enter(void);
void get_string_input(const char *prompt, char *buffer, int max_len);
int get_int_input(const char *prompt, int min_val, int max_val);
float get_float_input(const char *prompt, float min_val, float max_val);
bool get_yes_no_input(const char *prompt);

#endif // STUDENT_H
