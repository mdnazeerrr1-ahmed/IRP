# Student Academic Performance and GPA Analytics Engine

An institutional-grade, menu-driven terminal dashboard and analytics system developed strictly in ISO C standard. This application empowers educational institutions, faculty advisors, and students to track academic milestones, compute Semester GPA and Cumulative CGPA with 100% precision, detect weak or backlog subjects, and generate personalized academic remediation plans.

---

## 🌟 Key Features

1. **Dashboard Terminal Interface**
   - High-contrast ANSI colors, clear headings, and tabular formatted displays.
   - Live KPI summary bar tracking total enrollment, cohort average GPA, Dean's List qualifiers, and academic warnings.
   - Comprehensive breadcrumb navigation and input sanitization (buffer overflow and non-numeric input protection).

2. **Student Record Management**
   - Enroll new students with unique identification numbers.
   - View formatted directory of all registered students.
   - Search records via exact Student ID or flexible Name substring matching.
   - Update individual student profile attributes.
   - Delete records with confirmation safeguards.

3. **Course & Marks Management**
   - Register up to 15 subjects per student with custom course codes, course titles, credit weights (1–6), and semester markers.
   - Strict input validation ensuring marks fall strictly between `0.0` and `100.0`.
   - Real-time automatic conversion into standard letter grades and quality grade points.

4. **GPA & CGPA Analytics Engine**
   - **Grading Scale**:
     - `90.0 – 100.0`: **A+** (10 Grade Points) — *Outstanding Mastery*
     - `80.0 – 89.9` : **A**  (9 Grade Points) — *Excellent Conceptual Understanding*
     - `70.0 – 79.9` : **B+** (8 Grade Points) — *Very Good Performance*
     - `60.0 – 69.9` : **B**  (7 Grade Points) — *Good / Above Average*
     - `50.0 – 59.9` : **C**  (6 Grade Points) — *Average / Satisfactory*
     - `40.0 – 49.9` : **D**  (5 Grade Points) — *Marginal Pass*
     - `Below 40.0`  : **F**  (0 Grade Points) — *Fail / Backlog*
   - **Weighted Formula**:
     $$\text{GPA} = \frac{\sum (\text{Grade Point}_i \times \text{Credit Hours}_i)}{\sum \text{Credit Hours}_i}$$

5. **Diagnostic Analytics & Personalized Remediation**
   - Automatic identification of Highest-scoring and Lowest-scoring subjects.
   - Backlog detection (Grade F, $< 40$ marks) and Weak-subject warning flags ($40 \le \text{marks} < 50$, Grade D).
   - Performance tier categorization:
     - **Excellent** ($\ge 9.00$ GPA)
     - **Good** ($7.50 \le \text{GPA} < 9.00$)
     - **Average** ($6.00 \le \text{GPA} < 7.50$)
     - **Needs Improvement** ($< 6.00$ GPA)
   - Personalized improvement roadmaps with concrete study strategies, faculty mentoring recommendations, and backlog recovery steps.

6. **Institutional Cohort Analytics**
   - Class-wide aggregate metrics: Cohort Mean GPA, Valedictorian/Topper identification, lowest-performing student alerts, and tier distribution percentages.

7. **Persistent File Storage**
   - Automatic record serialization to `data/students.txt`.
   - Automatic database seeding with 3 diverse student profiles on first startup if no database file exists.

---

## 📁 Project Architecture

```
IRP/
├── main.c              # Program entry point, terminal dashboard UI, and master navigation
├── student.h           # Data structures (Subject, Student, StudentDatabase) and declarations
├── student.c           # Student enrollment, search, update, delete, and input validation
├── gpa.h               # Grading benchmarks, GPA/CGPA prototypes, and analytics headers
├── gpa.c               # Grade mapping, GPA calculations, report card generation, insights
├── file_handler.h      # Persistence interface and sample seeding prototypes
├── file_handler.c      # Flat-file serialization, deserialization, directory creation
├── data/
│   └── students.txt    # Persistent storage file for all student academic records
├── README.md           # Setup, installation, compilation, and usage documentation
├── REPORT.md           # Formal academic report with architecture diagrams and code analysis
└── PRESENTATION.md     # 10-slide oral pitch deck with presenter allocations and scripts
```

---

## 🛠️ Step-by-Step VS Code Setup Instructions (Windows)

Follow these beginner-friendly steps to set up, build, and run the project on Windows:

### Step 1: Install Visual Studio Code
1. Download VS Code for Windows from: [https://code.visualstudio.com/](https://code.visualstudio.com/)
2. Run the installer and ensure **"Add to PATH"** is checked during setup.
3. Launch Visual Studio Code.

### Step 2: Install the C/C++ Extension
1. Inside VS Code, click the **Extensions** icon on the left sidebar (or press `Ctrl + Shift + X`).
2. Search for `C/C++` (developed by Microsoft).
3. Click **Install**.

### Step 3: Install GCC / MinGW Compiler
If you do not have GCC installed:
1. **Option A (Recommended via MSYS2 / WinLibs)**:
   - Download the MinGW-w64 installer or MSYS2 from [https://www.msys2.org/](https://www.msys2.org/).
   - In MSYS2 terminal, run:
     ```bash
     pacman -S mingw-w64-ucrt-x86_64-gcc
     ```
2. **Option B (Direct Winget Command in PowerShell)**:
   ```powershell
   winget install --id MSYS2.MSYS2 -e
   ```
3. **Add MinGW to System Environment PATH**:
   - Open Windows Search, type **"Environment Variables"**, and click **"Edit the system environment variables"**.
   - Click **"Environment Variables..."** -> Select `Path` under User/System variables -> Click **Edit**.
   - Click **New** and add: `C:\msys64\ucrt64\bin` (or your MinGW `bin` folder path).
   - Click **OK** to save.
4. **Verify GCC Installation**:
   - Open PowerShell or Command Prompt and run:
     ```bash
     gcc --version
     ```
   - You should see GCC version information (e.g., `gcc.exe (Rev..., Built by MSYS2...) 13.x/14.x/16.x`).

### Step 4: Open the Project Folder in VS Code
1. Open VS Code.
2. Click **File -> Open Folder...** (or press `Ctrl + K, Ctrl + O`).
3. Select this project directory (`IRP`).

### Step 5: Compile the Project
1. Open the integrated terminal in VS Code:
   - Press `` Ctrl + ` `` (Backtick) or go to menu **Terminal -> New Terminal**.
2. Run the GCC compilation command:
   ```bash
   gcc main.c student.c gpa.c file_handler.c -o academic_engine
   ```
   *(Note: This compiles all four C source files cleanly into a single optimized Windows executable named `academic_engine.exe` without warnings).*

### Step 6: Run the Executable
In the same VS Code PowerShell terminal, execute:
```powershell
.\academic_engine.exe
```

---

## 🎮 Navigation & User Guide

Once launched, the engine presents an intuitive dashboard:

```
  +--------------------------------------------------------------------------------------------------+
  |                     STUDENT ACADEMIC PERFORMANCE & GPA ANALYTICS ENGINE                          |
  |                         Institutional Decision Support Dashboard v2.0                            |
  +--------------------------------------------------------------------------------------------------+
  [ LIVE METRICS ] Enrolled: 3  |  Cohort Mean GPA: 7.61  |  Dean's Honor Roll: 1  |  Academic Warnings: 1  |  Storage: ONLINE
  ----------------------------------------------------------------------------------------------------

  +-- SYSTEM NAVIGATION MENU ------------------------------------------------------------------------+
  |  [1] 📋 Student Directory Management   (Add, View, Search, Update, Delete Records)               |
  |  [2] 📚 Subject & Marks Entry           (Add Course, Enter Credits & Marks, Auto-Grade)          |
  |  [3] ⚡ GPA & CGPA Calculator Engine   (Grading Scales, Formula Derivation, Recalculate)          |
  |  [4] 📊 Academic Performance Analytics  (Strengths, Weaknesses, Benchmarks, Suggestions)           |
  |  [5] 📑 Student Official Report Card    (Full Individual Academic Transcript & Plan)               |
  |  [6] 📈 Institutional Cohort Analytics  (Class Averages, Toppers, Risk Distribution)               |
  |  [7] 💾 Data Persistence & File Center  (Save Records, Reload, Seed Sample Data)                   |
  |  [8] 🚪 Exit System                                                                              |
  +--------------------------------------------------------------------------------------------------+
```

### Pre-loaded Sample Personas

The system includes three realistic student records out-of-the-box:

1. **Alex Johnson (`STU-1001`)**:
   - Computer Science, 4th Semester.
   - Scored 94.5 in Data Structures, 96.0 in Discrete Math, 91.0 in OS.
   - **GPA**: `9.35` | **Classification**: `Excellent` (Dean's Honor Roll candidate).
2. **Priya Patel (`STU-1002`)**:
   - Electronics & Communication, 4th Semester.
   - Solid, consistent scores in Signals, Circuit Design, and Microprocessors.
   - **GPA**: `8.18` | **Classification**: `Good` (Consistent upper-quartile standing).
3. **Michael Brown (`STU-1003`)**:
   - Mechanical Engineering, 4th Semester.
   - Scored 34.0 in Thermodynamics (Failed, Grade F) and 46.0 in Fluid Mechanics (Weak, Grade D).
   - **GPA**: `5.29` | **Classification**: `Needs Improvement` (Academic Probation flag with concrete remediation guidance).

---

## 💡 Troubleshooting & Tips

- **ANSI Colors in Windows Terminal**: The program automatically enables `ENABLE_VIRTUAL_TERMINAL_PROCESSING` on Windows consoles. For the optimal aesthetic visual experience, run inside the VS Code integrated terminal or Windows Terminal.
- **Accidental Non-Numeric Input**: The custom input parsing functions (`get_int_input` and `get_float_input`) utilize `fgets` and token verification. If a user enters letters or symbols when numbers are expected, the application rejects the entry cleanly without crashing or entering infinite loops.
- **Data Safety**: All modifications (adds, updates, deletions) automatically sync to `data/students.txt` upon operation completion and on graceful exit.
