#include <stdio.h>
#include <sqlite3.h>

void runQuery(sqlite3 *db, const char *title, const char *sql) {
	sqlite3_stmt *stmt = NULL;

	printf("\n=== %s === \n", title);

	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	    if (rc != SQLITE_OK) {
	        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
	        return;
	    }


	int col_count = sqlite3_column_count(stmt);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
	        for (int i = 0; i < col_count; i++) {
	            const unsigned char *text = sqlite3_column_text(stmt, i);
	
	            if (text != NULL) {
	                printf("%s", text);
	            } else {
	                printf("NULL");
	            }
	
	            if (i < col_count - 1) {
	                printf(" | ");
	            }
	        }
	        printf("\n");
	    }

	if (rc != SQLITE_DONE) {
	        fprintf(stderr, "SQL step error: %s\n", sqlite3_errmsg(db));
	    }
	
	    sqlite3_finalize(stmt);
}

int main(void) {
    sqlite3 *db = NULL;

    int rc = sqlite3_open("study_information.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    runQuery(db,
        "Subjects with less than 6 credits",
        "SELECT code, name_en, credits, assesment_type "
        "FROM subjects "
        "WHERE credits < 6;"
    );

    runQuery(db,
        "Subjects with exam ordered by credits",
        "SELECT code, name_en, credits, assesment_type "
        "FROM subjects "
        "WHERE assesment_type = 'Exam' "
        "ORDER BY credits ASC;"
    );

    runQuery(db,
        "Grades for students named Marko",
        "SELECT students.fname, students.lname, subjects.code, subjects.name_en, declarations.grade "
        "FROM declarations "
        "JOIN students ON declarations.student_id = students.id "
        "JOIN subjects ON declarations.subject_id = subjects.id "
        "WHERE students.fname = 'Marko';"
    );

    runQuery(db,
        "My completed classes with grades",
        "SELECT subjects.code, subjects.name_en, subjects.credits, declarations.grade "
        "FROM declarations "
        "JOIN students ON declarations.student_id = students.id "
        "JOIN subjects ON declarations.subject_id = subjects.id "
        "WHERE students.eid = '12345678901' "
        "ORDER BY declarations.grade DESC;"
    );

    runQuery(db,
        "Total earned credits and average grade for each student",
        "SELECT students.fname, students.lname, "
        "SUM(subjects.credits) AS total_credits, "
        "AVG(declarations.grade) AS average_grade "
        "FROM declarations "
        "JOIN students ON declarations.student_id = students.id "
        "JOIN subjects ON declarations.subject_id = subjects.id "
        "WHERE declarations.grade > 0 "
        "GROUP BY students.id, students.fname, students.lname "
        "ORDER BY students.lname, students.fname;"
    );

    sqlite3_close(db);
    return 0;
}
