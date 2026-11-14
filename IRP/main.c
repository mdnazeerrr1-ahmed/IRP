#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "student.h"
#include "gpa.h"
#include "file_handler.h"

// ANSI Color definitions
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

// Setup console for ANSI escape sequences and UTF-8 output on Windows
static void setup_console(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    SetConsoleOutputCP(CP_UTF8);
#endif
}

// Display website-like dashboard header and KPI summary bar
static void render_dashboard_header(const StudentDatabase *db) {
    clear_screen();

    printf(CLR_CYAN CLR_BOLD);
    printf("  +--------------------------------------------------------------------------------------------------+\n");
    printf("  |                     STUDENT ACADEMIC PERFORMANCE & GPA ANALYTICS ENGINE                          |\n");
    printf("  |                         Institutional Decision Support Dashboard v2.0                            |\n");
    printf("  +--------------------------------------------------------------------------------------------------+\n" CLR_RESET);

    // Compute live KPI metrics
    float gpa_sum = 0.0f;
    int deans_list_cnt = 0;
    int at_risk_cnt = 0;

    for (int i = 0; i < db->count; i++) {
        gpa_sum += db->students[i].gpa;
        if (db->students[i].gpa >= PERF_EXCELLENT_MIN) {
            deans_list_cnt++;
        } else if (db->students[i].gpa < PERF_AVERAGE_MIN) {
            at_risk_cnt++;
        }
    }

    float cohort_avg = (db->count > 0) ? (gpa_sum / (float)db->count) : 0.0f;

    printf(CLR_BOLD "  [ LIVE METRICS ] " CLR_RESET);
    printf("Enrolled: " CLR_CYAN "%d" CLR_RESET "  |  ", db->count);
    printf("Cohort Mean GPA: " CLR_CYAN "%.2f" CLR_RESET "  |  ", cohort_avg);
    printf("Dean's Honor Roll: " CLR_GREEN "%d" CLR_RESET "  |  ", deans_list_cnt);
    printf("Academic Warnings: " CLR_RED "%d" CLR_RESET "  |  ", at_risk_cnt);
    printf("Storage: " CLR_GREEN "ONLINE" CLR_RESET "\n");
    printf("  ----------------------------------------------------------------------------------------------------\n");
}

// Student Management Submenu handler
static void handle_student_management_menu(StudentDatabase *db) {
    while (1) {
        render_dashboard_header(db);
        printf(CLR_CYAN "  >>> DASHBOARD > STUDENT MANAGEMENT MODULE\n\n" CLR_RESET);
        printf("    [1] Enroll New Student Profile\n");
        printf("    [2] View All Registered Students Directory\n");
        printf("    [3] Search Student (by ID or Name Keyword)\n");
        printf("    [4] Update Student Profile Information\n");
        printf("    [5] Delete Student Record\n");
        printf("    [6] Return to Main Dashboard\n\n");

        int choice = get_int_input("  -> Select an option (1-6): ", 1, 6);
        if (choice == 1) {
            add_student(db);
            save_students_to_file(db, DEFAULT_DATA_FILE);
        } else if (choice == 2) {
            view_all_students(db);
        } else if (choice == 3) {
            search_student_menu(db);
        } else if (choice == 4) {
            if (update_student(db)) {
                save_students_to_file(db, DEFAULT_DATA_FILE);
            }
        } else if (choice == 5) {
            if (delete_student(db)) {
                save_students_to_file(db, DEFAULT_DATA_FILE);
            }
        } else {
            break;
        }
    }
}

// Display GPA Calculation explanation and grading scale reference
static void display_gpa_formula_and_scales(StudentDatabase *db) {
    clear_screen();
    printf(CLR_CYAN CLR_BOLD);
    printf("  ====================================================================================================\n");
    printf("                                GPA & CGPA MATHEMATICAL COMPUTATION ENGINE                            \n");
    printf("  ====================================================================================================\n" CLR_RESET);

    printf(CLR_BOLD "\n  [1] INSTITUTIONAL GRADING BENCHMARKS & POINT ASSIGNMENT:\n" CLR_RESET);
    printf("  +-----------------+--------------+--------------+------------------------------------------------+\n");
    printf("  | Marks Range     | Letter Grade | Grade Points | Performance Description                        |\n");
    printf("  +-----------------+--------------+--------------+------------------------------------------------+\n");
    printf("  |  90.0 - 100.0   |     " CLR_GREEN "A+" CLR_RESET "       |      10      | Outstanding Mastery (Dean's List)              |\n");
    printf("  |  80.0 -  89.9   |     " CLR_GREEN "A " CLR_RESET "       |       9      | Excellent Conceptual Understanding             |\n");
    printf("  |  70.0 -  79.9   |     " CLR_CYAN "B+" CLR_RESET "       |       8      | Very Good Academic Performance                 |\n");
    printf("  |  60.0 -  69.9   |     " CLR_CYAN "B " CLR_RESET "       |       7      | Good / Above Average Competence                |\n");
    printf("  |  50.0 -  59.9   |     " CLR_YELLOW "C " CLR_RESET "       |       6      | Average / Satisfactory Threshold               |\n");
    printf("  |  40.0 -  49.9   |     " CLR_MAGENTA "D " CLR_RESET "       |       5      | Marginal Pass / Subject Needs Attention        |\n");
    printf("  |   0.0 -  39.9   |     " CLR_RED "F " CLR_RESET "       |       0      | Fail / Mandatory Supplementary Exam            |\n");
    printf("  +-----------------+--------------+--------------+------------------------------------------------+\n");

    printf(CLR_BOLD "\n  [2] FORMULA DERIVATION:\n" CLR_RESET);
    printf("      Semester GPA (Grade Point Average):\n");
    printf(CLR_CYAN "          GPA = [ SUM( Grade_Point_i * Credit_Hours_i ) ] / [ SUM( Credit_Hours_i ) ]\n" CLR_RESET);
    printf("      Cumulative GPA (CGPA):\n");
    printf(CLR_CYAN "          CGPA = Total Quality Points Across All Terms / Total Credits Registered\n" CLR_RESET);

    printf(CLR_BOLD "\n  [3] PERFORMANCE CATEGORIZATION TIERS:\n" CLR_RESET);
    printf("      - " CLR_GREEN "Excellent" CLR_RESET "         : GPA >= 9.00\n");
    printf("      - " CLR_CYAN "Good" CLR_RESET "              : 7.50 <= GPA < 9.00\n");
    printf("      - " CLR_YELLOW "Average" CLR_RESET "           : 6.00 <= GPA < 7.50\n");
    printf("      - " CLR_RED "Needs Improvement" CLR_RESET " : GPA < 6.00 (Academic Counseling Required)\n");

    recalculate_database_analytics(db);
    printf("\n" CLR_GREEN "  [+] All student records in memory have been verified and recalculated.\n" CLR_RESET);

    wait_for_enter();
}

// Data persistence management menu
static void handle_data_persistence_menu(StudentDatabase *db) {
    while (1) {
        render_dashboard_header(db);
        printf(CLR_CYAN "  >>> DASHBOARD > DATA PERSISTENCE & STORAGE MANAGEMENT\n\n" CLR_RESET);
        printf("    [1] Save Active Database to Disk (%s)\n", DEFAULT_DATA_FILE);
        printf("    [2] Reload Records from Disk File\n");
        printf("    [3] Reset & Seed Fresh Sample Dataset (3 Diverse Profiles)\n");
        printf("    [4] Return to Main Dashboard\n\n");

        int choice = get_int_input("  -> Select an option (1-4): ", 1, 4);
        if (choice == 1) {
            if (save_students_to_file(db, DEFAULT_DATA_FILE)) {
                printf(CLR_GREEN "\n  [+] Successfully saved %d student records to '%s'.\n" CLR_RESET, db->count, DEFAULT_DATA_FILE);
            }
            wait_for_enter();
        } else if (choice == 2) {
            if (load_students_from_file(db, DEFAULT_DATA_FILE)) {
                printf(CLR_GREEN "\n  [+] Successfully reloaded %d student records from disk.\n" CLR_RESET, db->count);
            } else {
                printf(CLR_RED "\n  [!] Failed to reload records. Ensure '%s' exists.\n" CLR_RESET, DEFAULT_DATA_FILE);
            }
            wait_for_enter();
        } else if (choice == 3) {
            if (get_yes_no_input("\n  [?] Are you sure you want to overwrite current records with sample data?")) {
                seed_sample_dataset(db);
                save_students_to_file(db, DEFAULT_DATA_FILE);
                printf(CLR_GREEN "  [+] Seed dataset loaded and saved! (3 sample students initialized).\n" CLR_RESET);
            } else {
                printf(CLR_CYAN "  [i] Operation cancelled.\n" CLR_RESET);
            }
            wait_for_enter();
        } else {
            break;
        }
    }
}

// Program Entry Point
int main(void) {
    setup_console();

    StudentDatabase db;
    init_student_database(&db);

    // Initialize database from file or seed default dataset
    ensure_data_directory_exists();
    load_or_seed_database(&db, DEFAULT_DATA_FILE);

    while (1) {
        render_dashboard_header(&db);

        printf("\n" CLR_BOLD "  +-- SYSTEM NAVIGATION MENU ------------------------------------------------------------------------+\n" CLR_RESET);
        printf("  |  " CLR_CYAN "[1]" CLR_RESET " 📋 Student Directory Management   (Add, View, Search, Update, Delete Records)       |\n");
        printf("  |  " CLR_CYAN "[2]" CLR_RESET " 📚 Subject & Marks Entry           (Add Course, Enter Credits & Marks, Auto-Grade)  |\n");
        printf("  |  " CLR_CYAN "[3]" CLR_RESET " ⚡ GPA & CGPA Calculator Engine   (Grading Scales, Formula Derivation, Recalculate)  |\n");
        printf("  |  " CLR_CYAN "[4]" CLR_RESET " 📊 Academic Performance Analytics  (Strengths, Weaknesses, Benchmarks, Suggestions)   |\n");
        printf("  |  " CLR_CYAN "[5]" CLR_RESET " 📑 Student Official Report Card    (Full Individual Academic Transcript & Plan)       |\n");
        printf("  |  " CLR_CYAN "[6]" CLR_RESET " 📈 Institutional Cohort Analytics  (Class Averages, Toppers, Risk Distribution)       |\n");
        printf("  |  " CLR_CYAN "[7]" CLR_RESET " 💾 Data Persistence & File Center  (Save Records, Reload, Seed Sample Data)           |\n");
        printf("  |  " CLR_RED  "[8]" CLR_RESET " 🚪 Exit System                                                                        |\n");
        printf(CLR_BOLD "  +--------------------------------------------------------------------------------------------------+\n" CLR_RESET);

        int choice = get_int_input("\n  -> Enter your command choice (1-8): ", 1, 8);

        switch (choice) {
            case 1:
                handle_student_management_menu(&db);
                break;

            case 2:
                add_subject_to_student(&db);
                save_students_to_file(&db, DEFAULT_DATA_FILE);
                break;

            case 3:
                display_gpa_formula_and_scales(&db);
                break;

            case 4: {
                if (db.count == 0) {
                    printf(CLR_YELLOW "\n  [i] No students in database. Please enroll students first.\n" CLR_RESET);
                    wait_for_enter();
                } else {
                    char id[MAX_ID_LEN];
                    get_string_input("\n  -> Enter Student ID for Performance Analytics: ", id, sizeof(id));
                    int idx = find_student_by_id(&db, id);
                    if (idx != -1) {
                        display_student_analytics(&db.students[idx]);
                    } else {
                        printf(CLR_RED "  [!] Student with ID '%s' not found.\n" CLR_RESET, id);
                        wait_for_enter();
                    }
                }
                break;
            }

            case 5: {
                if (db.count == 0) {
                    printf(CLR_YELLOW "\n  [i] No students in database. Please enroll students first.\n" CLR_RESET);
                    wait_for_enter();
                } else {
                    char id[MAX_ID_LEN];
                    get_string_input("\n  -> Enter Student ID to generate Report Card: ", id, sizeof(id));
                    int idx = find_student_by_id(&db, id);
                    if (idx != -1) {
                        print_student_report_card(&db.students[idx]);
                    } else {
                        printf(CLR_RED "  [!] Student with ID '%s' not found.\n" CLR_RESET, id);
                        wait_for_enter();
                    }
                }
                break;
            }

            case 6:
                print_analytics_summary_dashboard(&db);
                break;

            case 7:
                handle_data_persistence_menu(&db);
                break;

            case 8: {
                // Auto-save on exit
                save_students_to_file(&db, DEFAULT_DATA_FILE);

                clear_screen();
                printf("\n" CLR_CYAN CLR_BOLD);
                printf("  +--------------------------------------------------------------------------------------------------+\n");
                printf("  |                                                                                                  |\n");
                printf("  |       Thank you for using Student Academic Performance & GPA Analytics Engine!                   |\n");
                printf("  |                       Empowering Informed Institutional Decisions                                |\n");
                printf("  |                                                                                                  |\n");
                printf("  +--------------------------------------------------------------------------------------------------+\n" CLR_RESET);
                printf("\n  " CLR_GREEN "[+] Active student records safely committed to '%s'. Goodbye!\n\n" CLR_RESET, DEFAULT_DATA_FILE);
                return 0;
            }

            default:
                break;
        }
    }

    return 0;
}
