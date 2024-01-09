#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define small 0.008
#define large 0.02
#define N 3

typedef struct date{
	int day;
	int month;
	int year;
}DATE;
typedef struct order{
	char name[50];
	int numOfS;
	int numOfL;
	float cost;
	float finalCost;
	int discount;
	DATE start;
	DATE due;
	DATE finish;
}ORDER;



void savePendingOrdersToFile(ORDER o[]);
void loadPendingOrdersFromFile(ORDER o[]);
void saveFinishedOrdersToFile(ORDER o[]);
void loadFinishedOrdersFromFile(ORDER o[]);
bool userCheck(ORDER o);
DATE getCurrentDate(void);
int getUserCommand(void);
int findClient(ORDER o[]);
ORDER getOrder(void);
void showPendingOrders(ORDER o[]);
void showReadyOrders(ORDER o[]);
void showClosedOrders(ORDER o[]);
void executeOrder(ORDER o[]);
void finalizeOrder(ORDER o[]);
bool dateCheck2(DATE date1,DATE date2);
bool dateCheck1(DATE date);
void showClientOrders(ORDER o[]);

FILE *fptr;
const DATE nullDate = {0,0,0};
ORDER orders[N];
int main(int argc, char *argv[]) {
	int currentorder,selection;
	while ((selection=getUserCommand())!=0){
		switch (selection){
			case 0:{
				break;
			}
			case 1:{
				orders[currentorder]=getOrder();
				currentorder++;
				if (currentorder>N-1){
					currentorder=0;
				}
				break;
			}
			case 2:{
				showClientOrders(orders);
				break;
				}
			case 3:{
				showPendingOrders(orders);
				break;
			}
			 case 4: {
            	savePendingOrdersToFile(orders);
            	break;
        	}
        	case 5: {
            	loadPendingOrdersFromFile(orders);
            	break;
        	}
			case 6:{
				executeOrder(orders);
				break;
			}
			case 7:{
				showReadyOrders(orders);
				break;
			}
			case 8:{
				finalizeOrder(orders);
				break;
			}
			case 9:{
				showClosedOrders(orders);
				break;
			}
			case 10: {
            	saveFinishedOrdersToFile(orders);
            	break;
        	}
        	case 11: {
            	loadFinishedOrdersFromFile(orders);
            	break;
        	}
        	default:{
				break;
			}
		}
	}
	return 0;
}

int getUserCommand(void){
	int selection;
	printf("What do you want to do?\n"
			"0 to exit\n"
			"1 to place an order\n"
			"2 to show a customer's orders\n"
			"3 to show pending orders\n"
			"4 to save pending orders to file\n"
			"5 to load pending orders from file\n"
			"6 to start an order\n"
			"7 to show orders pending payment\n"
			"8 to pay for an order\n"
			"9 to show finished orders\n"
			"10 to save finished orders to a separate file\n"
			"11 to load from finished order file\n");
	scanf("%d",&selection);
	return selection;
}

int findClient(ORDER o[]){
	char clientName[50];
	int i;
	printf("Give the client's name:\n");
	scanf("%s",clientName);
	for (i=0;i<N;i++){
		if (strcmp(clientName,o[i].name)==0){
			printf("Client found.\n");
			return i;
		}	
	}
	printf("No client found.\n");
}

ORDER getOrder(void){
	ORDER o;
	printf("Give name:");
	scanf("%s",o.name);
	printf("Give number of small bottles: ");
	scanf("%d",&o.numOfS);
	printf("Give number of large bottles: ");
	scanf("%d",&o.numOfL);
	printf("Give initial Date:");
	scanf("%d/%d/%d",&o.start.day,&o.start.month,&o.start.year);
	if (dateCheck1(o.start)){
		printf("Invalid Date.\n");
		o.start=nullDate;
		return;
	}
	printf("Give Due Date:");
	scanf("%d/%d/%d",&o.due.day,&o.due.month,&o.due.year);
	if (dateCheck1(o.due)){
		printf("Invalid Date.\n");
		o.start=nullDate;
		o.due=nullDate;
		return;
	}
	if (dateCheck2(o.start,o.due)){
		printf("Invalid date combination.\n");
		o.start=nullDate;
		o.due=nullDate;
	}
	return o;
}

void showClientOrders(ORDER o[]){
	int i=findClient(o);
	if (userCheck(o[i])){
		return;
	}
	if (o[i].start.day==0 || o[i].due.day==0){
		printf("Error: Invalid Date\n");
		return;
	}
	printf("Here is %s's order\n",o[i].name);
	printf("Number of small bottles: %d\nNumber of large bottles: %d\n",
		    o[i].numOfS,o[i].numOfL);
	printf("Starting Date: %d/%d/%d\n"
		   "Due Date: %d/%d/%d\n",o[i].start.day,
		   o[i].start.month,o[i].start.year,
		   o[i].due.day,o[i].due.month,
		   o[i].due.year);
	if (o[i].cost==0){
		printf("Order status:Pending\n");
	} else if (o[i].finalCost==0){
		printf("Order status:Ready\n");
		printf("Price: %.2f\n",o[i].cost);
		
	} else {
		printf("Order status:Closed\n");
		printf("Price: %.2f\n",o[i].cost);
		printf("Final Price: %.2f\n",o[i].finalCost);
		printf("Discount: %d%%\n",o[i].discount);
		printf("Date Executed: %d/%d/%d\n",
			    o[i].finish.day,o[i].finish.month,o[i].finish.year);
	}
}

void showPendingOrders(ORDER o[]){
	int i;
	for (i=0;i<N;i++){
		if (o[i].cost==0){
			if (userCheck(o[i])){
				continue;
			}
			printf("Name: %s\nNumber of small bottles: %d\n"
				   "Number of large bottles: %d\n"
				   "Cost: %.2f\nStart Date: %d/%d/%d\n"
				   "Due Date: %d/%d/%d\n",
				   o[i].name,o[i].numOfS,o[i].numOfL,
				   o[i].cost,o[i].start.day,o[i].start.month,
				   o[i].start.year,o[i].due.day,
				   o[i].due.month,o[i].due.year);
		}
	}
}

void showClosedOrders(ORDER o[]){
	int i;
	for (i=0;i<N;i++){
		if (o[i].finalCost!=0){
			printf("Name: %s\n"
				   "Number of small bottles: %d\n"
				   "Number of large bottles: %d\n"
				   "Cost: %.2f\nFinal Cost: %.2f\n"
				   "Discount: %d%%\n"
				   "Date Executed: %d/%d/%d\n",
				   o[i].name,o[i].numOfS,o[i].numOfL,
				   o[i].cost,o[i].finalCost,
				   o[i].discount,o[i].finish.day
				   ,o[i].finish.month,o[i].finish.year);
		}
	}
}

void executeOrder(ORDER o[]){
	int i=findClient(o);
	if (userCheck(o[i])){
		return;
	}
	o[i].cost=(float)o[i].numOfS*small+o[i].numOfL*large;
	printf("The cost is %.2f\n",o[i].cost);
}

void showReadyOrders(ORDER o[]){
	int i;
	for (i=0;i<N;i++){
		if (o[i].cost!=0){
			printf("Name: %s\nNumber of small bottles: %d\n"
				   "Number of large bottles: %d\n"
				   "Cost: %.2f\nStart Date: %d/%d/%d\n"
				   "Due Date: %d/%d/%d\n"
				   ,o[i].name,o[i].numOfS,o[i].numOfL,
				   o[i].cost,o[i].start.day,
				   o[i].start.month,o[i].start.year,
				   o[i].due.day,o[i].due.month,o[i].due.year);
		}
	}
}

void savePendingOrdersToFile(ORDER o[]) {
    FILE *pendingFile = fopen("pendingOrders.txt", "w");
    if (pendingFile == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
	int i;
    for (i = 0; i < N; i++) {
    	if (userCheck(o[i])){
    		continue;
		}
        fprintf(pendingFile, 
			    "%s\t%d\t%d\t%.2f\t%.2f\t%d/%d/%d\t""%d/%d/%d\n",
				 o[i].name, o[i].numOfS, o[i].numOfL,
				 o[i].cost,o[i].finalCost,o[i].start.day,
				 o[i].start.month,o[i].start.year,
				 o[i].due.day,o[i].due.month,o[i].due.year);
    }

    fclose(pendingFile);
}

void loadPendingOrdersFromFile(ORDER o[]) {
    FILE *pendingFile = fopen("pendingOrders.txt", "r");
    if (pendingFile == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
	int i;
    for (i = 0; i < N; i++) {
        fscanf(pendingFile,
			   "%s\t%d\t%d\t%f\t%f\t%d/%d/%d\t%d/%d/%d\n",
				o[i].name, &o[i].numOfS, &o[i].numOfL,
				&o[i].cost, &o[i].finalCost, 
				&o[i].start.day, &o[i].start.month, 
				&o[i].start.year, &o[i].due.day, 
				&o[i].due.month, &o[i].due.year);

    }

    fclose(pendingFile);
}

void finalizeOrder(ORDER o[]){
	int i = findClient(o);
	o[i].finish = getCurrentDate();
	if (userCheck(o[i])){
		return;
	}
	if (o[i].cost==0){
		printf("Order is not ready\n");
		return;
	}
	int ordersize = o[i].numOfS + o[i].numOfL;
	if ((ordersize>3000||o[i].cost>200)&&o[i].cost<600){
		o[i].finalCost=o[i].cost-(o[i].cost*0.08);
		o[i].discount=8;
	} else if (o[i].cost>600){
		o[i].finalCost=o[i].cost-(o[i].cost*0.2);
		o[i].discount=20;
	}
	printf("The final price is %.2f\nWith a %d%% discount\n",
		   o[i].finalCost,o[i].discount);
}

void saveFinishedOrdersToFile(ORDER o[]) {
    FILE *finishedFile = fopen("finishedOrders.txt", "w");
    if (finishedFile == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
	int i;
    for (i = 0; i < N; i++) {
        if (o[i].finalCost!=0) {
            fprintf(finishedFile, 
				    "%s\t%d\t%d\t%f\t%f\t%d%%\t%d/%d/%d\t%d/%d/%d\t%d/%d/%d\n" ,
					o[i].name, o[i].numOfS, o[i].numOfL,o[i].cost,
					o[i].finalCost,o[i].discount,o[i].start.day,
					o[i].start.month,o[i].start.year,o[i].due.day,
					o[i].due.month,o[i].due.year,
					o[i].finish.day,o[i].finish.month,o[i].finish.year);
        }
    }

    fclose(finishedFile);
}


void loadFinishedOrdersFromFile(ORDER o[]) {
    FILE *finishedFile = fopen("finishedOrders.txt", "r");
    if (finishedFile == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
	int i;
    for (i = 0; i < N; i++) {
        fscanf(finishedFile, 
			   "%s\t%d\t%d\t%f\t%f\t%d%%\t%d/%d/%d\t%d/%d/%d\t%d/%d/%d\n", 
			   o[i].name, &o[i].numOfS, &o[i].numOfL,
			   &o[i].cost, &o[i].finalCost, &o[i].discount, 
			   &o[i].start.day, &o[i].start.month, &o[i].start.year, 
			   &o[i].due.day, &o[i].due.month, &o[i].due.year,
			   &o[i].finish.day,&o[i].finish.month,&o[i].finish.year);

    }

    fclose(finishedFile);
}

bool userCheck(ORDER o) {
    if (o.name[0] == '\0') {
        return true;  // The string is empty
    } else {
        return false;  // The string is not empty
    }
}

bool dateCheck1(DATE date){
	if (((date.month==4 || date.month==6 || date.month==9 || date.month==11) && date.day>30) || date.month>12 || date.day>31){
		return true;
	} else {
		return false;
	}
}

bool dateCheck2(DATE date1,DATE date2){
	if ((date1.day>date2.day && date1.month<date2.month) && date1.year>date2.year){
		return true;
	} else {
		return false;
	}
}

DATE getCurrentDate(void){
	DATE orderDate;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	orderDate.day = tm.tm_mday;
	orderDate.month = tm.tm_mon + 1;
	orderDate.year = tm.tm_year + 1900;
	return orderDate;
}
