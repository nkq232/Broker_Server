#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int cvtChar2Int(char* c) {
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


//Check database has username? Return number of username
//Khong can thiet lam nhung van done
int checkUsername(char *username) {
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
    strcpy(test_query, "select * from User u where u.username = \'");
    strcat(test_query, username);
    strcat(test_query, "\' ;");
    
    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    MYSQL_ROW row;

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }
    int count = 0;

    while ((row = mysql_fetch_row(result))) count++;

    mysql_close(con);
    return count;
}

//Return -1 if error, return userID if success
//Done
char * signIn(char *username, char *password) {
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
//Done
int signUp(char *username, char *password) {
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

    MYSQL_ROW row;

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    if ((row = mysql_fetch_row(result))) {
        mysql_close(con);
        return 0;
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
// Done
int getInfoByDay(FILE* fileName, char *typeID, char * locationID, char *date) {
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

    int num_fields = mysql_num_fields(result);

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
int getInfoByMonth(FILE* fileName, char *typeID, char * locationID, char *date) {
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

    int num_fields = mysql_num_fields(result);

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
int getInfoByYear(FILE* fileName, char *typeID, char * locationID, char *date) {
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

    int num_fields = mysql_num_fields(result);

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

    int num_fields = mysql_num_fields(result);

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

    int num_fields = mysql_num_fields(result);

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

//Done
int getTypeByLocation(FILE * fileName, char * locationID) {
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
    strcpy(test_query, "SELECT t.* FROM Data d JOIN Type t ON d.TypeId = t.TypeId WHERE d.LocationId = ");
    strcat(test_query, locationID);
    strcat(test_query, " GROUP BY d.TypeId;");

    

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    int num_fields = mysql_num_fields(result);

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

//Done
int registerInfo(char *userID, char *locationID, char *typeID) {
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
    strcpy(test_query, "INSERT INTO `Register` (`RegisterId`, `UserId`, `LocationId`, `TypeId`) VALUES (NULL, ");
    strcat(test_query, userID);
    strcat(test_query, ", ");
    strcat(test_query, locationID);
    strcat(test_query, ", ");
    strcat(test_query, typeID);
    strcat(test_query, ");");


    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }
    
    mysql_close(con);
    return 1;
}

//Done
int deleteRegisterInfo(char *userID, char *locationID, char *typeID) {
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
    strcpy(test_query, "DELETE FROM Register WHERE UserId = ");
    strcat(test_query, userID);
    strcat(test_query, " AND LocationId = ");
    strcat(test_query, locationID);
    strcat(test_query, " AND TypeId = ");
    strcat(test_query, typeID);
    strcat(test_query, ";");


    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }
    
    mysql_close(con);
    return 1;
}

int getTypeByUser(int type[], int*len, char * userID, char* locationID) {
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
    snprintf(test_query, sizeof(test_query), "select TypeId from Register where LocationId = %s and UserID = %s;", locationID, userID); 
    
    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }


    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    int num_fields = mysql_num_fields(result);

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

int insertValueFromSensor(char *typeID, char* locationID, char* value) {
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
    strcpy(test_query, "INSERT INTO `Data` (`DataId`, `LocationId`, `TypeId`, `Value`, `Time`) VALUES (NULL, ");
    strcat(test_query, locationID);
    strcat(test_query, ", ");
    strcat(test_query, typeID);
    strcat(test_query, ", ");
    strcat(test_query, value);
    strcat(test_query, ", current_timestamp());");

    if(mysql_query(con, test_query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }
    
    mysql_close(con);
    return 1;
}

// update du lieu tu mang
int updateDatabase();
// int main(){
    

//     int temp[1000];
//     int len;
//     getTypeByUser(temp, &len, "4", "24");
//     for (int i = 0; i < len; i++) {
//         printf("%d ", temp[i]);
//     }

// 	return 0;
// }



