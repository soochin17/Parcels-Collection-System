#include <iostream>
#include <fstream>  //for file handling
#include <limits>   //for clearing input buffer
#include "Locker.cpp"

using namespace std;

int main(){
    system("title Parcels Collection System - A Better Way To Collect Parcel!");
    string name, passwd, contactNo, input_name, input_passwd;
    int choice1, choice2;
    Locker a;
    ifstream fin;
    ofstream fout;

    do{
        system("CLS");
        cout << "   ___" << endl;
        cout << "  /__/| PARCELS" << endl;
        cout << "  |__|/ COLLECTION SYSTEM " << endl;
        cout << "  --------------------" << endl;
        cout << "  | 1. Register      |" << endl;
        cout << "  | 2. Login         |" << endl;
        cout << "  | 3. Drop off      |" << endl;
        cout << "  | 4. Collect       |" << endl;
        cout << "  --------------------" << endl;
        cout << "  > Choice (5 to Quit): ";
        cin >> choice1;
        switch(choice1){
            case 1:{
                bool taken;
                bool valid;
                cin.ignore();
                do{
                    taken = false;
                    valid = true;
                    cout << endl;
                    cout << "  Register" << endl;
                    cout << "  ----------" << endl;
                    cout << "  *Username should not contain any spacings." << endl << endl;
                    cout << "  Enter username : ";
                    getline(cin, input_name);

                    int length = input_name.size();     //get size of user input name
                    char check[length+1];
                    strcpy(check, input_name.c_str());  //convert string to char array

                    if(length == 0){    //check if input name is empty
                        MessageBox( 0, "Username cannot be empty!", "Error", MB_ICONINFORMATION );
                        valid = false;
                    }

                    for(int i=0; i < length; i++){      //check if the input name contains any spacings
                        if(isspace(check[i])){
                            MessageBox( 0, "Username should not contain any spacings.", "Error", MB_ICONINFORMATION );
                            valid = false;
                            break;
                        }
                    }

                    fin.open("user.txt", ios::app);
                    while (!fin.eof()){  //while not the end of file
                        fin >> name;      // read these values and put into these variables
                        fin >> passwd;
                        if (valid && input_name == name){
                            taken = true;
                            MessageBox( 0, "Username taken. Please enter another username.", "Error", MB_ICONINFORMATION );
                            break;
                        }
                    }
                    fin.close();
                    if(!taken && valid){
                        int ch;
                        cout << "  Enter password : ";
                        input_passwd = "";
                        while(ch = getch()){
                            if (ch == 13){ //ASCII value for enter key
                                fout.open("user.txt", ios::app);
                                fout << input_name << " ";
                                fout << input_passwd << endl;
                                fout.close();
                                MessageBox( 0, "You have registered successfully!", "Operation Successful", MB_ICONINFORMATION );
                                break;
                            }else if (ch == 8){ //ASCII value for backspace
                                if(input_passwd.length() > 0){
                                    cout<<"\b \b";//remove  * on screen;
                                    input_passwd.erase(input_passwd.length()-1); //erase String length
                                }
                            }else{
                                cout<<"*";
                                input_passwd += ch;
                            }
                        } //end while
                    } //end of outer if
                }while (taken || !valid);
                break;
            } //end of case 1
            case 2:{
                cin.ignore();
                int ch;
                cout << endl;
                cout << "  Login" << endl;
                cout << "  -------" << endl;
                cout << "  Enter username : ";
                getline(cin, input_name);

                cout << "  Enter password : ";
                input_passwd = "";
                while(ch = getch()){
                    if(ch == 13) { //ASCII value for enter key
                        int found = 0;
                        fin.open("user.txt", ios::app);
                        while (!fin.eof() && (found == 0)){  //while not the end of file & no users found
                            fin >> name;      // read these values and put into these variables
                            fin >> passwd;
                            if ((input_name == name) && (input_passwd == passwd)){
                                found++;
                                fin.close();
                                input_passwd = "";
                                cout << endl;
                                system("CLS");
                                cout << endl;
                                a.checkExpiry();    //check if there is any expired reservation and remove them
                                a.showNoti(name);
                                do{
                                    system("CLS");
                                    cout << endl;
                                    cout << "  Logged in as: " << name << endl;
                                    cout << "  ----------------------------" << endl;
                                    cout << "  | 1. Reserve a locker      |" << endl;
                                    cout << "  | 2. Cancel a reservation  |" << endl;
                                    cout << "  | 3. Status of locker(s)   |" << endl;
                                    cout << "  ----------------------------" << endl;
                                    cout << "  > Choice (5 to logout): ";
                                    cin >> choice2;

                                    switch(choice2){
                                        case 1:{
                                            a.reserve(name);
                                            break;
                                        }case 2:{
                                            a.cancel(name);
                                            break;
                                        }case 3:{
                                            a.viewStatus(name);
                                            break;
                                        }case 5:{
                                            break;
                                        }default:{
                                            cin.clear(); //clear error flag
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer
                                            MessageBox( 0, "Invalid input!", "Error", MB_ICONINFORMATION );
                                        }
                                    } //end of switch case
                                }while (choice2 != 5);
                            } else if (fin.eof() && found == 0){
                                MessageBox( 0, "Invalid name or password!", "Error", MB_ICONINFORMATION );
                                fin.close();
                                input_passwd = "";
                            } //end of if-elseif statement
                        } //end while
                        break;
                    }else if (ch == 8){ //ASCII value for backspace key
                        if(input_passwd.length() > 0){
                            cout<<"\b \b";//remove  * on screen;
                            input_passwd.erase(input_passwd.length() - 1); //erase String length
                        }
                    } else{
                          cout<<"*";
                          input_passwd += ch; //the input password was assigned to variable.
                    }
                }
                break;
            } //end of case 2
            case 3:{
                a.dropOff();
                break;
            }
            case 4:{
                a.collection();
                break;
            }
             case 5:{ //any char other than numbers will be 0
                MessageBox( 0, "Thank you for using the system! \nSee you again!", "Thank you message", MB_ICONINFORMATION );
                break;
            }
            default:
                cin.clear(); //clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer
                MessageBox( 0, "Invalid input!", "Error", MB_ICONINFORMATION );
        } //end of switch
    }while (choice1 != 5);

    return 0;
} //end of main
