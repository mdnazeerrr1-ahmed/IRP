#ifndef GPA_H
#define GPA_H

#include "student.h"

// Grade boundary definitions
#define GRADE_APLUS_MIN 90.0f
#define GRADE_A_MIN     80.0f
#define GRADE_BPLUS_MIN 70.0f
#define GRADE_B_MIN     60.0f
#define GRADE_C_MIN     50.0f
#define GRADE_D_MIN     40.0f

// Performance categorization thresholds
#define PERF_EXCELLENT_MIN 9.0f
#define PERF_GOOD_MIN      7.5f
#define PERF_AVERAGE_MIN   6.0f

// Grade mapping helper
void get_letter_grade(float marks, char *grade_out, int *point_out);

// GPA & CGPA calculation functions (Member 2)
float calculate_student_gpa(Student *s);
float calculate_student_cgpa(Student *s);
void update_student_analytics(Student *s);
void recalculate_database_analytics(StudentDatabase *db);

// Analytical insights functions (Member 2)
const Subject* get_highest_scoring_subject(const Student *s);
const Subject* get_lowest_scoring_subject(const Student *s);
float calculate_average_marks(const Student *s);
int count_failed_subjects(const Student *s);
int count_weak_subjects(const Student *s);
void determine_performance_level(float gpa, char *level_out, int max_len);

// Display & Report functions (Member 2 & 3)
void display_student_analytics(const Student *s);
void display_improvement_suggestions(const Student *s);
void print_student_report_card(const Student *s);
void print_analytics_summary_dashboard(const StudentDatabase *db);

#endif // GPA_H
