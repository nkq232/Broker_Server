#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

int cvtChar2Int(const char* c) {
    int i = 0;
    int num = 0;
    while(c[i]!='\0') {
        num = num * 10 + ((int)c[i] - (int)'0');
        i++;
    }
    return num;
}

/**
* Client
*/

//Return -1 if error, return userID if success
char * signIn(const char *username, const char *password) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return "-1";
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return "-1";
    }

    

    char test_query[10001];
    strcpy(test_query, "select * from User u where u.username = \'");
    strcat(test_query, username);
    strcat(test_query, "\' and u.password = \'");
    strcat(test_query, password);
    strcat(test_query, "\' ;");
    
    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return "-1";
    }

    MYSQL_ROW row;

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return "-1";
    }

    if ((row = mysql_fetch_row(result))) {
        mysql_close(con);
        return row[0];
    }
    mysql_close(con);
    return "-1";

}

//Return 1 if success, return 0 if username existed, return -1 if error;
int signUp(const char *username,const char *password) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    

    char test_query[10001];
    strcpy(test_query, "select * from User u where u.username = \'");
    strcat(test_query, username);
    strcat(test_query, "\' ;");

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row!=NULL) {
        mysql_close(con);
        return -1;
    }

    strcpy(test_query, "INSERT INTO `User` (`UserId`, `UserName`, `Password`) VALUES (NULL, \'");
    strcat(test_query, username);
    strcat(test_query, "\', \'");
    strcat(test_query, password);
    strcat(test_query, "\') ;");

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    mysql_close(con);
    return 1;
}


// Query and write data to FILE. Return -1 if error, 1 if success
int getInfoByDay(FILE* fileName, const char *typeID, const char * locationID, const char *date) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    

    char test_query[10001];
    strcpy(test_query, "SELECT HOUR(d.Time) AS Hour, d.Value AS Value FROM `Data` d WHERE d.LocationId = ");
    strcat(test_query, locationID);
    strcat(test_query, " AND d.TypeId = ");
    strcat(test_query, typeID);
    strcat(test_query, " AND Date(d.Time) = Date(\'");
    strcat(test_query, date);
    strcat(test_query, "\');");

    

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    fprintf(fileName,"[\n");
    int count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        if (count != 0) fprintf(fileName,",\n");
        char *time = row[0] ? row[0] : "null";
        char *value = row[1] ? row[1] : "null";        
        fprintf(fileName,"{\"Time\": %s, \"Value\": %s}",time, value);
        count++;
    }

    fprintf(fileName,"\n]");


    mysql_free_result(result);
    mysql_close(con);
    return 1;
}

// Done
int getInfoByMonth(FILE* fileName, const char *typeID, const char * locationID, const char *date) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    char test_query[10001];
    strcpy(test_query, "SELECT DAY(d.Time) AS Day, AVG(d.Value) AS Value FROM `Data` d WHERE d.LocationId = ");
    strcat(test_query, locationID);
    strcat(test_query, " AND d.TypeId = ");
    strcat(test_query, typeID);
    strcat(test_query, " AND YEAR(d.Time) = YEAR(\'");
    strcat(test_query, date);
    strcat(test_query, "\') AND MONTH(d.Time) = MONTH(\'");
    strcat(test_query, date);
    strcat(test_query, "\')GROUP BY Day;");

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    fprintf(fileName,"[\n");
    int count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        if (count != 0) fprintf(fileName,",\n");
        char *time = row[0] ? row[0] : "null";
        char *value = row[1] ? row[1] : "null";        
        fprintf(fileName,"{\"Time\": %s, \"Value\": %s}",time, value);
        count++;
    }

    fprintf(fileName,"\n]");


    mysql_free_result(result);
    mysql_close(con);
    return 1;
}

//Done
int getInfoByYear(FILE* fileName, const char *typeID, const char * locationID, const char *date) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    char test_query[10001];
    strcpy(test_query, "SELECT MONTH(d.Time) AS Month, AVG(d.Value) AS Value FROM `Data` d WHERE d.LocationId = ");
    strcat(test_query, locationID);
    strcat(test_query, " AND d.TypeId = ");
    strcat(test_query, typeID);
    strcat(test_query, " AND YEAR(d.Time) = YEAR(\'");
    strcat(test_query, date);
    strcat(test_query, "\') GROUP BY Month;");

    

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    fprintf(fileName,"[\n");
    int count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        if (count != 0) fprintf(fileName,",\n");
        char *time = row[0] ? row[0] : "null";
        char *value = row[1] ? row[1] : "null";        
        fprintf(fileName,"{\"Time\": %s, \"Value\": %s}",time, value);
        count++;
    }

    fprintf(fileName,"\n]");


    mysql_free_result(result);
    mysql_close(con);
    return 1;
}


//Lay danh sach location -- Done
int getLocation(FILE * fileName) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    

    char test_query[10001];
    strcpy(test_query, "select * from Location where 1;");

    

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    fprintf(fileName,"[\n");
    int count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        if (count != 0) fprintf(fileName,",\n");
        char *locationID = row[0] ? row[0] : "null";
        char *localtionName = row[1] ? row[1] : "null";        
        fprintf(fileName,"{\"locationID\": %s, \"locationName\": \"%s\"}",locationID, localtionName);
        count++;
    }

    fprintf(fileName,"\n]");


    mysql_free_result(result);
    mysql_close(con);
    return 1;

}

//Done
int getType(FILE * fileName) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    

    char test_query[10001];
    strcpy(test_query, "select * from Type where 1;");

    

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    fprintf(fileName,"[\n");
    int count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        if (count != 0) fprintf(fileName,",\n");
        char *typeID = row[0] ? row[0] : "null";
        char *typeName = row[1] ? row[1] : "null";     
        char *typeValue = row[2] ? row[2] : "null";       
        fprintf(fileName,"{\"TypeID\": %s, \"TypeName\": \"%s\", \"TypeValue\": \"%s\"}",typeID, typeName, typeValue);
        count++;
    }

    fprintf(fileName,"\n]");


    mysql_free_result(result);
    mysql_close(con);
    return 1;
}

int getTypeByRegister(FILE * fileName, int userID, const char * locationID) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }



    char test_query[10001];
    snprintf(test_query, sizeof(test_query), "select t.* from Type t join Register r on t.TypeId = r.TypeId where locationId = %s and userId = %d;", locationID, userID);
    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    fprintf(fileName,"[\n");
    int count = 0;

    while ((row = mysql_fetch_row(result)))
    {
        if (count != 0) fprintf(fileName,",\n");
        char *typeID = row[0] ? row[0] : "null";
        char *typeName = row[1] ? row[1] : "null";
        char *typeValue = row[2] ? row[2] : "null";
        fprintf(fileName,"{\"TypeID\": %s, \"TypeName\": \"%s\", \"TypeValue\": \"%s\"}",typeID, typeName, typeValue);
        count++;
    }

    fprintf(fileName,"\n]");


    mysql_free_result(result);
    mysql_close(con);
    return 1;
}

int registerInfo(int userID, const char *locationID, const char *typeID) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    

    char test_query[10001];
    snprintf(test_query,sizeof(test_query), "INSERT INTO `Register` (`RegisterId`, `UserId`, `LocationId`, `TypeId`) VALUES (NULL, \'%d\', \'%s\', \'%s\');", userID, locationID, typeID);

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }
    
    mysql_close(con);
    return 1;
}

int deleteRegisterInfo(int userID, const char *locationID, const char *typeID) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    

    char test_query[10001];
    snprintf(test_query, sizeof(test_query), "DELETE FROM Register WHERE UserId = %d AND LocationId = %s  AND TypeId = %s;", userID, locationID, typeID);

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }
    
    mysql_close(con);
    return 1;
}

int getTypeByUser(int type[], int*len, int userID, const char* locationID) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    char test_query[1000];
    snprintf(test_query, sizeof(test_query), "select TypeId from Register where LocationId = \'%s\' and UserId = \'%d\';", locationID, userID);
    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        *len = 0;
        return -1;
    }

    MYSQL_ROW row;

    int i = 0;

    while ((row = mysql_fetch_row(result)))
    {
        int n = row[0] ? cvtChar2Int(row[0]) : -1;
        type[i] = n;
        i++;
    }

    *len = i;

    mysql_free_result(result);
    mysql_close(con);
    return 1;
}



