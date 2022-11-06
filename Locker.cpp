#include <iostream>
#include <conio.h>      //for getch()
#include <fstream>      //for file handling
#include <windows.h>    //for pop out window
#include <ctime>        //for time
#include <limits>       //for clearing input buffer
#include "LockNumOutOfRange.h"

using namespace std;

class Locker{
  private:

    struct Date{  // a date has day 'd', month 'm' and year 'y'
        int d, m, y;
    };

    struct node{
        int lockNum, code;
        string name, passwd, contactNo, status;
        Date startDate, endDate;
        node *next;
    };

    time_t now = time(0);
    tm *ltm = localtime(&now);

    const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    ifstream fin;
    ofstream fout;
    int size;
    node *head, *tail;

  public:
    Locker(){
        head = NULL;
        tail = NULL;
        size = 0;
    }

    ~Locker(){
        node *cur = head;
        node *tmp;
        while( cur != NULL ){
            tmp = cur->next;
            delete cur;
            cur = tmp;
        }
        head = NULL;
    }

    bool isEmpty() const{
       return size == 0;
    }

    int getSize() const{
       return size;
    }

    bool findReservedloc(int locknum){
        node *cur = head;
        int i=0;
        while(i<getSize()){
            if ((cur->lockNum)==locknum){
                return 1;
            }
            cur = cur->next;
            i++;
        }
        return 0;
    }

    void display(){
        string a;
        cout << "\n\t|-------------------------------|\n";
        for (int i =1; i <= 20; i++)
        {
            if(findReservedloc(i))
                a= "N/A";
            else
                a=to_string(i);
            cout << "\t| " << a;
            if ((i) % 4 == 0)
            {
                cout << "\t|";
                cout << "\n\t|-------------------------------|\n";
            }
        }
    } //end of display

    void reserve(string name){
        cout << endl;
        cout << "  Reserve a locker" << endl;
        cout << "  -----------------" << endl;

        readFromFile();
        display();

        cout << endl;
        int lockNum = getLockNum();
        node *cur = head;

        if (getSize() == 0){
            size++;
            node* n1= new node;
            n1->name = name;
            n1->lockNum = lockNum;
            n1->status = "No";
            n1->startDate = {ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900};
            Date endDate = calcEndDate(n1->startDate, 30);
            n1->endDate = {endDate.d, endDate.m, endDate.y};
            n1->code = NULL;
            head=n1;
            tail=n1;
            MessageBox( 0, "Locker reserved successfully!", "Operation Successful", MB_ICONINFORMATION );
            saveToFile();
            return;
        }else{
            for (int skip = 0; skip < getSize(); skip++){
                if (cur->lockNum == lockNum){
                    MessageBox( 0, "Locker has been reserved by someone else!", "Error", MB_ICONINFORMATION );
                    break;
                    break;
                }else if ((skip == getSize()-1) && (cur->lockNum != lockNum)){
                    size++;
                    node* n1= new node;
                    n1->name = name;
                    n1->lockNum = lockNum;
                    n1->status = "No";
                    n1->startDate = {ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900};
                    Date endDate = calcEndDate(n1->startDate, 30);
                    n1->endDate = {endDate.d, endDate.m, endDate.y};
                    n1->code = NULL;
                    n1->next = head;
                    head = n1;
                    MessageBox( 0, "Locker reserved successfully!", "Operation Successful", MB_ICONINFORMATION );
                    saveToFile();
                    break;
                }
                cur = cur->next;
            }
        } //end of outer if-else statement
    } //end of reserve

    void cancel(string name){
        cout << endl;
        if(isEmpty()){
            MessageBox( 0, "No reservation found", "Note", MB_ICONINFORMATION );
            return;
        }else{
            cout << "  Cancel a reservation" << endl;
            cout << "  ---------------------" << endl;
            cout << "  *Note: Reservations with parcels delivered cannot be canceled." << endl << endl;
            int lockNum = getLockNum();

            node *cur = head;
            for (int skip = 0; skip < getSize(); skip++){
                if ((cur->lockNum == lockNum) && (cur->name == name) && (cur->status == "No")){
                    if (getSize() == 1 || skip == 0){ //delete first reservation of list
                        cur = head;
                        head = head->next;
                    }else{
                        node *prev;
                        prev = head;
                        for (int skip2 = 0; skip2 < skip-1; skip2++){ //find previous reservation
                            prev = prev->next;
                        }
                        cur = prev->next;
                        prev->next = cur->next;
                    }
                    cur->next = NULL;
                    delete cur;
                    cur = NULL;
                    size--;
                    MessageBox( 0, "Reservation cancelled!", "Operation Successful", MB_ICONINFORMATION );
                    saveToFile();
                    return;
                } else if ((cur->lockNum == lockNum) && (cur->name == name) && (cur->status == "Yes")){
                    MessageBox( 0, "Reservation cannot be cancelled!", "Error", MB_ICONINFORMATION );
                    return;
                }
                cur = cur->next;

            } //end of for-loop
            MessageBox( 0, "Reservation not found", "Error", MB_ICONINFORMATION );
        } //end of outer if-else statement
        saveToFile();
    } //end of cancel

    void readFromFile(){
        fin.open("locker.txt", ios::app); //open the text file
        fin.seekg(0, ios::end); // put the "cursor" at the end of the file
            if (fin.tellg()!= 0) { //if the file is not empty
                fin.seekg(0, ios::beg); //put back to beginning of file
                fin >> size ;
                if ( !fin.eof() ){ // if not end of filecout
                     head = new node;
                     fin >> head->lockNum; // Add the first data to the first node.
                     fin >> head->name;
                     fin >> head->startDate.d;
                     fin >> head->startDate.m;
                     fin >> head->startDate.y;
                     fin >> head->endDate.d;
                     fin >> head->endDate.m;
                     fin >> head->endDate.y;
                     fin >> head->status;
                     fin >> head->code;

                     head->next = NULL;
                     tail = head;

                     while ( !fin.eof() ){
                        tail->next = new node;
                        tail = tail->next;
                        fin >> tail->lockNum ; // Add remaining data to the next node.
                        fin >> tail->name;
                        fin >> tail->startDate.d;
                        fin >> tail->startDate.m;
                        fin >> tail->startDate.y;
                        fin >> tail->endDate.d;
                        fin >> tail->endDate.m;
                        fin >> tail->endDate.y;
                        fin >> tail->status;
                        fin >> tail->code;

                        tail->next = NULL;
                     }  //end while
                } //end if
            } //end of outer if
            fin.close(); //close file
    } //end of readFromFile

    void viewStatus(string name){
        cout << endl;
        cout << "  Status of locker(s)" << endl;
        cout << "  --------------------" << endl;
        cout << "  *Note: Reservations will be automatically cancelled once" << endl;
        cout << "   the expiry date is exceeeded, without prior notice. "<< endl;
        cout << "   Any parcel left in the locker will be sent to warehouse." << endl << endl;
        readFromFile();
        if(isEmpty()){
            MessageBox( 0, "No reservation found", "Note", MB_ICONINFORMATION );
            return;
        } else{
            node *cur;

            //display reservation(s) with status "Yes" first
            readFromFile();
            int found = 0;
            cur = head;
            for (int skip = 0; skip < getSize(); skip++){
                if (cur->name == name && cur->status == "Yes"){
                    cout << "  Locker number  : " << cur->lockNum << endl;
                    cout << "  Delivered      : " << cur->status << endl;
                    cout << "  Delivery Date  : " << cur->startDate.d << "/" << cur->startDate.m << "/" << cur->startDate.y<< endl;
                    cout << "  Expiry date    : " << cur->endDate.d << "/" << cur->endDate.m << "/" << cur->endDate.y << endl;
                    Date curDate = {ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900}; //today's date
                    cout << "  Remaining days : " << getDifference(curDate, cur->endDate) << endl;
                    cout << "  Unique code    : " << cur->code << endl;
                    found++;
                    cout << endl;
                }
                cur = cur->next;

            } //end of for-loop

            //Then, display reservation(s) with status "No"
            readFromFile();
            int found2 = 0;
            cur = head;
            for (int skip = 0; skip < getSize(); skip++){
                if (cur->name == name && cur->status == "No"){
                    cout << "  Locker number  : " << cur->lockNum << endl;
                    cout << "  Delivered      : " << cur->status << endl;
                    cout << "  Booking Date   : " << cur->startDate.d << "/" << cur->startDate.m << "/" << cur->startDate.y<< endl;
                    cout << "  Expiry date    : " << cur->endDate.d << "/" << cur->endDate.m << "/" << cur->endDate.y << endl;
                    Date curDate = {ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900}; //today's date
                    cout << "  Remaining days : " << getDifference(curDate, cur->endDate) << endl;
                    found2++;
                    cout << endl;
                }
                cur = cur->next;
            } //end of for-loop
            if ((found == 0) && (found2 == 0))
                MessageBox( 0, "No reservation found", "Note", MB_ICONINFORMATION );

        } //end of outer if-else
        cout << "  Press any key to continue..";
        getch();
    } //end of viewStatus

    void dropOff(){
        cout << endl;
        cout << "  Drop Off" << endl;
        cout << "  ---------" << endl;
        int lockNum = getLockNum();
        readFromFile();
        node *cur = head;
        for (int skip = 0; skip < getSize(); skip++){
            if (cur->lockNum == lockNum && cur->status == "No"){
                cur->status = "Yes";
                MessageBox( 0, "Drop off successful!", "Operation Successful", MB_ICONINFORMATION );
                cur->code = generateCode();
                cur->startDate = {ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900};
                Date endDate = calcEndDate(cur->startDate, 14);
                cur->endDate = {endDate.d, endDate.m, endDate.y};
                saveToFile();
                return;
            }
            else
                cur = cur->next;
        }
        MessageBox( 0, "Reservation not found", "Error", MB_ICONINFORMATION );
    } //end of dropOff

    void saveToFile(){
        sort_reservation(&head);

        fout.open("locker.txt");
        fout << size << endl;
        node* cur = head;
        for (int i=0; i < getSize(); i++){
             fout << cur->lockNum << " ";
             fout << cur->name << " ";
             fout << cur->startDate.d << " ";
             fout << cur->startDate.m << " ";
             fout << cur->startDate.y << " ";
             fout << cur->endDate.d << " ";
             fout << cur->endDate.m << " ";
             fout << cur->endDate.y << " ";
             fout << cur->status << " ";
             fout << cur->code << endl;
             cur = cur->next;
        }
        fout.close();
        fin.close();
    } // end of saveToFile

    void showNoti(string name){
        node *cur;
        cur = head;

        for (int i = 0; i<getSize(); i++){
            if (cur->status == "Yes" && cur->name == name){
                MessageBox( 0, "Yeah ! Your parcel is delivered.\nPlease go to \"Status of locker(s)\" to get a unique code to retrieve your parcel!", "Parcel Arrived", MB_ICONINFORMATION );
                break;
            }
            else
                cur = cur->next;
        } //end of for-loop
    } //end of showNoti

    void collection(){
        cout << endl;
        int code, lnum;
        cout << "  Collection" << endl;
        cout << "  -----------" << endl;
        lnum = getLockNum();
        cout << "  Enter unique code  : " ;
        cin >> code;
        readFromFile();
        node *cur = head;
        for (int skip = 0; skip < getSize(); skip++){
            if (lnum == cur->lockNum && code == cur->code){
                if (getSize() == 1 || skip == 0){ //delete first reservation of list
                    cur = head;
                    head = head->next;
                } else{
                    node *prev;
                    prev = head;
                    for (int skip2 = 0; skip2 < skip-1; skip2++){ //find previous reservation
                        prev = prev->next;
                    }
                    cur = prev->next;
                    prev->next = cur->next;
                }
                cur->next = NULL;
                MessageBox( 0, "Parcel is collected!\nThank you for using our service!", "Operation Successful", MB_ICONINFORMATION );
                delete cur;
                cur = NULL;
                size--;
                saveToFile();
                return;
            }
            cur = cur->next;
        }
        MessageBox( 0, "Invalid locker number or code", "Error", MB_ICONINFORMATION );
     } //end of collection

    int generateCode(){
        int code;

        srand((unsigned) time(0));
        code = (rand() % 899999 + 100000);

        return code;
    } //end of generate code

    int getLockNum(){
        double lnum;
        cin.clear(); //clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer
        cout << "  Enter locker number: ";
        cin >> lnum;

        int lockNum = (int)lnum;

        try{
            if((lockNum < 1) || (lockNum > 20)) {
                throw LockNumOutOfRange("Invalid input.");
            }else if((lockNum >= 1) || (lockNum <= 20)){
                return lockNum;
            }
        }catch(LockNumOutOfRange a){
            MessageBox( 0, "Locker number must be an integer range between 1 to 20. \nPlease input again.", "Error", MB_ICONINFORMATION );
            lockNum = getLockNum();
            return lockNum;
        }

    } //end of getLockNum

    int countLeapYears(Date d){
        int years = d.y;

        //check if the current year needs to be considered for the count of leap years or not
        if (d.m <= 2)
            years--;

        //a year is a leap year if it is a multiple of 4,
        //multiple of 400 and not a multiple of 100.
        return years / 4 - years / 100 + years / 400;
    } //end of countLeapYears

    int getDifference(Date dt1, Date dt2){ //returns number of days between two dates
        //count total number of days before dt1

        long int n1 = dt1.y*365 + dt1.d;   //initialize count using years and day

        for (int i=0; i< (dt1.m-1); i++)  //add days for months in given date
            n1 += monthDays[i];

        //since every leap year has 366 days,
        //add a day for every leap year
        n1 += countLeapYears(dt1);

        //count total number of days before dt1

        long int n2 = dt2.y*365 + dt2.d;

        for (int i=0; i<dt2.m - 1; i++)
            n2 += monthDays[i];

        n2 += countLeapYears(dt2);

        return (n2 - n1);   // return difference between two counts
    } //end of getDifference

    Date calcEndDate(Date startDate, int duration){  //calculate the expiry date for locker reservation
        int day = startDate.d;
        int month = startDate.m;
        int year = startDate.y;
        int counter = -1;
        int counter2 = -1;
        int tmp = day + duration;
        int extraDay = 0;

        if((year % 4 == 0) && (month == 2))    //if it is leap year and February, need to add one more day
                extraDay = 1;

        if (tmp <= (monthDays[month-1]+extraDay)) //if the end date is still within the same month
            day = tmp;

        while (tmp > (monthDays[month-1] + extraDay)){
            extraDay = 0;

            if((year % 4 == 0) && (month == 2))
                extraDay = 1;

            tmp = tmp - (monthDays[month-1] + extraDay);
            day = tmp;
            counter++;
            month++;
            while(month > 12){
                month = month - 12;
                counter2++;
                year++;
            }
        } //end of outer while-loop

        Date endDate = {day, month, year};
        return endDate;
    } //end of calcEndDate

    void checkExpiry(){
        readFromFile();
        Date curDate = {ltm->tm_mday, ltm->tm_mon+1, ltm->tm_year+1900}; //today's date

        node *cur = head;
        node *del;
        for (int skip = 0; skip < getSize(); skip++){
            if ((getDifference(curDate, cur->endDate)) < 0){    //if remaining days < 0
                if (getSize() == 1 || skip == 0){  //delete first reservation of list
                    cur = head;
                    head = head->next;
                } else{
                    node *prev;
                    prev = head;
                    for (int skip2 = 0; skip2 < skip-1; skip2++)//find previous reservation
                        prev = prev->next;

                    cur = prev->next;
                    prev->next = cur->next;
                }
                del = cur;
                del->next = NULL;
                delete del;
                del = NULL;
                size--;
            } // end of if
            cur = cur->next;
        } //end of for-loop
         saveToFile();
    } //end of checkExpiry

    void sort_reservation(node** head){
        bool swapped;
        node** cur = NULL; //double pointer used to keep track of current node
        node* tmp = NULL;

        for (int i = 0; i < getSize(); i++){
            cur = head;
            swapped = false;
            for (int j = i+1; j < getSize(); j++){
                node* first = *cur;
                node* second = first->next;

                if (first->lockNum > second->lockNum){
                    tmp = second->next;
                    second->next = first;
                    first->next = tmp;
                    *cur = second;
                    swapped = true;
                }
                cur = &(*cur)->next;  //point to the next node
            }
            // break if the loop ended without any swap
            if (!swapped)
                break;
        }
    } //end of sort_reservation
}; //end of class Locker
