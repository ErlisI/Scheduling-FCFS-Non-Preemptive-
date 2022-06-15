//
//  main.cpp
//  Lab #4
//
//  Created on 15.3.22.
//

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Customer {
    char customerName;
    int arrivalTime;
    int serviceTime;
    int turnaroundTime;
    int waitingTime;
    void setCustomer(char, int, int);
};

class Node {
public:
    Customer data;
    Node* next = NULL;
    Node(Customer d):data(d){};
};

class FCFSQueue {

private:
    Node* head = NULL;
    Node* tail = NULL;
    int length = 0;

public:
    //In-line method (since is just 3 lines). It just checks if the queue is empty or not
    bool IsEmpty(){
        if(length == 0)
            return true;

        return false;
    }
    int GetLength() const {return length;} //In-line method, since it just returns th length of the queue
    void Enqueue(Customer);
    Customer Dequeue();
    Customer peek();
    void display();
    void ghanttDisplay();
};

void readData(ifstream&, FCFSQueue&);

//Global variables to make the calculations easier
int gTime = 0;
int finishTime = 0;
double waitingTimeAvg = 0.0;

int main() {

    FCFSQueue jobQue; //This object is used to get the data
    FCFSQueue readyQue;//This object is used to calculate the data
    FCFSQueue finalQue;//This object is used to display everything

    Customer customerS;

    ifstream inFile;

    //Calls the function which reads the data
    readData(inFile, jobQue);

    //This is the logic
    while(!jobQue.IsEmpty()){

        //This is the simulation which adds the customer to the readQue based on the time that they came
        while(!jobQue.IsEmpty() && jobQue.peek().arrivalTime <= gTime){
                customerS = jobQue.Dequeue();
                readyQue.Enqueue(customerS);
        }

        //This does the calculations for everything in the customer struct
        customerS = readyQue.Dequeue();

        gTime += customerS.serviceTime;
        customerS.turnaroundTime = gTime - customerS.arrivalTime;

        customerS.waitingTime = customerS.turnaroundTime - customerS.serviceTime;
        waitingTimeAvg += customerS.waitingTime;

        //This adds the data to the final queue just so everything can be displayed nice
        finalQue.Enqueue(customerS);
    }

    //This is very similar to the one above, and that is because some customers might come at the same time, and won't get added to the Final Que
    while(!readyQue.IsEmpty()){

        customerS = readyQue.Dequeue();

        gTime += customerS.serviceTime;
        customerS.turnaroundTime = gTime - customerS.arrivalTime;

        customerS.waitingTime = customerS.turnaroundTime - customerS.serviceTime;
        waitingTimeAvg += customerS.waitingTime;

        finalQue.Enqueue(customerS);
    }

    //Displays everything nice and in order, as shown in Professors example
    finalQue.display();

    return 0;
}

//This function reads the data from the file to the jobQue
void readData(ifstream& inFile, FCFSQueue& jobQue){

    char customerN;
    int arrivalT;
    int serviceT;

    Customer customerS;

    inFile.open("CHANGE THIS!!!"); // Open file

    if (!inFile) { //check if file was opened correctly
        cout << "File can't be opened" << endl;
        exit(1);
    }

    //Getting the data and equeing them to the jobQue
    while (inFile>>customerN>>arrivalT>>serviceT) {

        customerS.setCustomer(customerN, arrivalT, serviceT);
        jobQue.Enqueue(customerS);

    }
    inFile.close();
}

//This function just sets the data of the customer
void Customer::setCustomer(char cName, int aTime, int sTime){
    this->customerName = cName;
    this->arrivalTime = aTime;
    this->serviceTime = sTime;
}

//Equeueing the Customer, which is basically the append function for linked list
void FCFSQueue::Enqueue(Customer customer){

    Node* newNode = new Node(customer);

    if (!newNode)
        return;

    //if list is not empty
    if (this->head != NULL) {
        this->tail->next = newNode;
        this->tail = newNode;
        this->length++;

    //if list is empty
    }else {
        this->head = newNode;
        this->tail = newNode;
        this->length++;
    }
}

//Dequeueing the Customer
Customer FCFSQueue::Dequeue(){

    Node* temp = this->head;


    //List is empty
    if (this->head == NULL){
        return Customer();
    }
     //
    Customer c = this->head->data;

    //List has only 1 customer
    if (this->head->next == NULL){
        delete this->head;
        this->head = NULL;
        this->tail = NULL;
        this->length--;
        return c;
    }

    //List has more than 1 customers
    this->head = temp->next;
    this->length--;
    delete temp;
    return c;
}

//This method just returns the first customer in the queue
Customer FCFSQueue::peek(){

    if (this->head == NULL){
        return Customer();
    }

    return this->head->data;

}

//Displays all the data
void FCFSQueue::display(){


    Node* temp = this->head;

    cout<<"FCFS (non-preemptive)"<<endl;
    cout<<"---------------------"<<endl;

    cout<<endl;

    cout<<"Total Time Required Is: "<<gTime<<" Time Units!"<<endl;

    cout<<endl;

    //Displaying and calculating the waiting time average
    cout<<"Average Waiting Time Is: "<<waitingTimeAvg/static_cast<double>(this->length)<<" Time Units!"<<endl;

    cout<<endl;

    //Displaying the Ghantt Chart
    ghanttDisplay();

    cout<<endl;

    //Displaying the data of each customer
    while(temp != NULL){

        cout<<"Process "<<temp->data.customerName<<":"<<endl;
        cout<<"----------"<<endl;
        cout<<"Service Time = "<<temp->data.serviceTime<<endl;
        cout<<"Turnaround Time = "<<temp->data.turnaroundTime<<endl;
        temp = temp->next;
        cout<<endl;
    }
}

//This method displays the Ghantt Chart
void FCFSQueue::ghanttDisplay(){

    Node* temp = this->head;

    cout<<"Ghantt Chart"<<endl;
    cout<<"------------"<<endl;

    cout<<"Time: |"<<setw(3);

    //Going through the whole linked list and displaying the finish time
    while(temp != NULL){

        cout<<finishTime<<setw(3)<<"|"<<setw(3);
        finishTime += temp->data.serviceTime;

        temp = temp->next;

        if(temp == NULL)
            cout<<finishTime<<setw(3)<<"|";
    }

    cout<<endl;

    temp = this->head;

    cout<<"CPU:  |"<<setw(3);

    //Going through the whole linked list and displaying the customer/process (CPU)
    while(temp != NULL){

        cout<<temp->data.customerName<<setw(3)<<"|"<<setw(3);
        temp = temp->next;
    }

    cout<<endl;
}
