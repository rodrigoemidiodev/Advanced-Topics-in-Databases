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
    int fib[20];

    fib[1] = 0;
    fib[2] = 1;

    for(int i=3; i<=20; i++){
        fib[i] = fib[i-1] + fib[i-2];
    }

    char query[128];

    for(int i=1; i<=20; i++){
        snprintf(query, sizeof(query), "INSERT INTO Fibonacci (n, fib) VALUES (%d, %d)", i, fib[i]);

        if(mysql_real_query(conn, query, strlen(query))){
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));

            mysql_close(conn);

            return 1;
        }
    }

    mysql_close(conn);

    return 0;
}



