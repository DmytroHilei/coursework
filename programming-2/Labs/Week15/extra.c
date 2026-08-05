#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>

#define DB_NAME "study_information.db"

#define MY_EID "12345678901"
#define MY_FNAME "Dmytro"
#define MY_LNAME "Hilei"
#define MY_UNI_ID "dhilei"

#define MY_SUBJECT_CODE "CS231n"
#define MY_SUBJECT_NAME_EN "Deep Learning for Computer Vision"
#define MY_SUBJECT_NAME_ET "Sügavõpe arvutinägemise jaoks"
#define MY_SUBJECT_CREDITS 8
#define MY_SUBJECT_ASSESSMENT "Exam"

typedef struct {
    int id;
    char fname[128];
    char lname[128];
} Student;

static void print_sql_error(sqlite3 *db, const char *message) {
    fprintf(stderr, "%s: %s\n", message, sqlite3_errmsg(db));
}

static int execute_simple(sqlite3 *db, const char *sql) {
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }

    return 1;
}

static int get_id_by_text(sqlite3 *db, const char *sql, const char *value) {
    sqlite3_stmt *stmt = NULL;
    int id = -1;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare failed");
        return -1;
    }

    sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    } else if (rc != SQLITE_DONE) {
        print_sql_error(db, "Step failed");
    }

    sqlite3_finalize(stmt);
    return id;
}

static int get_or_create_student(sqlite3 *db) {
    int student_id = get_id_by_text(
        db,
        "SELECT id FROM students WHERE eid = ?;",
        MY_EID
    );

    if (student_id != -1) {
        printf("Student already exists: %s %s\n", MY_FNAME, MY_LNAME);
        return student_id;
    }

    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT INTO students (eid, fname, lname, uni_id) "
        "VALUES (?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare student insert failed");
        return -1;
    }

    sqlite3_bind_text(stmt, 1, MY_EID, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, MY_FNAME, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, MY_LNAME, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, MY_UNI_ID, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        print_sql_error(db, "Student insert failed");
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    student_id = (int)sqlite3_last_insert_rowid(db);
    printf("Inserted student: %s %s\n", MY_FNAME, MY_LNAME);

    return student_id;
}

static int get_or_create_subject(sqlite3 *db) {
    int subject_id = get_id_by_text(
        db,
        "SELECT id FROM subjects WHERE code = ?;",
        MY_SUBJECT_CODE
    );

    if (subject_id != -1) {
        printf("Subject already exists: %s\n", MY_SUBJECT_CODE);
        return subject_id;
    }

    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT INTO subjects (name_en, name_et, code, credits, assesment_type) "
        "VALUES (?, ?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare subject insert failed");
        return -1;
    }

    sqlite3_bind_text(stmt, 1, MY_SUBJECT_NAME_EN, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, MY_SUBJECT_NAME_ET, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, MY_SUBJECT_CODE, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, MY_SUBJECT_CREDITS);
    sqlite3_bind_text(stmt, 5, MY_SUBJECT_ASSESSMENT, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        print_sql_error(db, "Subject insert failed");
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);

    subject_id = (int)sqlite3_last_insert_rowid(db);
    printf("Inserted subject: %s\n", MY_SUBJECT_CODE);

    return subject_id;
}

static int declaration_exists(sqlite3 *db, int student_id, int subject_id) {
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "SELECT id FROM declarations "
        "WHERE student_id = ? AND subject_id = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare declaration check failed");
        return 0;
    }

    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_int(stmt, 2, subject_id);

    rc = sqlite3_step(stmt);

    int exists = (rc == SQLITE_ROW);

    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        print_sql_error(db, "Declaration check failed");
    }

    sqlite3_finalize(stmt);
    return exists;
}

static void insert_declaration_if_missing(sqlite3 *db, int student_id, int subject_id, int grade) {
    if (student_id == -1 || subject_id == -1) {
        printf("Cannot insert declaration: invalid student_id or subject_id\n");
        return;
    }

    if (declaration_exists(db, student_id, subject_id)) {
        printf("Declaration already exists\n");
        return;
    }

    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "INSERT INTO declarations (student_id, subject_id, grade) "
        "VALUES (?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare declaration insert failed");
        return;
    }

    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_int(stmt, 2, subject_id);
    sqlite3_bind_int(stmt, 3, grade);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        print_sql_error(db, "Declaration insert failed");
    } else {
        printf("Inserted declaration with grade %d\n", grade);
    }

    sqlite3_finalize(stmt);
}

static int get_subject_id_by_code(sqlite3 *db, const char *code) {
    return get_id_by_text(
        db,
        "SELECT id FROM subjects WHERE code = ?;",
        code
    );
}

static void add_myself_reliably(sqlite3 *db) {
    printf("\n=== Extra 1: Add myself reliably ===\n");

    int student_id = get_or_create_student(db);

    int cs231n_id = get_or_create_subject(db);
    int programming1_id = get_subject_id_by_code(db, "IAX0583");
    int discrete_math_id = get_subject_id_by_code(db, "IAX0010");

    if (programming1_id == -1) {
        printf("Warning: subject IAX0583 not found\n");
    }

    if (discrete_math_id == -1) {
        printf("Warning: subject IAX0010 not found\n");
    }

    insert_declaration_if_missing(db, student_id, cs231n_id, 5);
    insert_declaration_if_missing(db, student_id, programming1_id, 5);
    insert_declaration_if_missing(db, student_id, discrete_math_id, 5);
}

static void make_base_uni_id(char *dest, size_t size, const char *fname, const char *lname) {
    if (size == 0) {
        return;
    }

    size_t pos = 0;

    if (fname != NULL && fname[0] != '\0' && pos + 1 < size) {
        dest[pos++] = (char)tolower((unsigned char)fname[0]);
    }

    if (lname != NULL) {
        for (size_t i = 0; lname[i] != '\0' && pos + 1 < size; i++) {
            if (!isspace((unsigned char)lname[i]) && lname[i] != '-') {
                dest[pos++] = (char)tolower((unsigned char)lname[i]);
            }
        }
    }

    dest[pos] = '\0';
}

static int uni_id_exists_for_other_student(sqlite3 *db, const char *uni_id, int student_id) {
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "SELECT id FROM students "
        "WHERE uni_id = ? AND id != ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare uni_id check failed");
        return 1;
    }

    sqlite3_bind_text(stmt, 1, uni_id, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, student_id);

    rc = sqlite3_step(stmt);

    int exists = (rc == SQLITE_ROW);

    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        print_sql_error(db, "Uni-id check failed");
    }

    sqlite3_finalize(stmt);
    return exists;
}

static void update_student_uni_id(sqlite3 *db, int student_id, const char *uni_id) {
    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "UPDATE students "
        "SET uni_id = ? "
        "WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare uni_id update failed");
        return;
    }

    sqlite3_bind_text(stmt, 1, uni_id, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, student_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        print_sql_error(db, "Uni-id update failed");
    }

    sqlite3_finalize(stmt);
}

static void generate_uni_ids(sqlite3 *db) {
    printf("\n=== Extra 2: Generate uni_ids ===\n");

    sqlite3_stmt *stmt = NULL;

    const char *sql =
        "SELECT id, fname, lname "
        "FROM students;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "Prepare students select failed");
        return;
    }

    Student *students = NULL;
    size_t count = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Student *tmp = realloc(students, (count + 1) * sizeof(Student));
        if (tmp == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(students);
            sqlite3_finalize(stmt);
            return;
        }

        students = tmp;

        students[count].id = sqlite3_column_int(stmt, 0);

        const unsigned char *fname = sqlite3_column_text(stmt, 1);
        const unsigned char *lname = sqlite3_column_text(stmt, 2);

        snprintf(students[count].fname, sizeof(students[count].fname), "%s", fname ? (const char *)fname : "");
        snprintf(students[count].lname, sizeof(students[count].lname), "%s", lname ? (const char *)lname : "");

        count++;
    }

    if (rc != SQLITE_DONE) {
        print_sql_error(db, "Students select failed");
        free(students);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    for (size_t i = 0; i < count; i++) {
        char base[256];
        char candidate[256];

        make_base_uni_id(base, sizeof(base), students[i].fname, students[i].lname);

        if (base[0] == '\0') {
            snprintf(base, sizeof(base), "student%d", students[i].id);
        }

        snprintf(candidate, sizeof(candidate), "%s", base);

        int suffix = 1;
        while (uni_id_exists_for_other_student(db, candidate, students[i].id)) {
            snprintf(candidate, sizeof(candidate), "%s%d", base, suffix);
            suffix++;
        }

        update_student_uni_id(db, students[i].id, candidate);

        printf("%s %s -> %s\n", students[i].fname, students[i].lname, candidate);
    }

    free(students);
}

static void run_query(sqlite3 *db, const char *title, const char *sql) {
    sqlite3_stmt *stmt = NULL;

    printf("\n=== %s ===\n", title);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        print_sql_error(db, "SQL prepare error");
        return;
    }

    int column_count = sqlite3_column_count(stmt);

    for (int i = 0; i < column_count; i++) {
        printf("%s", sqlite3_column_name(stmt, i));

        if (i < column_count - 1) {
            printf(" | ");
        }
    }

    printf("\n");

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (int i = 0; i < column_count; i++) {
            int type = sqlite3_column_type(stmt, i);

            if (type == SQLITE_INTEGER) {
                printf("%d", sqlite3_column_int(stmt, i));
            } else if (type == SQLITE_FLOAT) {
                printf("%.2f", sqlite3_column_double(stmt, i));
            } else if (type == SQLITE_TEXT) {
                printf("%s", sqlite3_column_text(stmt, i));
            } else if (type == SQLITE_NULL) {
                printf("NULL");
            } else {
                printf("[BLOB]");
            }

            if (i < column_count - 1) {
                printf(" | ");
            }
        }

        printf("\n");
    }

    if (rc != SQLITE_DONE) {
        print_sql_error(db, "SQL step error");
    }

    sqlite3_finalize(stmt);
}

int main(void) {
    sqlite3 *db = NULL;

    int rc = sqlite3_open_v2(DB_NAME, &db, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database '%s': %s\n", DB_NAME, sqlite3_errmsg(db));
        sqlite3_close(db);
        return EXIT_FAILURE;
    }

    execute_simple(db, "PRAGMA foreign_keys = ON;");

    execute_simple(db, "BEGIN TRANSACTION;");

    add_myself_reliably(db);
    generate_uni_ids(db);

    execute_simple(db, "COMMIT;");

    run_query(
        db,
        "Task 2 Part 1 Query 1: Subjects with less than 6 credits",
        "SELECT code, name_en, credits, assesment_type "
        "FROM subjects "
        "WHERE credits < 6;"
    );

    run_query(
        db,
        "Task 2 Part 1 Query 2: Subjects with exam ordered by credits",
        "SELECT code, name_en, credits, assesment_type "
        "FROM subjects "
        "WHERE LOWER(assesment_type) = 'exam' "
        "ORDER BY credits ASC;"
    );

    run_query(
        db,
        "Task 2 Part 2 Query 1: Grades for students named Marko",
        "SELECT students.fname, "
        "       students.lname, "
        "       subjects.code, "
        "       subjects.name_en, "
        "       declarations.grade "
        "FROM declarations "
        "JOIN students ON declarations.student_id = students.id "
        "JOIN subjects ON declarations.subject_id = subjects.id "
        "WHERE students.fname = 'Marko';"
    );

    run_query(
        db,
        "Task 2 Part 2 Query 2: My completed classes with grades",
        "SELECT subjects.code, "
        "       subjects.name_en, "
        "       subjects.credits, "
        "       declarations.grade "
        "FROM declarations "
        "JOIN students ON declarations.student_id = students.id "
        "JOIN subjects ON declarations.subject_id = subjects.id "
        "WHERE students.eid = '" MY_EID "' "
        "ORDER BY declarations.grade DESC;"
    );

    run_query(
        db,
        "Task 2 Part 3: Total earned credits and average grade for each student",
        "SELECT students.fname, "
        "       students.lname, "
        "       SUM(subjects.credits) AS total_credits, "
        "       AVG(declarations.grade) AS average_grade "
        "FROM declarations "
        "JOIN students ON declarations.student_id = students.id "
        "JOIN subjects ON declarations.subject_id = subjects.id "
        "WHERE declarations.grade > 0 "
        "GROUP BY students.id, students.fname, students.lname "
        "ORDER BY students.lname ASC, students.fname ASC;"
    );

    rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot close database: %s\n", sqlite3_errmsg(db));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
