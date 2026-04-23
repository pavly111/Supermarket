#include<iostream>
#include<fstream>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include<conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#else
#include <termios.h>
#include <unistd.h>
#endif
using namespace std;
string fix_width(const string& str) {
    string full_str=str;
    if (str.length() <= 10) {
         for(int i=0;i<10-str.length();i++){
            full_str+=" ";
         }
         return full_str;
    }
    if (str.length() > 10) {
        return str.substr(0, 7) + "...";
    }
    return str;
}
class shopping{
    private:
        int pcode; //product code
        float price;
        float dis; //discount
        string pname ; //name of the product

    public:
        //public function
        void menu();
        void administrator(); //for administrators
        void add(); //to add a product
        void edit(); //to edit the price, name etc
        void rem(); //to remove a product
        void edit_user();//to edit users
        void buyer(); //for buyer
        void list(); //to list all the items
        void receipt();  //to get the receipt
        //function to return back to adminstrator menu
        void exit_adminstrator (string input){
            if(input=="e"){
                administrator();
            }
        }
        //function to return back to buyer menu
        void exit_buyer(string input){
            if(input=="e"){
                buyer();
            }
        }
};

// Cross-platform getch for password input
#if !defined(_WIN32) && !defined(_WIN64)
char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}
#endif

// Replace all PlaySound calls with a cross-platform macro
#if defined(_WIN32) || defined(_WIN64)
#define PLAY_SOUND(file) PlaySound(TEXT(file), NULL, SND_FILENAME | SND_ASYNC)
#else
#include <cstdlib>
#define PLAY_SOUND(file) system((std::string("afplay ") + file + " &").c_str())
#endif

//declaring the definition of the menu function outside the class using the scope resolution operator
//scope resolution operator helps us in defining the function body outside the class
void shopping :: menu()
{
    m ://it is a label which will help us to jump to the main menu
    int choice;
    fstream username_data;//file store usernames and passwords
    string input;//to handle infinite loops and wrong choices
    string user="admin"; //intialized if it is first time
    string pass="admin"; //intialized if it is first time
    string hashpass; //to stored in file as a numbers not in it's original value
    hash<string> hashFunc;
    hashpass = to_string(hashFunc(pass));
    string username;//entered by user
    string password;//entered by user

    username_data.open("users.txt" , ios::in) ;//ios :: in -- is used to read from the file

    if(!username_data) //if the file doesnt exist
        {
            username_data.open("users.txt" , ios::app|ios::out);//ios::app is used to append to the file and ios:: out is used to write in the file
            username_data<<" "<<user<<" "<<hashpass<<"\n";
            username_data.close();
        }
    username_data.close();

    cout<<"\t\t\t\t\t\t ________________________________________________________________________________\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                              Supermarket Main Menu                             |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|________________________________________________________________________________|\n";
    cout<<endl;

    cout<<"\t\t\t\t\t\t|                               (1) Adminstrator                                 |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (2) Buyer                                        |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (3) Exit                                         |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";

    cout<<endl;

    cout<<"\t\t\t\t\t\t  Please Select By Number  : "; cin>>input ; cout<<endl ; //getting the user choice

    if(input != "1"&&input != "2"&&input !="3"){
        cout << "\033[31m";//red color
        PLAY_SOUND("error.wav");
        cout<<"\t\t\t\t\t\t  Invalid choice \n";
        cout<<"\t\t\t\t\t\t  Please select from the given Options \n";
        cout << "\033[0m";//return to white
        goto m;
    }

    choice=stoi(input);

    //using switch statement to execute different functionalities for different users

    switch(choice)
    {
      case 1: {
            cout << "\t\t\t\t\t\t  Please Login  \n";
            cout << "\t\t\t\t\t\t  Enter Your Username: ";
            cin >> username; // Getting the username of the user

            password.clear(); // Ensure password is empty before input
            cout << "\t\t\t\t\t\t  Enter Password: ";

            char ch;
            while (true) {
                ch = getch();
                if (ch == '\r' || ch == '\n') break; // Enter key
                if (ch == '\b') { // Handle Backspace
                    if (!password.empty()) {
                        cout << "\b \b"; // Remove last asterisk
                        password.pop_back();
                    }
                } else {
                    password += ch; // Add character to password
                    cout << '*'; // Display asterisk
                }
            }
            hash<string> hashFunc;
            password = to_string(hashFunc(password));

            // Authentication check
            username_data.open("users.txt");
            bool authenticated = false;
            username_data>>user>>pass; //if the file exists then we will iterate over the file values

            while(!username_data.eof()) //this will help us to iterate till the last of the file
            {
                if(user == username && pass ==password) //if a product with the same code exists then we will not add it again and thats why we are using token variable to keep count if it already exists or not
                {
                    authenticated = true;
                    break;
                }

                username_data>>user>>pass;
            }
            username_data.close(); //closing the file

            if (authenticated) {
                administrator();
            }
            else {
                cout << "\033[31m";//red color
                PLAY_SOUND("error.wav");
                cout << "\n\t\t\t\t\t\t  Invalid Username or Password \n";
                cout << "\033[0m";//return to white
                menu();
            }

            break;

            }

        case 2 :
            {
                buyer();
                break;
            }

        case 3 :
            {
                exit(0);//it will mark successfull termination of the programme
                break;
            }

    }
    goto m;

}

void shopping :: administrator()
{
    m: //label for goto option
    int choice;
    string input;
    cout<<endl;
    cout<<"\t\t\t\t\t\t ________________________________________________________________________________\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                              Adminstrator Menu                                 |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|________________________________________________________________________________|\n";
    cout<<endl;

    cout<<"\t\t\t\t\t\t|                               (1) Add new product                              |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (2) Modify the product                           |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (3) Remove the product                           |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (4) Edit Users                                   |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (5) Back to main menu                            |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";

    cout<<endl;

    cout<<"\n\t\t\t\t\t\t  Please Enter your choice : ";
    cin>>input;
    if(input != "1"&&input != "2"&&input !="3"&&input!="4"&&input!="5"){
            cout << "\033[31m";
            PLAY_SOUND("error.wav");
            cout<<"\t\t\t\t\t\t  Invalid choice \n";
            cout<<"\t\t\t\t\t\t  Please select from the given Options \n";
            cout << "\033[0m";
            goto m;
    }

    choice=stoi(input);

    switch(choice)
    {
        case 1:
            {
                add(); //to add a new product
                break;
            }
        case 2:
            {
                edit(); //to modify or edit the existing product
                break;
            }
        case 3:
            {
                rem(); //to remove the product
                break;
            }
        case 4:
            {
                edit_user();//to edit users
                break;
            }
        case 5:
            {
                menu(); //to display the menu
                break;
            }
    }

    goto m;
}


//this function will helps us to add the product in the cart
void shopping :: add()
{
    m:
    fstream data;//file store products data
    int product_code;
    int token = 0 ;
    float product_price;
    float product_discount;
    string product_name;

    string input_code;
    string input_price;
    string input_discount;

    cout<<"\n\n\t\t\t\t\t\t  [Add new product]  For administrator menu enter (e)";
    ic://to return if the code invalid(alpha detected)
    cout<<"\n\n\t\t\t\t\t\t  Enter Product Barcode : ";
    cin>>input_code;
        exit_adminstrator(input_code);

        for(int i=0;i<input_code.length();i++){
            if(isalpha(input_code[i])){
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout<<"\t\t\t\t\t\t  Invalid code, Try again \n";
                cout << "\033[0m";
                goto ic;
                }
            }
    pcode=stoi(input_code);
    cout<<"\n\n\t\t\t\t\t\t  Name of the Product : ";
    cin>>pname;
    exit_adminstrator(pname);
    ip://to return if the price invalid(alpha detected)
    cout<<"\n\n\t\t\t\t\t\t  Price of the Product : ";
    cin>>input_price ;
    exit_adminstrator(input_price);
    for(int i=0;i<input_price.length();i++){
        if(isalpha(input_price[i])){
            cout << "\033[31m";
            PLAY_SOUND("error.wav");
            cout<<"\t\t\t\t\t\t  Invalid price, Try again \n";
            cout << "\033[0m";
            goto ip;
        }
    }
    price=stoi(input_price);
    id://to return if the discount invalid (alpha detected)
    cout<<"\n\n\t\t\t\t\t\t  Discount on Product : ";
    cin>>input_discount;
    exit_adminstrator(input_discount);
        for(int i=0;i<input_discount.length();i++){
            if(isalpha(input_discount[i])){
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout<<"\t\t\t\t\t\t  Invalid discount, Try again \n";
                cout << "\033[0m";
                goto id;
        }
    }
    dis=stoi(input_discount);

    data.open("database.txt" , ios::in) ;//ios :: in -- is used to read from the file

    if(!data) //if the file doesnt exist
    {
        data.open("database.txt" , ios::app|ios::out);//ios::app is used to append to the file and ios:: out is used to write in the file
        data<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
        data.close(); //closing the file
    }
    else
    {
        data>>product_code>>product_name>>product_price>>product_discount; //if the file exists then we will iterate over the file values

        while(!data.eof()) //this will help us to iterate till the last of the file
        {
            if(product_code == pcode) //if a product with the same code exists then we will not add it again and thats why we are using token variable to keep count if it already exists or not
            {
                token++;
            }

            data>>product_code>>product_name>>product_price>>product_discount;
        }
        data.close(); //closing the file

        if(token == 1) //if the token value is incremented to 1 it means that the product is already present
        {
            cout << "\033[31m";
            PLAY_SOUND("error.wav");
            cout<<"\n\t\t\t\t\t\t  The code is taken !!";
            cout<<"\n\t\t\t\t\t\t  please try again";
            cout << "\033[0m";
            goto m;
        }
        else
        {
            data.open("database.txt" , ios::app|ios::out);//ios::app is used to append to the file and ios:: out is used to write in the file
            data<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
            data.close(); //closing the file
        }
    }
    cout<<"\033[32m";
    PLAY_SOUND("accept.wav");
    cout<<"\n\t\t\t\t\t\t  Record Inserted Successfully !! \n\n";
    cout<<"\033[0m";
}

void shopping :: edit()
{
    fstream data , data1; //creating two objects of the fstream class
    int pkey ; //to get the product code from the administrator
    int token = 0 ;
    int product_code;
    float product_price;
    float product_discount;
    string product_name ;

    string input_code;
    string input_price;
    string input_discount;

    cout<<"\n\t\t\t\t\t\t  [Modify the Product] For administrator menu enter (e)"<<endl;
    ik://to return if the code invalid(alpha detected)
    cout<<"\n\n\t\t\t\t\t\t  Enter Product Barcode : ";
    cin>>input_code;
    exit_adminstrator(input_code);

        for(int i=0;i<input_code.length();i++){
            if(isalpha(input_code[i])){
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout<<"\t\t\t\t\t\t  Invalid code, Try again \n";
                cout << "\033[0m";
                goto ik;
                }
            }
    pkey=stoi(input_code);

    data.open("database.txt" , ios::in);

    if(!data) //if the file doesnt exist
    {
        cout << "\033[31m";
        PLAY_SOUND("error.wav");
        cout<<"\n\n\t\t\t\t\t\t  File doesn't Exist ! ";
        cout << "\033[0m";
    }
    else
    {
        //we will write the updated value in the database1 file and then we will update its name to database after we make the changes
        data1.open("database1.txt" , ios::app|ios::out);

        data>>pcode>>pname>>price>>dis;

        while(!data.eof())
        {
            if(pkey == pcode) //if the product exists
            {
            ic://to return if the code invalid(alpha detected)
            cout<<"\n\n\t\t\t\t\t\t  Enter Product new Barcode : ";
            cin>>input_code;
            if(input_code=="e"){
                data.close();
                data1.close();
                remove("database1.txt");
                exit_adminstrator(input_code);
            }

            for(int i=0;i<input_code.length();i++){
                if(isalpha(input_code[i])){
                    cout << "\033[31m";
                    PLAY_SOUND("error.wav");
                    cout<<"\t\t\t\t\t\t  Invalid code, Try again \n";
                    cout << "\033[0m";
                    goto ic;
                    }
                }
                product_code=stoi(input_code);
                cout<<"\n\t\t\t\t\t\t  Name of the Product  : ";
                cin>>product_name;
                if(product_name=="e"){
                data.close();
                data1.close();
                remove("database1.txt");
                exit_adminstrator(product_name);
            }
                ip://to return if the price invalid(alpha detected)
                cout<<"\n\t\t\t\t\t\t  Price  : ";
                cin>>input_price ;
                if(input_price=="e"){
                data.close();
                data1.close();
                remove("database1.txt");
                exit_adminstrator(input_price);
            }
                for(int i=0;i<input_price.length();i++){
                    if(isalpha(input_price[i])){
                        cout << "\033[31m";
                        PLAY_SOUND("error.wav");
                        cout<<"\t\t\t\t\t\t  Invalid price, Try again \n";
                        cout << "\033[0m";
                        goto ip;
                    }
                        }
                product_price=stoi(input_price);
                id://to return if the discount invalid (alpha detected)
                cout<<"\n\t\t\t\t\t\t  Discount : ";
                cin>>input_discount;
                if(input_discount=="e"){
                data.close();
                data1.close();
                remove("database1.txt");
                exit_adminstrator(input_discount);
            }
                for(int i=0;i<input_discount.length();i++){
                    if(isalpha(input_discount[i])){
                        cout << "\033[31m";
                        PLAY_SOUND("error.wav");
                        cout<<"\t\t\t\t\t\t  Invalid discount, Try again \n";
                        cout << "\033[0m";
                        goto id;
                        }
                    }
                product_discount=stoi(input_discount);

                //enter the new record
                data1<<" "<<product_code<<" "<<product_name<<" "<<product_price<<" "<<product_discount<<"\n";
                cout << "\033[32m";
                PLAY_SOUND("accept.wav");
                cout<<"\n\t\t\t\t\t\t  Record Edited  "<<endl<<endl;
                cout << "\033[0m";
                token++; //incrementing token bcs we find the product with the same product code
            }
            else // when the pkey entered by the user doesnt matches with the existing products code
            {
                data1<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
            }

            data>>pcode>>pname>>price>>dis; //this will helps us in iterating again and again
        }
        //closing both the files
        data.close();
        data1.close();

        //we will firstly remove the old database file and then we will rename the database1.txt to database.txt
        remove("database.txt");
        rename("database1.txt" , "database.txt");

        if(token == 0) //it means that the product with the pcode is not present as the token value is not incremented
        {
            cout << "\033[31m";
            PLAY_SOUND("error.wav");
            cout<<"\n\t\t\t\t\t\t  Sorry! Record not found \n\n";
            cout << "\033[0m";
        }
    }
}

void shopping :: rem()
{
    fstream data , data1;//creating object of the fstream class
    int pkey ;
    int token = 0 ;
    string input_code;

    cout<<"\n\n\t\t\t\t\t\t  [Delete Product] For administrator menu enter (e)";

    ic://to return if the code invalid(alpha detected)
    cout<<"\n\n\t\t\t\t\t\t  Enter Product Barcode : ";
    cin>>input_code;
    exit_adminstrator(input_code);
        for(int i=0;i<input_code.length();i++){
            if(isalpha(input_code[i])){
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout<<"\t\t\t\t\t\t  Invalid code, Try again \n";
                cout << "\033[0m";
                goto ic;
                }
            }
    pkey=stoi(input_code);

    data.open("database.txt" , ios::in);//opening the file

    if(!data) //if file doesn't exist
    {
        cout << "\033[31m";
        PLAY_SOUND("error.wav");
        cout<<"\t\t\t\t\t\t  File doesn't Exist"<<endl;
        cout << "\033[0m";
    }
    else
    {
        data1.open("database1.txt" , ios::app|ios::out);
        data>>pcode>>pname>>price>>dis; //getting the values of the different variables so that we can iterate over the file

        while(!data.eof()) //we will iterate till the end of the file
        {
            if(pcode == pkey)
            {
                cout << "\033[32m";
                PLAY_SOUND("accept.wav");
                cout<<"\n\n\t\t\t\t\t\t  Product deleted Successfully "<<endl<<endl;
                cout << "\033[0m";
                token++; //incrementing the token because we find the product
            }
            else
            {
                data1<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";//writing into the file
            }

            data>>pcode>>pname>>price>>dis; //for iteratiion
        }

        //closing the opened files
        //and removing and renaming the files

        data.close();
        data1.close();

        remove("database.txt");
        rename("database1.txt" , "database.txt");

        if(token == 0)
        {
            cout << "\033[31m";
            PLAY_SOUND("error.wav");
            cout<<"\n\t\t\t\t\t\t  Record Not found \n"<<endl;
            cout << "\033[0m";
        }

    }
}

void shopping :: edit_user()
{
    m ://it is a label which will help us to jump to the mai meny
    int choice;
    fstream username_data;
    fstream username_data1;
    string input;
    string edited_username;
    string edited_password;
    string user;//from file
    string pass;//from file
    string username;//from user
    string password;//from user
    cout<<"\n\t\t\t\t\t\t  [Edit username and password]\n";
            cout << "\n\t\t\t\t\t\t  Enter Your Username: ";
            cin >> username; // Getting the username of the user

            password.clear(); // Ensure password is empty before input
            cout << "\n\t\t\t\t\t\t  Enter Password: ";

            char ch;
            while (true) {
                ch = getch();
                if (ch == '\r' || ch == '\n') break; // Enter key
                if (ch == '\b') { // Handle Backspace
                    if (!password.empty()) {
                        cout << "\b \b"; // Remove last asterisk
                        password.pop_back();
                    }
                } else {
                    password += ch; // Add character to password
                    cout << '*'; // Display asterisk
                }
            }
            hash<string> hashFunc;
            password = to_string(hashFunc(password));

            // Authentication check
            username_data.open("users.txt");
            bool authenticated = false;
            username_data>>user>>pass; //if the file exists then we will iterate over the file values

            while(!username_data.eof()) //this will help us to iterate till the last of the file
            {
                if(user == username && pass ==password) //if a product with the same code exists then we will not add it again and thats why we are using token variable to keep count if it already exists or not
                {
                    authenticated = true;
                    break;
                }
                username_data>>user>>pass;
            }
            username_data.close(); //closing the file
            if (authenticated==false) {
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout << "\n\t\t\t\t\t\t  Invalid Username or Password \n";
                cout << "\033[0m";
                administrator();
            }
            else
            {
            cout<<"\n\n\t\t\t\t\t\t  Enter the new username : ";
            cin>>edited_username;
            edited_password.clear(); // Ensure password is empty before input
            cout << "\n\t\t\t\t\t\t  Enter the new Password: ";

            char ch;
            while (true) {
                ch = getch();
                if (ch == '\r' || ch == '\n') break; // Enter key
                if (ch == '\b') { // Handle Backspace
                    if (!edited_password.empty()) {
                        cout << "\b \b"; // Remove last asterisk
                        edited_password.pop_back();
                    }
                } else {
                    edited_password += ch; // Add character to password
                    cout << '*'; // Display asterisk
                }
            }
            hash<string> hashFunc;
            edited_password = to_string(hashFunc(edited_password));
            username_data.open("users.txt" , ios::in);
            username_data1.open("users1.txt" , ios::app|ios::out);

            username_data>>user>>pass;

            while(!username_data.eof())
            {
                if(username==user && password==pass){
                    username_data1<<" "<<edited_username<<" "<<edited_password<<"\n";
                    cout << "\033[32m";
                    PLAY_SOUND("accept.wav");
                    cout<<"\n\n\t\t\t\t\t\t  Record Edited  "<<endl<<endl;
                    cout << "\033[0m";
                }
                else // when the pkey entered by the user doesnt matches with the existing products code
                {
                    username_data1<<" "<<user<<" "<<pass<<"\n";
                }
                username_data>>user>>pass; //this will helps us in iterating again and again
            }
            //closing both the files
            username_data.close();
            username_data1.close();

            //we will firstly remove the old database file and then we will rename the database1.txt to database.txt
            remove("users.txt");
            rename("users1.txt" , "users.txt");
            }
}

void shopping ::buyer()
{
    m: //label for goto statement
    string input;
    int choice;

    cout<<"\t\t\t\t\t\t ________________________________________________________________________________\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                                   Buyer Menu                                   |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|________________________________________________________________________________|\n";
    cout<<endl;

    cout<<"\t\t\t\t\t\t|                               (1) Buy product                                  |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";
    cout<<"\t\t\t\t\t\t|                               (2) Go back                                      |\n";
    cout<<"\t\t\t\t\t\t|                                                                                |\n";

    cout<<endl;

    cout<<"\t\t\t\t\t\t  Enter your Choice  : ";

    cin>>input;
        if(input != "1"&&input != "2"){
        cout << "\033[31m";
        PLAY_SOUND("error.wav");
        cout<<"\t\t\t\t\t\t  Invalid choice \n";
        cout<<"\t\t\t\t\t\t  Please select from the given Options \n";
        cout << "\033[0m";
        goto m;
    }

    choice=stoi(input);
    cout<<endl;
    switch(choice)
    {
        case 1:
            {
                receipt();
                break;
            }
        case 2:
            {
                menu();
                break;
            }

    }

    goto m;
}


//this function will displays all the items present to the users
void shopping :: list()
{
    fstream data;
    data.open("database.txt" , ios::in);

    cout<<"\n\n\t\t\t\t\t\t ___________________________________________________\n";
    cout<<"\t\t\t\t\t\t\tProduct No.\t Name \t\tPrice";
    cout<<"\n\t\t\t\t\t\t|___________________________________________________|\n";
    data>>pcode>>pname>>price>>dis;

    while(!data.eof())
    {
        cout<<"\t\t\t\t\t\t\t"<<pcode<<" \t\t "<<fix_width(pname)<<"\t"<<price<<"\n";
        data>>pcode>>pname>>price>>dis; //for iteration purpose
    }

    data.close();

}

void shopping :: receipt()
{
    m:
    fstream data;

    int arr_product_code[100]; //we are taking a array to hold the code of the various product
    int arr_product_quantity[100]; //we are taking a array to hold the quantity of each product

    string choice;
    int counter = 0 ; //counter
    float before_discount = 0;
    float after_discount = 0;
    float total = 0;

    string input_code;
    string input_quantity;

    cout<<"\t\t\t\t\t\t ___________________________________________________\n";
    cout<<"\t\t\t\t\t\t|                                                   |\n";
    cout<<"\t\t\t\t\t\t|               Available Products                  |\n";
    cout<<"\t\t\t\t\t\t|                                                   |\n";
    cout<<"\t\t\t\t\t\t|___________________________________________________|\n";
    cout<<endl;

    data.open("database.txt" , ios::in); //opening the database file in read mode

    if(!data) //if the file doesn't exists
    {
        cout << "\033[31m";
        PLAY_SOUND("error.wav");
        cout<<"\n\n\t\t\t\t\t\t  Empty database"<<endl;
        cout << "\033[0m";
        return ;
    }
    else
    {
        data.close(); //we will closed the already opened file first

        list(); //if the file exists then we will show its content to the user

        cout<<"\n\t\t\t\t\t\t ___________________________________________________\n";
        cout<<"\n\t\t\t\t\t\t|                                                   |\n";
        cout<<"\n\t\t\t\t\t\t|            Please place your order                |\n";
        cout<<"\n\t\t\t\t\t\t|            For Buyer menu enter (e)               |\n";
        cout<<"\n\t\t\t\t\t\t ___________________________________________________\n";

    //we are going to use do while loop here

        do
        {

            ic://to return if the code invalid(alpha detected)
            cout<<"\n\n\t\t\t\t\t\t  Enter Product Code  : ";
            cin>>input_code; //getting the product code in our array
            exit_buyer(input_code);

            for(int i=0;i<input_code.length();i++){
            if(isalpha(input_code[i])){
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout<<"\t\t\t\t\t\t  Invalid code, Try again \n";
                cout << "\033[0m";
                goto ic;
                }
            }
            arr_product_code[counter]=stoi(input_code);

            iq://to return if the quantity invalid (alpha detected)
            cout<<"\n\n\t\t\t\t\t\t  Enter the product Quantity  : ";
            cin>>input_quantity; //getting the product quantity in our array
            exit_buyer(input_quantity);
            for(int i=0;i<input_quantity.length();i++){
            if(isalpha(input_quantity[i])){
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout<<"\t\t\t\t\t\t  Invalid quantity, Try again \n";
                cout << "\033[0m";
                goto iq;
                }
            }
            arr_product_quantity[counter]=stoi(input_quantity);

            //checking if we ordering duplicate items or not
            for(int i = 0 ; i < counter ; i++)
            {
                if(arr_product_code[counter] == arr_product_code[i])
                {
                    arr_product_quantity[i]+=arr_product_quantity[counter];
                    counter--;
                }
            }
            //check available product
            data.open("database.txt", ios::in);
            bool available = false; // Initialize availability flag once
            // Reset file pointer to the beginning
            data.clear();
            data.seekg(0, ios::beg);
            // Read through the database to check if the product code exists
            while (data >> pcode >> pname >> price >> dis) {
                if (arr_product_code[counter] == pcode) {
                    available = true;
                    break; // No need to continue once the product is found
                }
            }
            data.close();
            if (!available) {
                cout << "\033[31m";
                PLAY_SOUND("error.wav");
                cout << "\n\n\t\t\t\t\t\t  Unavailable product, Try again ";
                cout << "\033[0m";
                goto ic; // Return to productcodeinput
            }

            //if it is not a duplicate product code then we are going to increase the count of product
            counter++;
            c:
            cout<<"\n\n\t\t\t\t\t\t  Do you want to Buy another Product ? If yes then enter 'yes' else 'no' : ";
            cin>>choice;
            exit_buyer(choice);
            if(choice != "n" && choice != "no" && choice != "No" && choice != "N" && choice != "y" && choice != "yes" && choice != "Yes" && choice != "Y"){
            cout << "\033[31m";
            PLAY_SOUND("error.wav");
            cout<<"\t\t\t\t\t\t  Invalid Choice, Try again \n";
            cout << "\033[0m";
            goto c;
            }
        }
        while (choice == "y" || choice == "yes" || choice == "Yes" || choice == "Y"); //if the user wants to buy then we will repeat the loop

        PLAY_SOUND("cash.wav");

        cout<<"\n\n\t\t\t\t\t\t ________________________________________RECEIPT_______________________________________________\n";
        cout<<"\n\t\t\t\t\t\t "<<"Product No. \t Product Name \t Product quantity       Price\t Amount\t   Amount with Discount\n";

        //iterating over the file to get the products and generate the prices and receipt
        data.open("database.txt" , ios::in) ;

        for(int i = 0 ; i < counter ; i++)
        {
            data.clear();
            data.seekg (0,ios::beg) ;  //return the file pointer to the beginning of the file

            data>>pcode>>pname>>price>>dis ;

            while(!data.eof())
            {
                if(pcode == arr_product_code[i]) //checking if the product key matches with any key in the whole file or not
                {
                    before_discount = price * arr_product_quantity[i];
                    after_discount = before_discount - (before_discount * dis/100);
                    total = total + after_discount;
                    cout<<"\n\t\t\t\t\t\t  "<<pcode<<"\t\t "<<fix_width(pname)<<"\t "<<arr_product_quantity[i]<<"\t\t\t"<<price<<"\t "<<before_discount<<"\t   "<<after_discount;
                }
                data>>pcode>>pname>>price>>dis;
            }


      }
      data.close();
    }

    cout<<"\n\n\t\t\t\t\t\t ______________________________________________________________________________________________";
    cout<<"\n\t\t\t\t\t\t Total Amount :  "<<total<<"\n\n";

}

int main()
{
    shopping s;
    s.menu();
    return 0;
}
