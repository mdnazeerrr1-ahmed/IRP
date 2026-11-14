# ACADEMIC PROJECT REPORT

---

## STUDENT ACADEMIC PERFORMANCE AND GPA ANALYTICS ENGINE
### An Institutional Decision Support & Terminal Analytics System

```
========================================================================================
COURSE NAME         : Data Structures and Modular C Programming (CS-204)
ACADEMIC YEAR       : 2026 – 2027
DEPARTMENT          : Department of Computer Science & Engineering
COLLEGE NAME        : [Insert College / University Name Here]
SUBMISSION DATE     : October 27, 2026
========================================================================================
```

### PROJECT TEAM AND DIVISION OF RESPONSIBILITIES

| Team Member | Roll Number | Primary Assigned Responsibilities |
|---|---|---|
| **Member 1: [Name Placeholder]** | [Roll No. 1] | **Student Management Module & Data Models**: Architecture of `Subject`, `Student`, and `StudentDatabase` structures; implementation of Student Enrollment (Add), Directory Display (View All), Bi-modal Search (ID & Name Substring), Profile Update, Record Deletion, and safe input parsing functions. |
| **Member 2: [Name Placeholder]** | [Roll No. 2] | **GPA Engine & Performance Analytics**: Implementation of the 10-point standardized grading scale; mathematical derivation and execution of Semester GPA and Cumulative CGPA; extrema detection (Highest/Lowest scoring courses); weak and backlog subject identification; personalized remediation suggestion engine. |
| **Member 3: [Name Placeholder]** | [Roll No. 3] | **System Integration, Terminal UI/UX & Persistence**: Implementation of the ANSI terminal dashboard layout and live KPI summary bar; flat-file serialization and deserialization (`file_handler.c`); sample dataset generation; test suite execution; comprehensive documentation and pitch script. |

---

## 1. ABSTRACT

In contemporary higher education, academic evaluation systems frequently suffer from fragmented data management, calculation errors arising from manual credit-weighting, and delayed identification of struggling students. This project presents the **Student Academic Performance and GPA Analytics Engine**, a standalone, high-performance terminal application developed strictly in standardized ISO C (C99/C11).

The application eliminates external dependencies—requiring no SQL databases, Python runtimes, or web frameworks—and delivers a responsive, website-like dashboard experience within standard terminal consoles. The system incorporates an automated Grade Point Average (GPA) and Cumulative Grade Point Average (CGPA) engine based on a standardized 10-point scale, multi-criteria subject diagnostics, early at-risk backlog alerts, and actionable academic recovery plans. Data persists seamlessly across sessions via a structured flat-file architecture. Tested rigorously against extreme inputs and boundary conditions, the engine proves resilient, portable, and pedagogically invaluable for academic mentors and institutional administrators.

---

## 2. INTRODUCTION

The assessment of student performance is a cornerstone of higher education administration. Grade Point Average (GPA) and Cumulative Grade Point Average (CGPA) serve as universally recognized metrics for evaluating academic competence, determining honors designations (such as the Dean’s List), establishing scholarship eligibility, and screening for graduate admissions and corporate placements.

Historically, academic marks tracking at the departmental level has relied on ad-hoc spreadsheets or manual grade ledgers. These manual workflows suffer from significant drawbacks:
- Disregard of varying credit hours leading to unweighted arithmetic average errors.
- Lack of granular diagnostic visibility into specific course failures masked by an acceptable aggregate score.
- Software bloat associated with enterprise software that requires internet connectivity, database management servers, and substantial computing overhead.

The **Student Academic Performance and GPA Analytics Engine** addresses these challenges by offering a lightning-fast, zero-dependency, local terminal dashboard. Leveraging ANSI escape sequences, structured record models, and modular procedural paradigms, the software provides faculty and students with immediate diagnostic feedback, clear visualizations, and automated academic advising roadmaps.

---

## 3. PROBLEM STATEMENT

Colleges, department heads, and students face critical bottlenecks in organizing marks and tracking academic trajectories:
1. **Arithmetic Inaccuracies**: Manual computation of GPA across courses with disparate credit weightings (e.g., 4-credit core theory vs. 2-credit laboratory courses) frequently leads to rounding errors and incorrect grade determinations.
2. **Delayed Intervention for At-Risk Students**: Students with marginal passing grades ($40 \le \text{marks} < 50$) or isolated backlogs are often identified only at the conclusion of an academic year, preventing timely remediation.
3. **Absence of Actionable Feedback**: Traditional transcripts merely report numerical scores without contextual guidance on how to prioritize study schedules, consult professors, or clear backlogs.
4. **Fragility of Entry-Level Software**: Basic educational C programs frequently crash when non-numeric characters are entered into numeric fields due to uncontrolled buffer behavior in standard input functions (`scanf`).

There is a distinct need for a lightweight, robust, portable C program capable of securely managing student records, executing exact credit-weighted calculations, providing diagnostic analytics, and ensuring persistence without requiring complex database infrastructure.

---

## 4. OBJECTIVES

The core objectives of this project are:
1. **Develop a Modular C Architecture**: Implement a decoupled, multi-file codebase conforming to ISO C standards, separating data modeling, computation, persistence, and interface rendering.
2. **Automate Credit-Weighted Calculations**: Accurately convert percentage marks into standard letter grades (A+ through F) and compute Semester GPA and Cumulative CGPA using exact weighted formulas.
3. **Provide Diagnostic Performance Analytics**: Identify course-level extrema (highest- and lowest-scoring subjects), detect backlogs ($< 40$ marks), flag weak courses ($< 50$ marks), and categorize overall standing into defined tiers (*Excellent*, *Good*, *Average*, *Needs Improvement*).
4. **Generate Personalized Improvement Plans**: Automatically construct tailored remediation suggestions based on a student’s specific deficiencies or achievements.
5. **Engineer an Intuitive Terminal Dashboard**: Create an aesthetic, website-like terminal interface featuring bordered tables, ANSI color-coded alerts, live KPI metrics, and breadcrumb navigation.
6. **Ensure Persistent Storage**: Implement safe flat-file serialization to maintain records across application restarts with self-healing default seed data.
7. **Ensure Complete Fault Tolerance**: Implement robust input parsing to guarantee the application never crashes on invalid or malicious user entries.

---

## 5. SCOPE OF THE PROJECT

### In-Scope Capabilities
- In-memory management of up to 100 enrolled students and up to 15 subjects per student.
- Bi-modal search capabilities supporting exact Student ID lookups and case-insensitive substring name queries.
- Complete CRUD (Create, Read, Update, Delete) operations with confirmation dialogs.
- Mathematical mapping of numeric scores (0.0 to 100.0) to standard letter grades and 10-point scale grade points.
- Instant recalculation of GPA, CGPA, and institutional tier classifications upon any subject addition or edit.
- Generation of printable terminal report cards and cohort-wide comparative dashboards.
- Text-based flat-file storage in `data/students.txt`.

### Out-of-Scope (Future Enhancements)
- Multi-user concurrency or network-distributed database synchronization.
- Direct export to graphical PDF formats (currently output as structured ASCII terminal transcripts).
- Integration with external web-based Learning Management Systems (LMS).

---

## 6. FUNCTIONAL REQUIREMENTS

The application is structured into four functional modules:

```
+--------------------------------------------------------------------------------------------------+
|                                    FUNCTIONAL REQUIREMENTS MATRIX                                |
+----+----------------------+----------------------------------------------------------------------+
| ID | Module               | Functional Requirement Description                                   |
+----+----------------------+----------------------------------------------------------------------+
| F1 | Student Management   | Enroll student with unique ID, Name, Department, Year, and Semester. |
| F2 | Student Management   | Display directory table containing all enrolled students.            |
| F3 | Student Management   | Search records by exact alphanumeric ID or case-insensitive name.    |
| F4 | Student Management   | Update individual student attributes (Name, Dept, Year, Semester).   |
| F5 | Student Management   | Delete student record with user confirmation safeguard.              |
+----+----------------------+----------------------------------------------------------------------+
| F6 | Subject Management   | Enroll course with Code, Title, Credits (1–6), Semester, and Marks.  |
| F7 | Subject Management   | Restrict entered marks strictly within the range [0.0, 100.0].       |
| F8 | Subject Management   | Support up to 15 distinct courses per student profile.               |
+----+----------------------+----------------------------------------------------------------------+
| F9 | GPA/CGPA Engine      | Map scores to grades: A+(10), A(9), B+(8), B(7), C(6), D(5), F(0).  |
| F10| GPA/CGPA Engine      | Compute credit-weighted GPA: Sum(Grade_Point * Credits) / Total_Cred.|
| F11| GPA/CGPA Engine      | Classify performance into Excellent, Good, Average, Needs Improvement|
+----+----------------------+----------------------------------------------------------------------+
| F12| Performance Analytics| Compute arithmetic mean marks and determine Highest & Lowest course. |
| F13| Performance Analytics| Flag failed courses (Grade F, <40) and weak courses (Grade D, 40-49).|
| F14| Performance Analytics| Generate targeted, individualized improvement advice.                |
| F15| Institutional Cohort | Compute cohort average GPA, identify Topper and at-risk students.    |
+----+----------------------+----------------------------------------------------------------------+
| F16| Persistence & Reports| Serialize and deserialize records to/from data/students.txt.         |
| F17| Persistence & Reports| Initialize 3 diverse benchmark student profiles if file is missing.  |
| F18| Persistence & Reports| Render formatted official terminal transcript report card.           |
+----+----------------------+----------------------------------------------------------------------+
```

---

## 7. NON-FUNCTIONAL REQUIREMENTS

1. **Reliability & Robustness**: The application must never crash or enter an infinite loop when receiving invalid inputs (e.g., typing letters into credit fields).
2. **Execution Performance**: Computations, searches, and file input/output must execute with sub-millisecond latency.
3. **Portability**: The codebase must compile cleanly across all major platforms using standard C compilers (MinGW, GCC, Clang) without requiring third-party libraries.
4. **Usability & Aesthetic Quality**: Terminal outputs must utilize clear headings, double-bordered tables, distinct ANSI colors, and intuitive numbered menus.
5. **Maintainability**: The codebase must adhere to strict modular boundaries with separate headers and source implementations, accompanied by comprehensive documentation.

---

## 8. TOOLS AND TECHNOLOGIES USED

- **Programming Language**: ISO C (C99 / C11 Standards)
- **Compiler**: GCC (GNU Compiler Collection) / MinGW-w64 (UCRT64 / MSVCRT)
- **Integrated Development Environment (IDE)**: Visual Studio Code with Microsoft C/C++ Extension (`ms-vscode.cpptools`)
- **Operating System Platform**: Microsoft Windows 10 / 11 (PowerShell & Windows Terminal)
- **Terminal Virtualization**: Windows Virtual Terminal Processing (`ENABLE_VIRTUAL_TERMINAL_PROCESSING`) and UTF-8 Console Output (`SetConsoleOutputCP(CP_UTF8)`)
- **Version Control**: Git & GitHub

---

## 9. SYSTEM DESIGN

### 9.1 Module Decomposition

The architecture is cleanly divided into four modular units:
1. **User Interface & Navigation Controller (`main.c`)**:
   Handles Windows console configuration, displays the live KPI dashboard bar, renders menus, and dispatches actions.
2. **Student Entity Management (`student.h`, `student.c`)**:
   Defines `Subject`, `Student`, and `StudentDatabase` structures. Implements CRUD lifecycle methods and defensive input handlers.
3. **GPA Analytics & Remediation Engine (`gpa.h`, `gpa.c`)**:
   Executes grading conversions, credit-weighted GPA/CGPA summations, extrema tracking, backlog detection, and report card rendering.
4. **File Persistence & Seeding Engine (`file_handler.h`, `file_handler.c`)**:
   Manages filesystem directory creation, structured flat-file serialization, deserialization, and sample benchmark seeding.

### 9.2 Data Flow Diagram

```
+-----------------------------------------------------------------------------+
|                                DATA FLOW MODEL                              |
+-----------------------------------------------------------------------------+

    [ Persistent Disk: data/students.txt ]
                    |
                    v (load_or_seed_database)
    +---------------------------------------------------+
    |     In-Memory Database (StudentDatabase db)       |
    |  - Array of Student records                       |
    |  - Each with Array of Subject records             |
    +---------------------------------------------------+
           |                     ^                 ^
           v (view/search)       | (add/update)    | (recalculate)
    +--------------------+ +-------------+ +--------------------+
    | Terminal Directory | | User Inputs | | GPA Analytics      |
    | & Search Views     | | (Sanitized) | | Engine (gpa.c)     |
    +--------------------+ +-------------+ +--------------------+
                                                   |
                                                   v
                                        +--------------------+
                                        | Official Transcript|
                                        | & Remediation Plan |
                                        +--------------------+
```

### 9.3 System Execution Flowchart (ASCII)

```
                     +---------------------------------------+
                     |                 START                 |
                     +---------------------------------------+
                                         |
                                         v
                     +---------------------------------------+
                     | Enable ANSI Virtual Terminal Mode     |
                     +---------------------------------------+
                                         |
                                         v
                     +---------------------------------------+
                     | Ensure 'data/' Directory Exists       |
                     +---------------------------------------+
                                         |
                                         v
                     +---------------------------------------+
                     | Check 'data/students.txt' Existence   |
                     +---------------------------------------+
                                    /         \
                             (Exists)         (Missing/Empty)
                                  /             \
                                 v               v
            +-------------------------+   +-------------------------------+
            | Load Records from File  |   | Seed 3 Diverse Sample Records |
            +-------------------------+   +-------------------------------+
                                 \               /
                                  \             /
                                   v           v
                     +---------------------------------------+
                     | Synchronize GPA & Analytics in Memory |
                     +---------------------------------------+
                                         |
                                         v
                     +---------------------------------------+
                     | Render Live Dashboard & Master Menu   |
                     +---------------------------------------+
                                         |
                                         v
                     +---------------------------------------+
                     | Read User Option Selection (1 to 8)   |
                     +---------------------------------------+
                                         |
         +-------+-------+-------+-------+-------+-------+-------+
         |       |       |       |       |       |       |       |
         v       v       v       v       v       v       v       v
       [ 1 ]   [ 2 ]   [ 3 ]   [ 4 ]   [ 5 ]   [ 6 ]   [ 7 ]   [ 8 ]
      Student Course    GPA   Student Official Cohort Storage  Exit
      Manage   Entry  Formula Analytics Report  Stats Center  Program
         |       |       |       |       |       |       |       |
         +-------+-------+-------+-------+-------+-------+       v
                         |                                 +------------+
                         v                                 | Auto-Save  |
            +--------------------------+                   | to Disk    |
            | Sync Changes to File     |                   +------------+
            +--------------------------+                         |
                         |                                       v
                         +-------------------------------->+------------+
                                                           |    STOP    |
                                                           +------------+
```

### 9.4 Navigation State Machine Diagram (ASCII)

```
+=============================================================================+
|                      NAVIGATION STATE MACHINE DIAGRAM                       |
+=============================================================================+

                        [ MAIN DASHBOARD MENU (1-8) ]
                                      |
       +------------------------------+------------------------------+
       |                              |                              |
       v                              v                              v
[ 1: Student Management ]   [ 2: Course & Marks Entry ]   [ 3: GPA Calculator ]
  |--> 1.1 Enroll Profile     |--> Enter Student ID         |--> Grading Scale
  |--> 1.2 View Directory     |--> Enter Course Details     |--> Formula Math
  |--> 1.3 Search Records     |--> Auto-Compute Grade       |--> Recalculate
  |--> 1.4 Update Student     |--> Recalculate GPA          +-------------------+
  |--> 1.5 Delete Record      +-------------------------+
  |--> 1.6 Back to Dashboard
       |
       +------------------------------+------------------------------+
       |                              |                              |
       v                              v                              v
[ 4: Academic Analytics ]   [ 5: Official Report Card ]   [ 6: Cohort Stats ]
  |--> Extrema (High/Low)     |--> Academic Bio             |--> Cohort Mean GPA
  |--> Backlogs (<40 marks)   |--> Course Grade Table       |--> Class Topper
  |--> Weak Subjects (<50)    |--> Credit Summary           |--> Risk Breakdown
  |--> Remediation Plan       |--> Improvement Advice       +-------------------+
  +-------------------------+ +-------------------------+
       |
       +------------------------------+
       |                              |
       v                              v
[ 7: Persistence Center ]   [ 8: Exit System ]
  |--> Save Active Memory     |--> Commit Changes to Disk
  |--> Reload from Disk       |--> Display Thank-You Card
  |--> Reset to Seed Data     |--> Terminate Process
  +-------------------------+ +-------------------------+
```

---

## 10. ALGORITHM AND LOGIC EXPLANATION

### 10.1 Student Record Handling Logic
- **Structure Representation**: Individual subjects are encapsulated within `Subject` structures, nested inside a parent `Student` record.
- **Unique Identification**: Student enrollment requires a unique alphanumeric identifier (e.g., `STU-1001`). The lookup function `find_student_by_id()` traverses the active array using case-insensitive comparison (`str_case_cmp()`) to prevent duplicate registrations.
- **Bi-modal Search**:
  1. *Exact Lookup*: Direct ID match returning a single unique record.
  2. *Keyword Substring Lookup*: Iterates through all registered names; if `str_case_str()` detects the query token anywhere within a student's full name, the index is recorded, allowing multi-record discovery.
- **Safe Record Deletion**: Uses array shifting. When record $k$ is removed, elements from $k+1$ to $N-1$ shift left by one index, preserving memory contiguity without dangling references.

### 10.2 GPA and CGPA Calculation Logic
- **Conversion to Grade Points**:
  Each subject's raw score $M \in [0.0, 100.0]$ is mapped to a grade point $P \in \{0, 5, 6, 7, 8, 9, 10\}$ and letter grade string:
  $$\text{Grade}(M) = \begin{cases}
  \text{"A+"}, P = 10 & \text{if } M \ge 90.0 \\
  \text{"A"},  P = 9  & \text{if } 80.0 \le M < 90.0 \\
  \text{"B+"}, P = 8  & \text{if } 70.0 \le M < 80.0 \\
  \text{"B"},  P = 7  & \text{if } 60.0 \le M < 70.0 \\
  \text{"C"},  P = 6  & \text{if } 50.0 \le M < 60.0 \\
  \text{"D"},  P = 5  & \text{if } 40.0 \le M < 50.0 \\
  \text{"F"},  P = 0  & \text{if } M < 40.0
  \end{cases}$$
- **Credit-Weighted Formula**:
  Let $C_i$ denote the credit hours of subject $i$, and $P_i$ denote its corresponding grade point. The GPA is formulated as:
  $$\text{GPA} = \frac{\sum_{i=1}^{k} (P_i \times C_i)}{\sum_{i=1}^{k} C_i}$$
  *Edge Case Guard*: If $\sum C_i = 0$ or $k = 0$, the function returns $0.00$ to prevent division-by-zero faults.

### 10.3 Performance Diagnostic & Remediation Logic
- **Extrema Tracking**: The engine performs linear scans over a student’s subject array to locate pointers to the maximum and minimum scoring courses.
- **Risk Detection**:
  - *Backlog Subjects*: Counted where $M_i < 40.0$ (Grade F). These courses yield $P_i = 0$ and generate critical alerts advising registration for supplementary examination periods.
  - *Weak Subjects*: Counted where $40.0 \le M_i < 50.0$ (Grade D). These indicate conceptual deficits that pull down cumulative standing.
- **Performance Tier Classification**:
  - $\text{GPA} \ge 9.00 \implies \textbf{Excellent}$ (Dean's Honor Roll standing)
  - $7.50 \le \text{GPA} < 9.00 \implies \textbf{Good}$ (Upper-quartile standing)
  - $6.00 \le \text{GPA} < 7.50 \implies \textbf{Average}$ (Satisfactory standing)
  - $\text{GPA} < 6.00 \implies \textbf{Needs Improvement}$ (Academic probation warning)

### 10.4 Persistent File Storage Architecture
- Records are saved in a clean, human-readable yet strictly parseable delimited format in `data/students.txt`.
- Header: `# STUDENT_ACADEMIC_ANALYTICS_DATA_V1` followed by total count.
- Student block begins with `STUDENT|id|name|dept|year|sem|count|gpa|cgpa|level`.
- Each subject follows with `SUBJECT|code|name|credits|sem|marks|grade|points`.
- The block terminates with `END_STUDENT`.
- On loading, the parser handles missing files gracefully and self-heals by invoking `seed_sample_dataset()` if no file is present.

---

## 11. COMPLETE CODE EXPLANATION

### 11.1 Source File Inventory

```
+--------------------+---------------------------------------------------------------------------------+
| Source File Name   | Modular Purpose & Core Responsibilities                                         |
+--------------------+---------------------------------------------------------------------------------+
| student.h          | Data structure definitions (Subject, Student, StudentDatabase) and prototypes.  |
| student.c          | Implementation of Student CRUD operations, tables, and input sanitizers.        |
| gpa.h              | Grading scale constants, GPA/CGPA formulas, and analytical prototypes.          |
| gpa.c              | Mathematical GPA calculation, extrema detection, remediation, and report cards. |
| file_handler.h     | File persistence function headers and sample seed declarations.                 |
| file_handler.c     | Flat-file serialization/deserialization logic and directory handling.            |
| main.c             | Application entry point, console mode initialization, dashboard & menu dispatch.|
+--------------------+---------------------------------------------------------------------------------+
```

### 11.2 Comprehensive Line-by-Line / Block Code Explanation Table

The table below provides a detailed line-by-line and block analysis of every source file in the project, documenting why each segment exists, where it is referenced, and the problem it resolves:

| File Name | Line Range / Identifier | Code Snippet / Construct | Why It Is Used | Where It Is Used | What Problem It Solves |
|---|---|---|---|---|---|
| `student.h` | 1–3, 62 | `#ifndef STUDENT_H`, `#define STUDENT_H`, `#endif` | Header inclusion guards. | Encloses entire `student.h` file. | Prevents duplicate definition compiler errors when included across multiple translation units. |
| `student.h` | 6–12 | `#define MAX_STUDENTS 100`, `#define MAX_SUBJECTS 15`, etc. | Defines architectural buffer limits for students, subjects, and text strings. | Used across array sizing in `student.h`, `student.c`, `main.c`. | Avoids magic numbers, centralizes memory allocation boundaries. |
| `student.h` | 15–23 | `typedef struct { char code[16]; ... float marks; int grade_point; } Subject;` | Models an individual academic course with credits, marks, grade, and quality points. | Embedded in `Student` struct; used in GPA calculation. | Encapsulates all attributes of a course into a single type. |
| `student.h` | 26–37 | `typedef struct { char id[20]; ... Subject subjects[15]; float gpa; } Student;` | Represents an enrolled student entity with bio and academic record. | Core entity in `StudentDatabase`. | Binds personal details with course records and computed GPA. |
| `student.h` | 40–43 | `typedef struct { Student students[MAX_STUDENTS]; int count; } StudentDatabase;` | Encapsulates the in-memory array of students and active count. | Passed by pointer throughout all modules. | Eliminates reliance on unsafe global variables. |
| `student.h` | 46–60 | Function prototypes (`add_student`, `clear_screen`, `get_int_input`, etc.) | Declares public interface for student management and input parsing. | Included by `student.c`, `main.c`, `file_handler.c`. | Enables compiler type-checking and decoupled compilation. |
| `gpa.h` | 6–18 | Benchmark definitions (`GRADE_APLUS_MIN 90.0f`, `PERF_EXCELLENT_MIN 9.0f`) | Standardizes grade thresholds and performance tier boundaries. | Used in `gpa.c` and `main.c`. | Enforces institutional consistency in grade assignments. |
| `gpa.h` | 20–38 | GPA prototypes (`calculate_student_gpa`, `display_student_analytics`, etc.) | Declares public interface for grading engine and report cards. | Included by `gpa.c`, `student.c`, `main.c`. | Provides clean API for analytical operations. |
| `file_handler.h` | 6–14 | File persistence prototypes (`save_students_to_file`, `DEFAULT_DATA_FILE`) | Declares file storage interface and default relative path. | Included by `file_handler.c`, `main.c`. | Abstracts file system interactions away from business logic. |
| `student.c` | 11–21 | ANSI color macros (`CLR_CYAN`, `CLR_GREEN`, `CLR_RED`, `CLR_RESET`) | Terminal color formatting codes. | Applied across all UI print statements. | Creates a visually engaging, modern dashboard interface. |
| `student.c` | 24–32 | `static int str_case_cmp(const char *s1, const char *s2)` | Case-insensitive string comparison helper. | Used in ID lookup (`find_student_by_id`). | Enables users to type "stu-1001" or "STU-1001" interchangeably. |
| `student.c` | 35–50 | `static const char* str_case_str(const char *haystack, const char *needle)` | Case-insensitive substring search helper. | Used in `find_students_by_name`. | Allows partial name matching regardless of capitalization. |
| `student.c` | 53–59 | `void clear_screen(void)` | Clears terminal screen portably across platforms. | Called before rendering each dashboard screen. | Removes clutter from previous operations. |
| `student.c` | 62–66 | `void wait_for_enter(void)` | Pauses execution until user presses the Enter key. | Called at the end of every viewing operation. | Prevents output from disappearing before the user can read it. |
| `student.c` | 69–96 | `void get_string_input(const char *prompt, char *buffer, int max_len)` | Reads text safely using `fgets`, strips trailing `\r`/`\n`, trims whitespace. | Used for all string inputs (Name, ID, Department). | Prevents buffer overflows and rejects accidental empty submissions. |
| `student.c` | 99–116 | `int get_int_input(const char *prompt, int min_val, int max_val)` | Reads whole line into buffer, parses with `sscanf`, validates `[min_val, max_val]`. | Used for menu choices, semester, credits, and year inputs. | Completely prevents infinite loops and crashes from non-numeric input. |
| `student.c` | 119–136 | `float get_float_input(const char *prompt, float min_val, float max_val)` | Reads whole line, parses float via `sscanf`, enforces valid numeric bounds. | Used for course marks entry (`0.0` to `100.0`). | Guarantees entered marks are strictly within the legitimate range. |
| `student.c` | 139–154 | `bool get_yes_no_input(const char *prompt)` | Interactive boolean prompt reading 'y'/'Y' or 'n'/'N'. | Used for deletion confirmations and sequential course additions. | Prevents ambiguous responses to critical confirmation prompts. |
| `student.c` | 157–161 | `void init_student_database(StudentDatabase *db)` | Sets database record counter to 0. | Called in `main()` during program boot. | Initializes clean state and avoids reading garbage memory. |
| `student.c` | 164–172 | `int find_student_by_id(const StudentDatabase *db, const char *id)` | Iterates through records comparing IDs. | Used in search, update, delete, and add subject. | Returns exact array index or -1 if not found. |
| `student.c` | 175–184 | `int find_students_by_name(...)` | Scans all records for name substring matches. | Used in keyword search menu option. | Enables discovery when faculty only remembers a first or last name. |
| `student.c` | 187–254 | `bool add_student(StudentDatabase *db)` | Enrolls new student profile, verifies ID uniqueness, allows immediate course entry. | Invoked from Student Management menu. | Centralizes enrollment validation and auto-triggers analytics. |
| `student.c` | 257–294 | `void view_all_students(const StudentDatabase *db)` | Renders directory of all registered students in an aligned ASCII border table. | Invoked from Student Management menu option 2. | Gives administrators an instant overview of all students. |
| `student.c` | 297–346 | `void search_student_menu(const StudentDatabase *db)` | Submenu providing exact ID or keyword search modes. | Invoked from Student Management menu option 3. | Provides multi-faceted discovery workflows. |
| `student.c` | 349–394 | `bool update_student(StudentDatabase *db)` | Allows targeted editing of Name, Department, Year, or Semester. | Invoked from Student Management menu option 4. | Enables easy correction of clerical errors without re-entering data. |
| `student.c` | 397–430 | `bool delete_student(StudentDatabase *db)` | Requests confirmation, shifts remaining elements left, decrements count. | Invoked from Student Management menu option 5. | Maintains array continuity upon record deletion safely. |
| `student.c` | 433–474 | `bool add_subject_to_student(StudentDatabase *db)` | Enrolls subject with code, credits, marks; triggers immediate GPA recalculation. | Invoked from Master Menu option 2. | Binds academic performance data to student entities. |
| `student.c` | 477–514 | `void view_student_subjects(const Student *s)` | Renders course-wise table with credits, marks, grades, points, and Pass/Fail status. | Called inside transcript report cards and analytics screens. | Visualizes complete course history cleanly. |
| `gpa.c` | 18–38 | `void get_letter_grade(float marks, char *grade_out, int *point_out)` | Maps numeric score to standard letter grade string and 0–10 grade points. | Called on every subject creation/update in `student.c` and `gpa.c`. | Ensures consistent, objective academic grading standards. |
| `gpa.c` | 42–58 | `float calculate_student_gpa(Student *s)` | Computes $\sum(P_i \times C_i) / \sum C_i$ with zero-credit divisor guard. | Called by analytics updates and report generation. | Accurately calculates credit-weighted Semester GPA. |
| `gpa.c` | 61–64 | `float calculate_student_cgpa(Student *s)` | Computes cumulative grade point average across all registered terms. | Called during analytics synchronization. | Tracks overall academic trajectory across semesters. |
| `gpa.c` | 67–77 | `void determine_performance_level(float gpa, char *level_out, int max_len)` | Categorizes GPA into Excellent, Good, Average, or Needs Improvement. | Called when updating student analytics. | Translates raw numbers into qualitative institutional standing. |
| `gpa.c` | 80–91 | `void update_student_analytics(Student *s)` | Recomputes GPA, CGPA, and updates performance level string. | Called after any course addition, grade edit, or data load. | Keeps student metrics synchronized with their marks. |
| `gpa.c` | 94–99 | `void recalculate_database_analytics(StudentDatabase *db)` | Iterates across all enrolled students calling `update_student_analytics`. | Invoked on program boot and formula view. | Ensures complete database consistency. |
| `gpa.c` | 102–112 | `const Subject* get_highest_scoring_subject(const Student *s)` | Linear scan locating course with maximum marks. | Used in performance insights. | Identifies a student's core academic strength. |
| `gpa.c` | 115–125 | `const Subject* get_lowest_scoring_subject(const Student *s)` | Linear scan locating course with minimum marks. | Used in performance insights. | Identifies course needing prioritized study attention. |
| `gpa.c` | 128–135 | `float calculate_average_marks(const Student *s)` | Computes unweighted arithmetic mean of all subject marks. | Displayed on report cards and analytics. | Provides percentage-equivalent metric alongside GPA. |
| `gpa.c` | 138–147 | `int count_failed_subjects(const Student *s)` | Counts subjects with marks $< 40.0$ (Grade F). | Used to trigger backlog remediation warnings. | Identifies academic progression roadblocks. |
| `gpa.c` | 150–159 | `int count_weak_subjects(const Student *s)` | Counts subjects with marks $< 50.0$ (Grade D and F). | Used in study recommendation logic. | Catches marginal passes before they turn into failures. |
| `gpa.c` | 162–234 | `void display_student_analytics(const Student *s)` | Displays analytical breakdown of strengths, weaknesses, credits, and metrics. | Invoked from Master Menu option 4. | Gives students and faculty an analytical performance breakdown. |
| `gpa.c` | 237–304 | `void display_improvement_suggestions(const Student *s)` | Constructs personalized improvement roadmap based on failed/weak subjects and tier. | Embedded in analytics view and official report cards. | Provides actionable, pedagogical advice rather than bare statistics. |
| `gpa.c` | 307–346 | `void print_student_report_card(const Student *s)` | Generates complete formatted transcript with header, courses, and summary metrics. | Invoked from Master Menu option 5. | Produces an official academic transcript suitable for advising. |
| `gpa.c` | 349–408 | `void print_analytics_summary_dashboard(const StudentDatabase *db)` | Calculates cohort mean GPA, identifies class topper, lowest GPA, and tier counts. | Invoked from Master Menu option 6. | Equips department heads with cohort-level academic insights. |
| `file_handler.c` | 18–20 | `void ensure_data_directory_exists(void)` | Creates `data/` directory using `_mkdir` (Windows) or `mkdir` (POSIX). | Called before saving or loading files. | Prevents file creation failures caused by missing folders. |
| `file_handler.c` | 23–60 | `bool save_students_to_file(...)` | Serializes records into structured pipe-delimited format in `data/students.txt`. | Called on additions, updates, deletions, and graceful exit. | Guarantees data persists after application termination. |
| `file_handler.c` | 63–145 | `bool load_students_from_file(...)` | Deserializes text lines, extracts tokens via `strtok`, rebuilds in-memory structs. | Called on program startup. | Restores previous session state seamlessly. |
| `file_handler.c` | 148–236 | `void seed_sample_dataset(StudentDatabase *db)` | Generates 3 realistic student profiles with 5 courses each (Alex, Priya, Michael). | Called if `data/students.txt` is missing or empty. | Provides immediate demonstration records without manual data entry. |
| `file_handler.c` | 239–251 | `bool load_or_seed_database(...)` | Attempts file load; falls back to seeding sample data if file is absent. | Invoked during `main()` initialization. | Implements self-healing startup logic. |
| `main.c` | 23–35 | `static void setup_console(void)` | Enables `ENABLE_VIRTUAL_TERMINAL_PROCESSING` and sets UTF-8 code page. | Called at the very start of `main()`. | Guarantees ANSI colors and box borders render properly on Windows. |
| `main.c` | 38–70 | `static void render_dashboard_header(const StudentDatabase *db)` | Renders banner and live KPI bar (Enrolled, Mean GPA, Dean's List, At-Risk). | Called at the top of every dashboard redraw. | Delivers a modern, website-like dashboard feel in the terminal. |
| `main.c` | 73–105 | `static void handle_student_management_menu(...)` | Manages Student Directory submenu and dispatches CRUD operations. | Invoked from Master Menu option 1. | Organizes student operations into a dedicated sub-view. |
| `main.c` | 108–144 | `static void display_gpa_formula_and_scales(...)` | Displays full grading benchmark table, formula derivation, and recalculates memory. | Invoked from Master Menu option 3. | Explains mathematical grading logic transparently. |
| `main.c` | 147–183 | `static void handle_data_persistence_menu(...)` | Submenu to save, reload, or reset sample data from disk. | Invoked from Master Menu option 7. | Gives users explicit control over file storage operations. |
| `main.c` | 186–273 | `int main(void)` | Sets up console, loads/seeds database, runs master event loop, handles auto-save. | Program entry point. | Orchestrates application lifecycle from boot to exit. |

---

## 12. TESTING SECTION

The application underwent rigorous verification testing across standard operational scenarios, edge cases, and invalid input conditions. All test cases passed with 100% adherence to specifications.

```
+========================================================================================================================+
|                                                  VERIFICATION TEST SUITE                                               |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| Test ID | Test Description           | Test Input            | Expected Output             | Actual Output     | Status|
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-01   | New Student Enrollment     | ID: STU-2001          | Record added to database;   | Enrolled, table   | PASS  |
|         | with unique ID & valid bio | Name: "Rohan Sharma"  | live count increments;      | displays record,  |       |
|         |                            | Dept: "CSE", Yr: 1    | table reflects entry.       | count updated.    |       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-02   | Duplicate ID Rejection     | ID: STU-1001          | System rejects duplicate;   | "Warning: Student | PASS  |
|         |                            | (Already registered)  | prompts user to enter a     | ID already regis- |       |
|         |                            |                       | unique identifier.          | tered" displayed. |       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-03   | Non-Numeric Character      | Menu Choice: "abc"    | System catches formatting   | "Invalid numeric  | PASS  |
|         | Rejection in Numeric Field | Semester: "xyz"       | error; re-prompts user      | format. Try again"|       |
|         |                            |                       | without infinite loops.     | cleanly handled.  |       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-04   | Marks Out-of-Bounds        | Marks: -15.0          | Rejected with out-of-range  | "Out of range!    | PASS  |
|         | Validation                 | Marks: 105.0          | notice; loops until valid   | Expected [0.0 -   |       |
|         |                            | Marks: 92.5           | [0.0 - 100.0] entered.      | 100.0]" displayed.|       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-05   | Grade Point & GPA Math     | CS101: 4 Cr, 95 Mk(A+)| Sum Quality Pts: 40+27=67.  | Total Credits: 7  | PASS  |
|         | Verification               | MA101: 3 Cr, 85 Mk(A) | Total Credits: 7.           | GPA = 9.57.       |       |
|         |                            |                       | GPA = 67 / 7 = 9.5714...    | Matches to 2 dec. |       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-06   | Backlog & Weak Subject     | ME401: 4 Cr, 34 Mk(F) | ME401 flagged as Failed (F);| Red Backlog Alert | PASS  |
|         | Remediation Trigger        | ME402: 4 Cr, 46 Mk(D) | ME402 flagged as Weak (D);  | & Supplementary   |       |
|         |                            |                       | Category: Needs Improvement | Exam Advice given.|       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
| TC-07   | Flat-File Persistence &    | Modify student name,  | File data/students.txt      | Records persist;  | PASS  |
|         | Reload Across Restarts     | exit program (Opt 8), | updated; restart reloads    | modified name     |       |
|         |                            | relaunch executable.  | modified record accurately. | restored cleanly. |       |
+---------+----------------------------+-----------------------+-----------------------------+-------------------+-------+
```

---

## 13. ADVANTAGES

1. **Zero External Dependencies**: Developed purely with standard C libraries, eliminating complex configurations or external packages.
2. **Deterministic Speed**: Operates with sub-millisecond execution times and minimal memory footprint.
3. **Resilient Input Architecture**: Custom `fgets`-based sanitization completely prevents terminal crashes from invalid inputs.
4. **Website-Like Dashboard in Terminal**: ANSI colors, aligned tables, and live metrics elevate the command-line experience.
5. **Pedagogical Relevance**: Bridges the gap between numerical evaluation and student counseling by providing concrete remediation roadmaps.

---

## 14. LIMITATIONS

1. **Terminal-Bound Interface**: Runs exclusively in terminal windows without a graphical windowing system (GUI).
2. **Fixed Static Array Bounds**: Maximum capacities are bounded at compile time (`MAX_STUDENTS 100`, `MAX_SUBJECTS 15`).
3. **Single-User Access Model**: Designed for local execution by a single faculty advisor rather than concurrent networked access.

---

## 15. FUTURE ENHANCEMENTS

1. **Dynamic Memory Allocation**: Transitioning from static arrays to dynamic heap-allocated linked lists to support arbitrarily large cohorts.
2. **Automated PDF Transcript Generation**: Generating styled, printable PDF transcripts directly from C.
3. **ASCII Histogram Plotting**: Rendering terminal-based grade distribution histograms and semester-by-semester GPA trend graphs.
4. **Role-Based Authentication**: Adding encrypted faculty and student login profiles to control data modification privileges.

---

## 16. CONCLUSION

The **Student Academic Performance and GPA Analytics Engine** successfully demonstrates the implementation of a full-featured software solution in standard C. By decoupling data management, mathematical calculation, diagnostic analysis, and persistence into modular files, the application delivers exceptional reliability and responsiveness.

The engine replaces error-prone manual calculations with exact credit-weighted formulas while offering intuitive diagnostic alerts that help institutions identify at-risk students before backlogs accumulate. The project proves that standard C, when structured with rigorous engineering practices and modern terminal aesthetics, remains an outstanding language for building fast, robust, and dependable software systems.

---

## 17. REFERENCES

1. Kernighan, B. W., & Ritchie, D. M. (1988). *The C Programming Language* (2nd ed.). Prentice Hall.
2. ISO/IEC 9899:2018 (Information Technology — Programming Languages — C). International Organization for Standardization.
3. Microsoft Corporation. *Console Virtual Terminal Sequences*. Microsoft Learn Documentation.
4. All India Council for Technical Education (AICTE). *Examination Reform Policy and Credit-Based Grading System Guidelines*.
5. Prata, S. (2013). *C Primer Plus* (6th ed.). Addison-Wesley Professional.
