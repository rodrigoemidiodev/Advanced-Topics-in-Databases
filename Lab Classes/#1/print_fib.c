#include <mysql.h>
#include <stdio.h>
#include <string.h>

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

    // Print table
    const char *select_fib_table = "SELECT * FROM Fibonacci";
    
    if(mysql_real_query(conn, select_fib_table, strlen(select_fib_table))){
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));

        mysql_close(conn);  

        return 1;   
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if(result == NULL){
        fprintf(stderr, "Result is null\n");

        mysql_close(conn);

        return 1;
    }

    MYSQL_ROW row;
    
    printf("+----+---------+\n");
    printf("| n  | fib     |\n");
    printf("+----+---------+\n");

    while((row = mysql_fetch_row(result))){
        printf("| %-2s | %-7s |\n", row[0], row[1]);
    }

    printf("+----+---------+\n");

    mysql_free_result(result);

    mysql_close(conn);

    return 0;
}