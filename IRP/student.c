#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"
#include "gpa.h"

// ANSI Color macros
#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_DIM     "\033[2m"
#define CLR_CYAN    "\033[36m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_RED     "\033[31m"
#define CLR_BLUE    "\033[34m"
#define CLR_MAGENTA "\033[35m"
#define CLR_WHITE   "\033[37m"

// Case-insensitive string comparison helper
static int str_case_cmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        if (diff != 0) return diff;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

// Case-insensitive substring search helper
static const char* str_case_str(const char *haystack, const char *needle) {
    if (!*needle) return haystack;
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack;
            const char *n = needle;
            while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++;
                n++;
            }
            if (!*n) return haystack;
        }
    }
    return NULL;
}

// Screen clearing utility
void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pause and wait for user acknowledgment
void wait_for_enter(void) {
    printf("\n" CLR_DIM "  >> Press ENTER to continue... " CLR_RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Clean string input with newline strip and buffer flush
void get_string_input(const char *prompt, char *buffer, int max_len) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, max_len, stdin) != NULL) {
            size_t len = strlen(buffer);
            if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
                buffer[len - 1] = '\0';
                len--;
            }
            if (len > 0 && buffer[len - 1] == '\r') {
                buffer[len - 1] = '\0';
                len--;
            }
            // Trim leading spaces
            char *start = buffer;
            while (isspace((unsigned char)*start)) start++;
            if (start != buffer) {
                memmove(buffer, start, strlen(start) + 1);
            }
            if (strlen(buffer) > 0) {
                break;
            }
        } else {
            // EOF encountered
            buffer[0] = '\0';
            return;
        }
        printf(CLR_RED "  [!] Input cannot be empty. Please enter a valid value.\n" CLR_RESET);
    }
}

// Robust integer input with range check and buffer clearance
int get_int_input(const char *prompt, int min_val, int max_val) {
    char input_buf[64];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input_buf, sizeof(input_buf), stdin) != NULL) {
            char extra;
            if (sscanf(input_buf, "%d %c", &value, &extra) == 1) {
                if (value >= min_val && value <= max_val) {
                    return value;
                }
                printf(CLR_RED "  [!] Out of bounds! Expected range [%d - %d]. Try again.\n" CLR_RESET, min_val, max_val);
            } else {
                printf(CLR_RED "  [!] Invalid numeric format. Please enter an integer.\n" CLR_RESET);
            }
        } else {
            // EOF encountered
            return min_val;
        }
    }
}

// Robust floating-point input with range validation
float get_float_input(const char *prompt, float min_val, float max_val) {
    char input_buf[64];
    float value;
    while (1) {
        printf("%s", prompt);
        if (fgets(input_buf, sizeof(input_buf), stdin) != NULL) {
            char extra;
            if (sscanf(input_buf, "%f %c", &value, &extra) == 1) {
                if (value >= min_val && value <= max_val) {
                    return value;
                }
                printf(CLR_RED "  [!] Out of range! Expected [%.1f - %.1f]. Try again.\n" CLR_RESET, min_val, max_val);
            } else {
                printf(CLR_RED "  [!] Invalid number format. Please enter a decimal/number.\n" CLR_RESET);
            }
        } else {
            // EOF encountered
            return min_val;
        }
    }
}

// Simple Yes/No prompt
bool get_yes_no_input(const char *prompt) {
    char input_buf[32];
    while (1) {
        printf("%s (y/n): ", prompt);
        if (fgets(input_buf, sizeof(input_buf), stdin) != NULL) {
            for (int i = 0; input_buf[i]; i++) {
                if (!isspace((unsigned char)input_buf[i])) {
                    char c = tolower((unsigned char)input_buf[i]);
                    if (c == 'y') return true;
                    if (c == 'n') return false;
                    break;
                }
            }
        } else {
            // EOF encountered
            return false;
        }
        printf(CLR_RED "  [!] Please type 'y' for yes or 'n' for no.\n" CLR_RESET);
    }
}

// Initialize database
void init_student_database(StudentDatabase *db) {
    if (db) {
        db->count = 0;
    }
}

// Find student index by ID (case-insensitive)
int find_student_by_id(const StudentDatabase *db, const char *id) {
    if (!db || !id) return -1;
    for (int i = 0; i < db->count; i++) {
        if (str_case_cmp(db->students[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

// Find students matching name keyword
int find_students_by_name(const StudentDatabase *db, const char *keyword, int match_indices[], int max_matches) {
    if (!db || !keyword) return 0;
    int matches = 0;
    for (int i = 0; i < db->count && matches < max_matches; i++) {
        if (str_case_str(db->students[i].name, keyword) != NULL) {
            match_indices[matches++] = i;
        }
    }
    return matches;
}

// Add a new student record
bool add_student(StudentDatabase *db) {
    if (!db) return false;

    if (db->count >= MAX_STUDENTS) {
        printf("\n" CLR_RED "  [!] Database capacity reached (Max %d students).\n" CLR_RESET, MAX_STUDENTS);
        wait_for_enter();
        return false;
    }

    printf("\n" CLR_CYAN "  +-------------------------------------------------------------+\n");
    printf("  |               ENROLL NEW STUDENT PROFILE 🙎                 |\n");
    printf("  +----------------------------------------------------------+\n" CLR_RESET);

    char id[MAX_ID_LEN];
    while (1) {
        get_string_input("  -> Enter Student ID (e.g., STU-101): ", id, sizeof(id));
        if (find_student_by_id(db, id) != -1) {
            printf(CLR_YELLOW "  [!] Warning: Student ID '%s' is already registered. Enter a unique ID.\n" CLR_RESET, id);
        } else {
            break;
        }
    }

    Student *s = &db->students[db->count];
    strncpy(s->id, id, sizeof(s->id) - 1);
    s->id[sizeof(s->id) - 1] = '\0';

    get_string_input("  -> Enter Full Name: ", s->name, sizeof(s->name));
    get_string_input("  -> Enter Department / Branch (e.g., Computer Science): ", s->department, sizeof(s->department));
    s->academic_year = get_int_input("  -> Enter Academic Year (1 to 4): ", 1, 4);
    s->current_semester = get_int_input("  -> Enter Current Semester (1 to 8): ", 1, 8);
    s->subject_count = 0;
    s->gpa = 0.0f;
    s->cgpa = 0.0f;
    strncpy(s->performance_level, "Not Evaluated", sizeof(s->performance_level));

    db->count++;

    printf("\n" CLR_GREEN "  [+] Student '%s' (ID: %s) successfully registered!\n" CLR_RESET, s->name, s->id);

    if (get_yes_no_input("  -> Would you like to enter subjects/marks now?")) {
        while (s->subject_count < MAX_SUBJECTS) {
            printf("\n" CLR_CYAN "  --- Adding Subject #%d for %s ---" CLR_RESET "\n", s->subject_count + 1, s->name);
            Subject *sub = &s->subjects[s->subject_count];

            get_string_input("  -> Course Code (e.g., CS201): ", sub->code, sizeof(sub->code));
            get_string_input("  -> Course Title: ", sub->name, sizeof(sub->name));
            sub->credits = get_int_input("  -> Credit Hours (1 to 6): ", 1, 6);
            sub->semester = s->current_semester;
            sub->marks = get_float_input("  -> Marks Obtained (0.0 to 100.0): ", 0.0f, 100.0f);

            get_letter_grade(sub->marks, sub->grade, &sub->grade_point);
            s->subject_count++;

            printf(CLR_GREEN "     Recorded: %s | Grade: %s | Points: %d\n" CLR_RESET, sub->code, sub->grade, sub->grade_point);

            if (s->subject_count < MAX_SUBJECTS) {
                if (!get_yes_no_input("  -> Add another subject for this student?")) {
                    break;
                }
            } else {
                printf(CLR_YELLOW "  [i] Maximum subjects reached for this student.\n" CLR_RESET);
                break;
            }
        }
        update_student_analytics(s);
    }

    wait_for_enter();
    return true;
}

// Display all registered students in an elegant table
void view_all_students(const StudentDatabase *db) {
    if (!db || db->count == 0) {
        printf("\n" CLR_YELLOW "  [i] Student database is currently empty. No records to display.\n" CLR_RESET);
        wait_for_enter();
        return;
    }

    printf("\n" CLR_CYAN CLR_BOLD);
    printf("  ====================================================================================================\n");
    printf("                                      REGISTERED STUDENT DIRECTORY                                    \n");
    printf("  ====================================================================================================\n" CLR_RESET);

    printf(CLR_BOLD "  +------------+----------------------+--------------------+------+-----+----------+------+-------------------+\n");
    printf("  | Student ID | Full Name            | Department         | Year | Sem | Subjects | GPA  | Performance Level |\n");
    printf("  +------------+----------------------+--------------------+------+-----+----------+------+-------------------+\n" CLR_RESET);

    for (int i = 0; i < db->count; i++) {
        const Student *s = &db->students[i];

        // Format performance level color
        const char *color = CLR_WHITE;
        if (strcmp(s->performance_level, "Excellent") == 0) color = CLR_GREEN;
        else if (strcmp(s->performance_level, "Good") == 0) color = CLR_CYAN;
        else if (strcmp(s->performance_level, "Average") == 0) color = CLR_YELLOW;
        else if (strcmp(s->performance_level, "Needs Improvement") == 0) color = CLR_RED;

        printf("  | %-10s | %-20.20s | %-18.18s |  %2d  |  %2d |    %2d    | %4.2f | %s%-17.17s" CLR_RESET " |\n",
               s->id,
               s->name,
               s->department,
               s->academic_year,
               s->current_semester,
               s->subject_count,
               s->gpa,
               color,
               s->performance_level);
    }

    printf("  +------------+----------------------+--------------------+------+-----+----------+------+-------------------+\n");
    printf(CLR_DIM "  Total Records: %d student(s) enrolled.\n" CLR_RESET, db->count);

    wait_for_enter();
}

// Search student menu
void search_student_menu(const StudentDatabase *db) {
    if (!db || db->count == 0) {
        printf("\n" CLR_YELLOW "  [i] No students registered in database to search.\n" CLR_RESET);
        wait_for_enter();
        return;
    }

    printf("\n" CLR_CYAN "  +-------------------------------------------------------------+\n");
    printf("  |                    🔍 SEARCH STUDENT RECORDS 🔍                |\n");
    printf("  +-------------------------------------------------------------+\n" CLR_RESET);
    printf("    [1] Search by Exact Student ID\n");
    printf("    [2] Search by Name (Substring / Keyword)\n");
    printf("    [3] Cancel & Return to Dashboard\n");

    int choice = get_int_input("  -> Choose an option (1-3): ", 1, 3);
    if (choice == 1) {
        char id[MAX_ID_LEN];
        get_string_input("  -> Enter Student ID to search: ", id, sizeof(id));
        int idx = find_student_by_id(db, id);
        if (idx != -1) {
            printf("\n" CLR_GREEN "  [+] Student Found!" CLR_RESET "\n");
            print_student_report_card(&db->students[idx]);
        } else {
            printf(CLR_RED "  [!] No student found with ID '%s'.\n" CLR_RESET, id);
            wait_for_enter();
        }
    } else if (choice == 2) {
        char keyword[MAX_NAME_LEN];
        get_string_input("  -> Enter Name or Keyword to search: ", keyword, sizeof(keyword));
        int matches[MAX_STUDENTS];
        int count = find_students_by_name(db, keyword, matches, MAX_STUDENTS);
        if (count > 0) {
            printf("\n" CLR_GREEN "  [+] Found %d matching student(s):\n" CLR_RESET, count);
            for (int i = 0; i < count; i++) {
                const Student *s = &db->students[matches[i]];
                printf("    %d. %s (ID: %s) - %s, Sem %d, GPA: %.2f [%s]\n",
                       i + 1, s->name, s->id, s->department, s->current_semester, s->gpa, s->performance_level);
            }
            if (get_yes_no_input("\n  -> Would you like to view detailed report of a student?")) {
                int pick = get_int_input("  -> Enter matching student number (1 to count): ", 1, count);
                print_student_report_card(&db->students[matches[pick - 1]]);
            } else {
                wait_for_enter();
            }
        } else {
            printf(CLR_RED "  [!] No students found matching name keyword '%s'.\n" CLR_RESET, keyword);
            wait_for_enter();
        }
    }
}

// Update student information
bool update_student(StudentDatabase *db) {
    if (!db || db->count == 0) {
        printf("\n" CLR_YELLOW "  [i] Database is empty. Nothing to update.\n" CLR_RESET);
        wait_for_enter();
        return false;
    }

    char id[MAX_ID_LEN];
    get_string_input("\n  -> Enter Student ID to update: ", id, sizeof(id));
    int idx = find_student_by_id(db, id);
    if (idx == -1) {
        printf(CLR_RED "  [!] Student with ID '%s' was not found.\n" CLR_RESET, id);
        wait_for_enter();
        return false;
    }

    Student *s = &db->students[idx];

    while (1) {
        printf("\n" CLR_CYAN "  +-------------------------------------------------------------+\n");
        printf("  |                UPDATE STUDENT: %-28.28s |\n", s->name);
        printf("  +-------------------------------------------------------------+\n" CLR_RESET);
        printf("    [1] Update Full Name         (Current: %s)\n", s->name);
        printf("    [2] Update Department/Branch (Current: %s)\n", s->department);
        printf("    [3] Update Academic Year     (Current: %d)\n", s->academic_year);
        printf("    [4] Update Current Semester  (Current: %d)\n", s->current_semester);
        printf("    [5] Done Editing / Back to Menu\n");

        int choice = get_int_input("  -> Select attribute to edit (1-5): ", 1, 5);
        if (choice == 1) {
            get_string_input("  -> Enter New Full Name: ", s->name, sizeof(s->name));
            printf(CLR_GREEN "  [+] Name updated successfully!\n" CLR_RESET);
        } else if (choice == 2) {
            get_string_input("  -> Enter New Department: ", s->department, sizeof(s->department));
            printf(CLR_GREEN "  [+] Department updated successfully!\n" CLR_RESET);
        } else if (choice == 3) {
            s->academic_year = get_int_input("  -> Enter New Academic Year (1 to 4): ", 1, 4);
            printf(CLR_GREEN "  [+] Academic year updated successfully!\n" CLR_RESET);
        } else if (choice == 4) {
            s->current_semester = get_int_input("  -> Enter New Current Semester (1 to 8): ", 1, 8);
            printf(CLR_GREEN "  [+] Semester updated successfully!\n" CLR_RESET);
        } else {
            break;
        }
    }

    return true;
}

// Delete student record
bool delete_student(StudentDatabase *db) {
    if (!db || db->count == 0) {
        printf("\n" CLR_YELLOW "  [i] Database is empty. Nothing to delete.\n" CLR_RESET);
        wait_for_enter();
        return false;
    }

    char id[MAX_ID_LEN];
    get_string_input("\n  -> Enter Student ID to delete: ", id, sizeof(id));
    int idx = find_student_by_id(db, id);
    if (idx == -1) {
        printf(CLR_RED "  [!] Student with ID '%s' was not found.\n" CLR_RESET, id);
        wait_for_enter();
        return false;
    }

    Student *s = &db->students[idx];
    printf(CLR_YELLOW "\n  [?] CONFIRMATION REQUIRED:\n");
    printf("      Are you sure you want to PERMANENTLY delete:\n");
    printf("      Student: %s | ID: %s | Dept: %s\n" CLR_RESET, s->name, s->id, s->department);

    if (get_yes_no_input("  -> Confirm deletion?")) {
        // Shift remaining elements left
        for (int i = idx; i < db->count - 1; i++) {
            db->students[i] = db->students[i + 1];
        }
        db->count--;
        printf(CLR_GREEN "  [+] Student record '%s' has been successfully deleted.\n" CLR_RESET, id);
    } else {
        printf(CLR_CYAN "  [i] Deletion aborted. Student record preserved.\n" CLR_RESET);
    }

    wait_for_enter();
    return true;
}

// Add subject to a student
bool add_subject_to_student(StudentDatabase *db) {
    if (!db || db->count == 0) {
        printf("\n" CLR_YELLOW "  [i] No students found. Please add a student first.\n" CLR_RESET);
        wait_for_enter();
        return false;
    }

    char id[MAX_ID_LEN];
    get_string_input("\n  -> Enter Student ID to add subject for: ", id, sizeof(id));
    int idx = find_student_by_id(db, id);
    if (idx == -1) {
        printf(CLR_RED "  [!] Student with ID '%s' not found.\n" CLR_RESET, id);
        wait_for_enter();
        return false;
    }

    Student *s = &db->students[idx];
    if (s->subject_count >= MAX_SUBJECTS) {
        printf(CLR_RED "  [!] This student already has the maximum allowable subjects (%d).\n" CLR_RESET, MAX_SUBJECTS);
        wait_for_enter();
        return false;
    }

    printf("\n" CLR_CYAN "  +-------------------------------------------------------------+\n");
    printf("  |            ADD COURSE & MARKS: %-26.26s |\n", s->name);
    printf("  +-------------------------------------------------------------+\n" CLR_RESET);

    Subject *sub = &s->subjects[s->subject_count];
    get_string_input("  -> Subject / Course Code (e.g. CS302): ", sub->code, sizeof(sub->code));
    get_string_input("  -> Subject Title: ", sub->name, sizeof(sub->name));
    sub->credits = get_int_input("  -> Credit Hours (1 to 6): ", 1, 6);
    sub->semester = get_int_input("  -> Semester for Course (1 to 8): ", 1, 8);
    sub->marks = get_float_input("  -> Marks Obtained (0.0 to 100.0): ", 0.0f, 100.0f);

    get_letter_grade(sub->marks, sub->grade, &sub->grade_point);
    s->subject_count++;

    // Recalculate student analytics immediately
    update_student_analytics(s);

    printf("\n" CLR_GREEN "  [+] Subject '%s' (%s) recorded successfully!\n", sub->name, sub->code);
    printf("      Marks: %.1f | Grade: %s | Grade Points: %d\n", sub->marks, sub->grade, sub->grade_point);
    printf("      Updated Student GPA: %.2f [%s]\n" CLR_RESET, s->gpa, s->performance_level);

    wait_for_enter();
    return true;
}

// Display tabular view of a student's subjects
void view_student_subjects(const Student *s) {
    if (!s) return;

    if (s->subject_count == 0) {
        printf("  " CLR_YELLOW "No course subjects currently enrolled for this student.\n" CLR_RESET);
        return;
    }

    printf(CLR_BOLD "  +----+------------+--------------------------------+---------+-----+-------+-------+--------+----------+\n");
    printf("  | No | Code       | Subject Title                  | Credits | Sem | Marks | Grade | Points | Status   |\n");
    printf("  +----+------------+--------------------------------+---------+-----+-------+-------+--------+----------+\n" CLR_RESET);

    for (int i = 0; i < s->subject_count; i++) {
        const Subject *sub = &s->subjects[i];
        const char *status = (sub->grade_point > 0) ? CLR_GREEN "PASS    " CLR_RESET : CLR_RED "FAIL    " CLR_RESET;

        const char *grade_color = CLR_WHITE;
        if (strcmp(sub->grade, "A+") == 0 || strcmp(sub->grade, "A") == 0) grade_color = CLR_GREEN;
        else if (strcmp(sub->grade, "B+") == 0 || strcmp(sub->grade, "B") == 0) grade_color = CLR_CYAN;
        else if (strcmp(sub->grade, "C") == 0) grade_color = CLR_YELLOW;
        else if (strcmp(sub->grade, "D") == 0) grade_color = CLR_MAGENTA;
        else if (strcmp(sub->grade, "F") == 0) grade_color = CLR_RED;

        printf("  | %2d | %-10s | %-30.30s |    %d    |  %d  | %5.1f | %s%-5s" CLR_RESET " |   %2d   | %s |\n",
               i + 1,
               sub->code,
               sub->name,
               sub->credits,
               sub->semester,
               sub->marks,
               grade_color,
               sub->grade,
               sub->grade_point,
               status);
    }
    printf("  +----+------------+--------------------------------+---------+-----+-------+-------+--------+----------+\n");
}
