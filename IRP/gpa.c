#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpa.h"
#include "student.h"

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

// Convert numeric marks into letter grade and grade points according to standard scale
void get_letter_grade(float marks, char *grade_out, int *point_out) {
    if (marks >= GRADE_APLUS_MIN) {
        strcpy(grade_out, "A+");
        *point_out = 10;
    } else if (marks >= GRADE_A_MIN) {
        strcpy(grade_out, "A");
        *point_out = 9;
    } else if (marks >= GRADE_BPLUS_MIN) {
        strcpy(grade_out, "B+");
        *point_out = 8;
    } else if (marks >= GRADE_B_MIN) {
        strcpy(grade_out, "B");
        *point_out = 7;
    } else if (marks >= GRADE_C_MIN) {
        strcpy(grade_out, "C");
        *point_out = 6;
    } else if (marks >= GRADE_D_MIN) {
        strcpy(grade_out, "D");
        *point_out = 5;
    } else {
        strcpy(grade_out, "F");
        *point_out = 0;
    }
}

// Calculate GPA based on current semester or all subjects
// Formula: GPA = Sum(Grade Point * Credits) / Sum(Credits)
float calculate_student_gpa(Student *s) {
    if (!s || s->subject_count == 0) return 0.0f;

    float total_weighted_points = 0.0f;
    int total_credits = 0;

    for (int i = 0; i < s->subject_count; i++) {
        // Ensure grade and points are synchronized
        get_letter_grade(s->subjects[i].marks, s->subjects[i].grade, &s->subjects[i].grade_point);

        total_weighted_points += (float)(s->subjects[i].grade_point * s->subjects[i].credits);
        total_credits += s->subjects[i].credits;
    }

    if (total_credits == 0) return 0.0f;
    return total_weighted_points / (float)total_credits;
}

// Calculate cumulative GPA (CGPA)
float calculate_student_cgpa(Student *s) {
    // For single-phase records, CGPA aligns with total credit-weighted GPA
    return calculate_student_gpa(s);
}

// Categorize student performance level based on GPA
void determine_performance_level(float gpa, char *level_out, int max_len) {
    if (gpa >= PERF_EXCELLENT_MIN) {
        strncpy(level_out, "Excellent", max_len - 1);
    } else if (gpa >= PERF_GOOD_MIN) {
        strncpy(level_out, "Good", max_len - 1);
    } else if (gpa >= PERF_AVERAGE_MIN) {
        strncpy(level_out, "Average", max_len - 1);
    } else {
        strncpy(level_out, "Needs Improvement", max_len - 1);
    }
    level_out[max_len - 1] = '\0';
}

// Update all GPA and performance analytics for a single student
void update_student_analytics(Student *s) {
    if (!s) return;
    if (s->subject_count == 0) {
        s->gpa = 0.0f;
        s->cgpa = 0.0f;
        strncpy(s->performance_level, "Not Evaluated", sizeof(s->performance_level));
        return;
    }

    s->gpa = calculate_student_gpa(s);
    s->cgpa = calculate_student_cgpa(s);
    determine_performance_level(s->gpa, s->performance_level, sizeof(s->performance_level));
}

// Recalculate analytics for all students in the database
void recalculate_database_analytics(StudentDatabase *db) {
    if (!db) return;
    for (int i = 0; i < db->count; i++) {
        update_student_analytics(&db->students[i]);
    }
}

// Find subject with highest marks
const Subject* get_highest_scoring_subject(const Student *s) {
    if (!s || s->subject_count == 0) return NULL;
    int best_idx = 0;
    for (int i = 1; i < s->subject_count; i++) {
        if (s->subjects[i].marks > s->subjects[best_idx].marks) {
            best_idx = i;
        }
    }
    return &s->subjects[best_idx];
}

// Find subject with lowest marks
const Subject* get_lowest_scoring_subject(const Student *s) {
    if (!s || s->subject_count == 0) return NULL;
    int worst_idx = 0;
    for (int i = 1; i < s->subject_count; i++) {
        if (s->subjects[i].marks < s->subjects[worst_idx].marks) {
            worst_idx = i;
        }
    }
    return &s->subjects[worst_idx];
}

// Calculate arithmetic mean of marks
float calculate_average_marks(const Student *s) {
    if (!s || s->subject_count == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < s->subject_count; i++) {
        sum += s->subjects[i].marks;
    }
    return sum / (float)s->subject_count;
}

// Count number of subjects with Grade F (marks < 40)
int count_failed_subjects(const Student *s) {
    if (!s) return 0;
    int fails = 0;
    for (int i = 0; i < s->subject_count; i++) {
        if (s->subjects[i].marks < GRADE_D_MIN) {
            fails++;
        }
    }
    return fails;
}

// Count number of weak subjects (marks < 50)
int count_weak_subjects(const Student *s) {
    if (!s) return 0;
    int weak = 0;
    for (int i = 0; i < s->subject_count; i++) {
        if (s->subjects[i].marks < GRADE_C_MIN) {
            weak++;
        }
    }
    return weak;
}

// Render analytical insights module
void display_student_analytics(const Student *s) {
    if (!s) return;

    if (s->subject_count == 0) {
        printf("\n" CLR_YELLOW "  [i] Student has no registered subjects to analyze.\n" CLR_RESET);
        wait_for_enter();
        return;
    }

    const Subject *highest = get_highest_scoring_subject(s);
    const Subject *lowest = get_lowest_scoring_subject(s);
    float avg_marks = calculate_average_marks(s);
    int failed = count_failed_subjects(s);
    int weak = count_weak_subjects(s);

    int total_credits_attempted = 0;
    int total_credits_earned = 0;
    for (int i = 0; i < s->subject_count; i++) {
        total_credits_attempted += s->subjects[i].credits;
        if (s->subjects[i].grade_point > 0) {
            total_credits_earned += s->subjects[i].credits;
        }
    }

    printf("\n" CLR_CYAN CLR_BOLD);
    printf("  <====================================================================================================>\n");
    printf("                           ACADEMIC PERFORMANCE ANALYTICS & INSIGHTS ENGINE 📑                         \n");
    printf("  <====================================================================================================>\n" CLR_RESET);

    printf("  Student: " CLR_BOLD "%-25s" CLR_RESET " | ID: " CLR_CYAN "%-10s" CLR_RESET " | Dept: %-15s | Sem: %d\n",
           s->name, s->id, s->department, s->current_semester);
    printf("  ----------------------------------------------------------------------------------------------------\n\n");

    // Subject breakdown table
    view_student_subjects(s);

    printf("\n" CLR_BOLD "  --- STATISTICAL PERFORMANCE METRICS ---" CLR_RESET "\n");
    printf("  [+] Grade Point Average (GPA)   : " CLR_CYAN "%.2f / 10.00\n" CLR_RESET, s->gpa);
    printf("  [+] Cumulative GPA (CGPA)       : " CLR_CYAN "%.2f / 10.00\n" CLR_RESET, s->cgpa);
    printf("  [+] Arithmetic Mean Marks       : %.2f / 100.00\n", avg_marks);
    printf("  [+] Total Credits Attempted     : %d credits\n", total_credits_attempted);
    printf("  [+] Total Credits Earned        : %d credits (%s)\n",
           total_credits_earned, (total_credits_earned == total_credits_attempted) ? CLR_GREEN "100% Cleared" CLR_RESET : CLR_RED "Incomplete" CLR_RESET);

    if (highest) {
        printf("  [+] Highest-Scoring Subject     : " CLR_GREEN "%s (%s)" CLR_RESET " with %.1f marks (Grade %s, %d Pts)\n",
               highest->name, highest->code, highest->marks, highest->grade, highest->grade_point);
    }
    if (lowest) {
        const char *lcolor = (lowest->marks < GRADE_D_MIN) ? CLR_RED : CLR_YELLOW;
        printf("  [+] Lowest-Scoring Subject      : %s%s (%s)" CLR_RESET " with %.1f marks (Grade %s, %d Pts)\n",
               lcolor, lowest->name, lowest->code, lowest->marks, lowest->grade, lowest->grade_point);
    }

    printf("  [+] Failed Subjects (Grade F)   : ");
    if (failed > 0) {
        printf(CLR_RED CLR_BOLD "%d subject(s) require remediation\n" CLR_RESET, failed);
    } else {
        printf(CLR_GREEN "0 subjects (Clean Academic Record)\n" CLR_RESET);
    }

    printf("  [+] Weak Subjects (< 50 marks)  : ");
    if (weak > 0) {
        printf(CLR_YELLOW "%d subject(s) identified for academic support\n" CLR_RESET, weak);
    } else {
        printf(CLR_GREEN "0 subjects (All competencies above baseline)\n" CLR_RESET);
    }

    // Performance badge
    const char *badge_color = CLR_WHITE;
    if (strcmp(s->performance_level, "Excellent") == 0) badge_color = CLR_GREEN;
    else if (strcmp(s->performance_level, "Good") == 0) badge_color = CLR_CYAN;
    else if (strcmp(s->performance_level, "Average") == 0) badge_color = CLR_YELLOW;
    else if (strcmp(s->performance_level, "Needs Improvement") == 0) badge_color = CLR_RED;

    printf("\n  [+] Overall Standing Category   : %s" CLR_BOLD "[ %s ]" CLR_RESET "\n", badge_color, s->performance_level);

    display_improvement_suggestions(s);

    wait_for_enter();
}

// Generate personalized, actionable academic improvement suggestions
void display_improvement_suggestions(const Student *s) {
    if (!s) return;

    printf("\n" CLR_BOLD " << ====================================================================================================>>\n");
    printf("                               PERSONALIZED ACADEMIC IMPROVEMENT PLAN  🛄                              \n");
    printf(" << ====================================================================================================>>\n" CLR_RESET);

    int failed = count_failed_subjects(s);
    int weak = count_weak_subjects(s);

    if (failed > 0) {
        printf(CLR_RED "  [CRITICAL ACTION REQUIRED - BACKLOGS DETECTED]\n" CLR_RESET);
        printf("  * The following course(s) require immediate remediation / re-examination:\n");
        for (int i = 0; i < s->subject_count; i++) {
            if (s->subjects[i].marks < GRADE_D_MIN) {
                printf(CLR_RED "    - %s (%s): Scored %.1f marks (Credit weight: %d)\n" CLR_RESET,
                       s->subjects[i].name, s->subjects[i].code, s->subjects[i].marks, s->subjects[i].credits);
            }
        }
        printf("  * Action Steps:\n");
        printf("    1. Consult course instructors during office hours for targeted doubt clearance.\n");
        printf("    2. Register for the upcoming supplementary examination window.\n");
        printf("    3. Dedicate at least 6 hours/week on foundational problem sets for these failed courses.\n\n");
    }

    if (weak > failed) {
        printf(CLR_YELLOW "  [TARGETED SUBJECT INTERVENTIONS]\n" CLR_RESET);
        printf("  * Courses with scores between 40 and 49 (Grade D) that pull down overall GPA:\n");
        for (int i = 0; i < s->subject_count; i++) {
            if (s->subjects[i].marks >= GRADE_D_MIN && s->subjects[i].marks < GRADE_C_MIN) {
                printf(CLR_YELLOW "    - %s (%s): Scored %.1f marks (Grade: D)\n" CLR_RESET,
                       s->subjects[i].name, s->subjects[i].code, s->subjects[i].marks);
            }
        }
        printf("  * Recommendation: Form peer study groups and focus on past exam question papers.\n\n");
    }

    printf(CLR_CYAN "  [STRATEGIC GPA ROADMAP]\n" CLR_RESET);
    if (s->gpa >= PERF_EXCELLENT_MIN) {
        printf("  * " CLR_GREEN "Outstanding Performance!" CLR_RESET " You are on track for the Dean's Honor Roll.\n");
        printf("  * Suggestions:\n");
        printf("    1. Consider serving as a Peer Academic Tutor for junior students.\n");
        printf("    2. Explore undergraduate research publications, internships, or competitive programming.\n");
        printf("    3. Maintain consistent weekly study rhythms to safeguard your GPA above 9.0.\n");
    } else if (s->gpa >= PERF_GOOD_MIN) {
        printf("  * " CLR_CYAN "Strong and Consistent Standing." CLR_RESET " You have solid conceptual foundations.\n");
        printf("  * Suggestions:\n");
        printf("    1. Focus on higher-credit courses (3+ credits); minor boosts in them will elevate GPA to 9.0+.\n");
        printf("    2. Conduct active recall and timed mock tests before university examinations.\n");
        printf("    3. Convert B/B+ courses into A/A+ by targeting assignment deadlines and internal tests.\n");
    } else if (s->gpa >= PERF_AVERAGE_MIN) {
        printf("  * " CLR_YELLOW "Moderate Academic Standing." CLR_RESET " Good potential with significant room for growth.\n");
        printf("  * Suggestions:\n");
        printf("    1. Create a structured daily 2-hour revision calendar covering theoretical & numerical topics.\n");
        printf("    2. Complete textbook reference problems rather than relying solely on lecture slides.\n");
        printf("    3. Seek regular feedback from course professors on midterm exam deficiencies.\n");
    } else {
        printf("  * " CLR_RED "Academic Probation / At-Risk Notice." CLR_RESET " Immediate turnaround is essential.\n");
        printf("  * Suggestions:\n");
        printf("    1. Meet with the Academic Department Counselor for an academic recovery contract.\n");
        printf("    2. Limit non-academic extracurricular commitments until core courses are stabilized.\n");
        printf("    3. Re-evaluate study methodology: shift from passive reading to active problem solving.\n");
    }
}

// Generate an official-style Student Performance Report Card
void print_student_report_card(const Student *s) {
    if (!s) return;

    printf("\n" CLR_CYAN CLR_BOLD);
    printf(" { +--------------------------------------------------------------------------------------------------+\n");
    printf("  |                     ✨✨OFFICIAL STUDENT ACADEMIC TRANSCRIPT & PERFORMANCE REPORT ✨✨                  |\n");
    printf("  { +--------------------------------------------------------------------------------------------------+\n" CLR_RESET);

    printf("  Student Full Name  : " CLR_BOLD "%-35s" CLR_RESET " Registration ID  : " CLR_CYAN "%-15s" CLR_RESET "\n", s->name, s->id);
    printf("  Department/Branch  : %-35s Academic Year    : %d (Semester %d)\n", s->department, s->academic_year, s->current_semester);
    printf("  Enrollment Status  : Active Degree Candidate            Evaluation Date  : Current Term\n");
    printf("  +--------------------------------------------------------------------------------------------------+\n");

    if (s->subject_count == 0) {
        printf("  " CLR_YELLOW "No enrolled courses recorded for this student.\n" CLR_RESET);
    } else {
        view_student_subjects(s);

        int total_credits = 0;
        int total_points = 0;
        for (int i = 0; i < s->subject_count; i++) {
            total_credits += s->subjects[i].credits;
            total_points += (s->subjects[i].grade_point * s->subjects[i].credits);
        }

        const char *perf_color = CLR_WHITE;
        if (strcmp(s->performance_level, "Excellent") == 0) perf_color = CLR_GREEN;
        else if (strcmp(s->performance_level, "Good") == 0) perf_color = CLR_CYAN;
        else if (strcmp(s->performance_level, "Average") == 0) perf_color = CLR_YELLOW;
        else if (strcmp(s->performance_level, "Needs Improvement") == 0) perf_color = CLR_RED;

        printf("  +--------------------------------------------------------------------------------------------------+\n");
        printf("  | SUMMARY ACADEMIC METRICS:                                                                        |\n");
        printf("  |  * Total Credits Registered : %-3d                 * Total Quality Points Earned : %-4d            |\n", total_credits, total_points);
        printf("  |  * Semester GPA             : " CLR_CYAN CLR_BOLD "%-4.2f / 10.00" CLR_RESET "        * Cumulative CGPA             : " CLR_CYAN CLR_BOLD "%-4.2f / 10.00" CLR_RESET " |\n", s->gpa, s->cgpa);
        printf("  |  * Average Marks Percentage : %-5.2f %%             * Performance Tier Classification: %s%-17.17s" CLR_RESET " |\n",
               calculate_average_marks(s), perf_color, s->performance_level);
        printf("  +--------------------------------------------------------------------------------------------------+\n");

        display_improvement_suggestions(s);
    }

    wait_for_enter();
}

// Cohort analytics dashboard across all students
void print_analytics_summary_dashboard(const StudentDatabase *db) {
    if (!db || db->count == 0) {
        printf("\n" CLR_YELLOW "  [i] Student database is empty. No aggregate analytics available.\n" CLR_RESET);
        wait_for_enter();
        return;
    }

    float total_gpa_sum = 0.0f;
    int excellent_cnt = 0;
    int good_cnt = 0;
    int average_cnt = 0;
    int remediation_cnt = 0;

    int topper_idx = 0;
    int lowest_idx = 0;

    for (int i = 0; i < db->count; i++) {
        const Student *s = &db->students[i];
        total_gpa_sum += s->gpa;

        if (s->gpa > db->students[topper_idx].gpa) topper_idx = i;
        if (s->gpa < db->students[lowest_idx].gpa) lowest_idx = i;

        if (strcmp(s->performance_level, "Excellent") == 0) excellent_cnt++;
        else if (strcmp(s->performance_level, "Good") == 0) good_cnt++;
        else if (strcmp(s->performance_level, "Average") == 0) average_cnt++;
        else remediation_cnt++;
    }

    float class_avg_gpa = total_gpa_sum / (float)db->count;

    printf("\n" CLR_CYAN CLR_BOLD);
    printf("  ====================================================================================================\n");
    printf("                               INSTITUTIONAL COHORT ANALYTICS OVERVIEW  🎒🎓                              \n");
    printf("  ====================================================================================================\n" CLR_RESET);

    printf(CLR_BOLD "  +-------------------------------------+-----------------------------------------------------+\n");
    printf("  | METRIC                              | VALUE                                               |\n");
    printf("  +-------------------------------------+-----------------------------------------------------+\n" CLR_RESET);
    printf("  | Total Registered Cohort Size        | %-51d |\n", db->count);
    printf("  | Class Mean GPA                      | " CLR_CYAN "%.2f / 10.00" CLR_RESET "                                         |\n", class_avg_gpa);
    printf("  | Highest Performing Student (Val.)   | " CLR_GREEN "%-28.28s (GPA: %.2f)" CLR_RESET "     |\n",
           db->students[topper_idx].name, db->students[topper_idx].gpa);
    printf("  | Lowest Scoring Student              | " CLR_RED "%-28.28s (GPA: %.2f)" CLR_RESET "     |\n",
           db->students[lowest_idx].name, db->students[lowest_idx].gpa);
    printf("  +-------------------------------------+-----------------------------------------------------+\n\n");

    printf(CLR_BOLD "  --- PERFORMANCE TIER DISTRIBUTION ---" CLR_RESET "\n");
    printf("  [+] " CLR_GREEN "Excellent (GPA >= 9.0)" CLR_RESET "         : %2d student(s) [%4.1f%%]\n",
           excellent_cnt, (float)excellent_cnt * 100.0f / db->count);
    printf("  [+] " CLR_CYAN "Good (7.5 <= GPA < 9.0)" CLR_RESET "        : %2d student(s) [%4.1f%%]\n",
           good_cnt, (float)good_cnt * 100.0f / db->count);
    printf("  [+] " CLR_YELLOW "Average (6.0 <= GPA < 7.5)" CLR_RESET "     : %2d student(s) [%4.1f%%]\n",
           average_cnt, (float)average_cnt * 100.0f / db->count);
    printf("  [+] " CLR_RED "Needs Improvement (GPA < 6.0)" CLR_RESET " : %2d student(s) [%4.1f%%]\n",
           remediation_cnt, (float)remediation_cnt * 100.0f / db->count);

    if (remediation_cnt > 0) {
        printf("\n" CLR_RED CLR_BOLD "  [!] AT-RISK STUDENTS IDENTIFIED FOR ACADEMIC COUNSELING:" CLR_RESET "\n");
        for (int i = 0; i < db->count; i++) {
            if (strcmp(db->students[i].performance_level, "Needs Improvement") == 0) {
                printf(CLR_RED "      - %s (ID: %s) | GPA: %.2f | Failed Courses: %d\n" CLR_RESET,
                       db->students[i].name, db->students[i].id, db->students[i].gpa, count_failed_subjects(&db->students[i]));
            }
        }
    }

    wait_for_enter();
}
