#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "student.h"

#define DEFAULT_DATA_FILE "data/students.txt"

// File persistence functions (Member 3)
bool save_students_to_file(const StudentDatabase *db, const char *filepath);
bool load_students_from_file(StudentDatabase *db, const char *filepath);
void ensure_data_directory_exists(void);
void seed_sample_dataset(StudentDatabase *db);
bool load_or_seed_database(StudentDatabase *db, const char *filepath);

#endif // FILE_HANDLER_H
