#include <mysql.h>
#include <stdio.h>
#include <string.h>

// This version uses MYSQL prepared statement for insert

int main(){
    // Intialize connection
    MYSQL *conn = mysql_init(NULL);

    if(conn == NULL){
        fprintf(stderr, "MySQL init failed\n");

        return 1;
    }

    char user[16];
    char pass[16];
    char db[16];

    // Input user, password and database
    printf("Enter user: ");
    scanf("%s", user);
    printf("Enter password: ");
    scanf("%s", pass);
    printf("Enter database: ");
    scanf("%s", db);

    // Testing connection
    if (mysql_real_connect(conn, "localhost", user, pass, db, 0, NULL, 0) == NULL){
        fprintf(stderr, "Connection failed: %s\n", mysql_error(conn));

        mysql_close(conn);

        return 1;
    }

    // Ensure no table `Fibonacci` exists in the database
    const char *drop_fib_table = "DROP TABLE IF EXISTS Fibonacci";

    if(mysql_real_query(conn, drop_fib_table, strlen(drop_fib_table))){
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));

        mysql_close(conn);

        return 1;
    }

    // Create table `Fibonacci` in the database
    const char *create_fib_table =
        "CREATE TABLE IF NOT EXISTS Fibonacci ("
        "n INT AUTO_INCREMENT PRIMARY KEY, "
        "fib INT NOT NULL"
        ")";

    if(mysql_real_query(conn, create_fib_table, strlen(create_fib_table))){
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));

        mysql_close(conn);

        return 1;
    }

    // Populate table `Fibonacci` with the first 20 Fibonacci numbers
    MYSQL_STMT *stmt = mysql_stmt_init(conn);

    if(!stmt){
        fprintf(stderr, "Statement init failed: %s\n", mysql_error(conn));

        mysql_close(conn);

        return 1;
    }

    const char *insert_fib = "INSERT INTO Fibonacci (n, fib) VALUES (?, ?)";

    if(mysql_stmt_prepare(stmt, insert_fib, strlen(insert_fib))){
        fprintf(stderr, "Statement prepare failed: %s\n", mysql_stmt_error(stmt));

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return 1;
    }

    MYSQL_BIND result_bind[2];
    memset(result_bind, 0, sizeof(result_bind));

    int n, fib;

    result_bind[0].buffer_type = MYSQL_TYPE_LONG;
    result_bind[0].buffer = &n;
    result_bind[1].buffer_type = MYSQL_TYPE_LONG;
    result_bind[1].buffer = &fib;

    if(mysql_stmt_bind_param(stmt, result_bind)){
        fprintf(stderr, "Bind result failed: %s\n", mysql_stmt_error(stmt));

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return 1;
    }

    int fib_array[20];

    fib_array[1] = 0;
    fib_array[2] = 1;

    for(int i=3; i<=20; i++){
        fib_array[i] = fib_array[i-1] + fib_array[i-2];
    }

    for(int i=1; i<=20; i++){
        n = i;
        fib = fib_array[i];

        if(mysql_stmt_execute(stmt)){
            fprintf(stderr, "Execution failed: %s\n", mysql_stmt_error(stmt));

            mysql_stmt_close(stmt);
            mysql_close(conn);

            return 1;
        }
    }

    mysql_stmt_close(stmt);
    mysql_close(conn);

    return 0;
}



