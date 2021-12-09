#ifndef THREADQUEUE_H_
#define THREADQUEUE_H_
#define LOCATION_MAX 5000
#define SENSOR_MAX 50

struct Node {
	int* confd;
	struct Node* next;
};

char* current_values[LOCATION_MAX][SENSOR_MAX];

void fakeData() {
	for (int i = 0; i < LOCATION_MAX; i ++)
		for (int j = 0; j < SENSOR_MAX; j++)
			current_values[i][j] = "1.2";
}




// struct tempNode {
// 	int typeOfNode;		// 0 if sensor, 1 if client
// 	char *nodeID;		// ID of sensor or client depend on typeOfNode
// 	int* confd;
	
// 	char *locationID;	// Only sensor
// 	char *value;		// Only sensor
// 	struct tempNode* next;

// };



/**
* Y tuong khi client goi getInfoNow
* Duyet vong for qua cac node de lay du lieu tu sensor
*/


typedef struct Node nodex;

int* dequeue();
void enqueue();

#endif