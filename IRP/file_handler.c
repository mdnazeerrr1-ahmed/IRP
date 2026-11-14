#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "gpa.h"

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIRECTORY(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MAKE_DIRECTORY(path) mkdir(path, 0777)
#endif

// ANSI Color macros
#define CLR_RESET   "\033[0m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_RED     "\033[31m"
#define CLR_CYAN    "\033[36m"

// Ensure the data storage directory exists
void ensure_data_directory_exists(void) {
    MAKE_DIRECTORY("data");
}

// Save student records to persistent text file
bool save_students_to_file(const StudentDatabase *db, const char *filepath) {
    if (!db) return false;

    ensure_data_directory_exists();

    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        printf(CLR_RED "  [!] Error opening file '%s' for writing.\n" CLR_RESET, filepath);
        return false;
    }

    fprintf(fp, "# STUDENT_ACADEMIC_ANALYTICS_DATA_V1\n");
    fprintf(fp, "STUDENT_COUNT %d\n", db->count);

    for (int i = 0; i < db->count; i++) {
        const Student *s = &db->students[i];
        fprintf(fp, "STUDENT|%s|%s|%s|%d|%d|%d|%.2f|%.2f|%s\n",
                s->id,
                s->name,
                s->department,
                s->academic_year,
                s->current_semester,
                s->subject_count,
                s->gpa,
                s->cgpa,
                s->performance_level);

        for (int j = 0; j < s->subject_count; j++) {
            const Subject *sub = &s->subjects[j];
            fprintf(fp, "SUBJECT|%s|%s|%d|%d|%.2f|%s|%d\n",
                    sub->code,
                    sub->name,
                    sub->credits,
                    sub->semester,
                    sub->marks,
                    sub->grade,
                    sub->grade_point);
        }
        fprintf(fp, "END_STUDENT\n");
    }

    fclose(fp);
    return true;
}

// Load student records from persistent text file
bool load_students_from_file(StudentDatabase *db, const char *filepath) {
    if (!db) return false;

    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        return false; // File doesn't exist yet
    }

    char line[512];
    db->count = 0;

    Student current_student;
    memset(&current_student, 0, sizeof(Student));
    bool in_student = false;

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Strip trailing newline characters
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (len == 0 || line[0] == '#') continue;

        if (strncmp(line, "STUDENT_COUNT", 13) == 0) {
            continue;
        }

        if (strncmp(line, "STUDENT|", 8) == 0) {
            memset(&current_student, 0, sizeof(Student));
            char *token = strtok(line + 8, "|");
            if (token) strncpy(current_student.id, token, sizeof(current_student.id) - 1);

            token = strtok(NULL, "|");
            if (token) strncpy(current_student.name, token, sizeof(current_student.name) - 1);

            token = strtok(NULL, "|");
            if (token) strncpy(current_student.department, token, sizeof(current_student.department) - 1);

            token = strtok(NULL, "|");
            if (token) current_student.academic_year = atoi(token);

            token = strtok(NULL, "|");
            if (token) current_student.current_semester = atoi(token);

            token = strtok(NULL, "|");
            int expected_subs = token ? atoi(token) : 0;
            (void)expected_subs;

            token = strtok(NULL, "|");
            if (token) current_student.gpa = (float)atof(token);

            token = strtok(NULL, "|");
            if (token) current_student.cgpa = (float)atof(token);

            token = strtok(NULL, "|");
            if (token) strncpy(current_student.performance_level, token, sizeof(current_student.performance_level) - 1);

            current_student.subject_count = 0;
            in_student = true;
        } else if (strncmp(line, "SUBJECT|", 8) == 0 && in_student) {
            if (current_student.subject_count < MAX_SUBJECTS) {
                Subject *sub = &current_student.subjects[current_student.subject_count];
                char *token = strtok(line + 8, "|");
                if (token) strncpy(sub->code, token, sizeof(sub->code) - 1);

                token = strtok(NULL, "|");
                if (token) strncpy(sub->name, token, sizeof(sub->name) - 1);

                token = strtok(NULL, "|");
                if (token) sub->credits = atoi(token);

                token = strtok(NULL, "|");
                if (token) sub->semester = atoi(token);

                token = strtok(NULL, "|");
                if (token) sub->marks = (float)atof(token);

                token = strtok(NULL, "|");
                if (token) strncpy(sub->grade, token, sizeof(sub->grade) - 1);

                token = strtok(NULL, "|");
                if (token) sub->grade_point = atoi(token);

                current_student.subject_count++;
            }
        } else if (strncmp(line, "END_STUDENT", 11) == 0 && in_student) {
            if (db->count < MAX_STUDENTS) {
                // Ensure analytical metrics are synchronized
                update_student_analytics(&current_student);
                db->students[db->count++] = current_student;
            }
            in_student = false;
        }
    }

    fclose(fp);
    return true;
}

// Seed realistic sample data for 3 diverse student personas
void seed_sample_dataset(StudentDatabase *db) {
    if (!db) return;

    db->count = 0;

    // --- Student 1: Alex Johnson (Computer Science) - High Achiever (Dean's Honor Roll) ---
    Student *s1 = &db->students[db->count++];
    strncpy(s1->id, "STU-1001", sizeof(s1->id) - 1);
    strncpy(s1->name, "Alex Johnson", sizeof(s1->name) - 1);
    strncpy(s1->department, "Computer Science", sizeof(s1->department) - 1);
    s1->academic_year = 2;
    s1->current_semester = 4;
    s1->subject_count = 5;

    // Subjects for Alex
    strncpy(s1->subjects[0].code, "CS401", sizeof(s1->subjects[0].code) - 1);
    strncpy(s1->subjects[0].name, "Data Structures & Algorithms", sizeof(s1->subjects[0].name) - 1);
    s1->subjects[0].credits = 4;
    s1->subjects[0].semester = 4;
    s1->subjects[0].marks = 94.5f;

    strncpy(s1->subjects[1].code, "CS402", sizeof(s1->subjects[1].code) - 1);
    strncpy(s1->subjects[1].name, "Database Management Systems", sizeof(s1->subjects[1].name) - 1);
    s1->subjects[1].credits = 4;
    s1->subjects[1].semester = 4;
    s1->subjects[1].marks = 88.0f;

    strncpy(s1->subjects[2].code, "CS403", sizeof(s1->subjects[2].code) - 1);
    strncpy(s1->subjects[2].name, "Operating Systems Architecture", sizeof(s1->subjects[2].name) - 1);
    s1->subjects[2].credits = 4;
    s1->subjects[2].semester = 4;
    s1->subjects[2].marks = 91.0f;

    strncpy(s1->subjects[3].code, "MA401", sizeof(s1->subjects[3].code) - 1);
    strncpy(s1->subjects[3].name, "Discrete Mathematics & Logic", sizeof(s1->subjects[3].name) - 1);
    s1->subjects[3].credits = 3;
    s1->subjects[3].semester = 4;
    s1->subjects[3].marks = 96.0f;

    strncpy(s1->subjects[4].code, "CS405", sizeof(s1->subjects[4].code) - 1);
    strncpy(s1->subjects[4].name, "Software Engineering Lab", sizeof(s1->subjects[4].name) - 1);
    s1->subjects[4].credits = 2;
    s1->subjects[4].semester = 4;
    s1->subjects[4].marks = 92.0f;

    update_student_analytics(s1);

    // --- Student 2: Priya Patel (Electronics & Comm.) - Consistent Good Performer ---
    Student *s2 = &db->students[db->count++];
    strncpy(s2->id, "STU-1002", sizeof(s2->id) - 1);
    strncpy(s2->name, "Priya Patel", sizeof(s2->name) - 1);
    strncpy(s2->department, "Electronics & Comm.", sizeof(s2->department) - 1);
    s2->academic_year = 2;
    s2->current_semester = 4;
    s2->subject_count = 5;

    // Subjects for Priya
    strncpy(s2->subjects[0].code, "EC401", sizeof(s2->subjects[0].code) - 1);
    strncpy(s2->subjects[0].name, "Signals and Linear Systems", sizeof(s2->subjects[0].name) - 1);
    s2->subjects[0].credits = 4;
    s2->subjects[0].semester = 4;
    s2->subjects[0].marks = 82.5f;

    strncpy(s2->subjects[1].code, "EC402", sizeof(s2->subjects[1].code) - 1);
    strncpy(s2->subjects[1].name, "Microprocessor & Interfaces", sizeof(s2->subjects[1].name) - 1);
    s2->subjects[1].credits = 4;
    s2->subjects[1].semester = 4;
    s2->subjects[1].marks = 76.0f;

    strncpy(s2->subjects[2].code, "EC403", sizeof(s2->subjects[2].code) - 1);
    strncpy(s2->subjects[2].name, "Electromagnetic Field Theory", sizeof(s2->subjects[2].name) - 1);
    s2->subjects[2].credits = 3;
    s2->subjects[2].semester = 4;
    s2->subjects[2].marks = 71.0f;

    strncpy(s2->subjects[3].code, "EC404", sizeof(s2->subjects[3].code) - 1);
    strncpy(s2->subjects[3].name, "Analog Circuit Design", sizeof(s2->subjects[3].name) - 1);
    s2->subjects[3].credits = 4;
    s2->subjects[3].semester = 4;
    s2->subjects[3].marks = 85.0f;

    strncpy(s2->subjects[4].code, "EC405", sizeof(s2->subjects[4].code) - 1);
    strncpy(s2->subjects[4].name, "Hardware Simulation Lab", sizeof(s2->subjects[4].name) - 1);
    s2->subjects[4].credits = 2;
    s2->subjects[4].semester = 4;
    s2->subjects[4].marks = 88.0f;

    update_student_analytics(s2);

    // --- Student 3: Michael Brown (Mechanical Engg.) - Needs Improvement / Backlogs ---
    Student *s3 = &db->students[db->count++];
    strncpy(s3->id, "STU-1003", sizeof(s3->id) - 1);
    strncpy(s3->name, "Michael Brown", sizeof(s3->name) - 1);
    strncpy(s3->department, "Mechanical Engg.", sizeof(s3->department) - 1);
    s3->academic_year = 2;
    s3->current_semester = 4;
    s3->subject_count = 5;

    // Subjects for Michael
    strncpy(s3->subjects[0].code, "ME401", sizeof(s3->subjects[0].code) - 1);
    strncpy(s3->subjects[0].name, "Applied Thermodynamics", sizeof(s3->subjects[0].name) - 1);
    s3->subjects[0].credits = 4;
    s3->subjects[0].semester = 4;
    s3->subjects[0].marks = 34.0f; // FAILED (< 40)

    strncpy(s3->subjects[1].code, "ME402", sizeof(s3->subjects[1].code) - 1);
    strncpy(s3->subjects[1].name, "Fluid Mechanics & Machinery", sizeof(s3->subjects[1].name) - 1);
    s3->subjects[1].credits = 4;
    s3->subjects[1].semester = 4;
    s3->subjects[1].marks = 46.0f; // WEAK (40-49, Grade D)

    strncpy(s3->subjects[2].code, "ME403", sizeof(s3->subjects[2].code) - 1);
    strncpy(s3->subjects[2].name, "Kinematics of Machines", sizeof(s3->subjects[2].name) - 1);
    s3->subjects[2].credits = 4;
    s3->subjects[2].semester = 4;
    s3->subjects[2].marks = 58.0f; // Grade C

    strncpy(s3->subjects[3].code, "ME404", sizeof(s3->subjects[3].code) - 1);
    strncpy(s3->subjects[3].name, "Manufacturing Technology", sizeof(s3->subjects[3].name) - 1);
    s3->subjects[3].credits = 3;
    s3->subjects[3].semester = 4;
    s3->subjects[3].marks = 64.0f; // Grade B

    strncpy(s3->subjects[4].code, "ME405", sizeof(s3->subjects[4].code) - 1);
    strncpy(s3->subjects[4].name, "Computer-Aided Drafting Lab", sizeof(s3->subjects[4].name) - 1);
    s3->subjects[4].credits = 2;
    s3->subjects[4].semester = 4;
    s3->subjects[4].marks = 72.0f; // Grade B+

    update_student_analytics(s3);
}

// Automatically load database from file, or populate seed dataset if missing
bool load_or_seed_database(StudentDatabase *db, const char *filepath) {
    if (!db) return false;

    if (!load_students_from_file(db, filepath) || db->count == 0) {
        printf(CLR_YELLOW "  [*] No existing database file found at '%s'. Initializing seed data...\n" CLR_RESET, filepath);
        seed_sample_dataset(db);
        save_students_to_file(db, filepath);
        printf(CLR_GREEN "  [+] Seed dataset loaded successfully with %d diverse academic profiles.\n" CLR_RESET, db->count);
        return false;
    }

    return true;
}
