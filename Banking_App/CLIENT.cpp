#include<iostream>
#include<winsock.h>
#include<cstdio>
#include<string>
#include<setjmp.h>
#include<windows.h>
#include<ctime>
#include<cctype>

#define NETWORK_ERROR -1
#define NETWORK_OK 0

using namespace std;

//Global variable Declaration
SOCKET commsocket;
char sendbuffer[500],recvbuffer[500];
int p,i,nret;
jmp_buf MAYDAY;
time_t mytime;
struct tm* timeinfo;
char IP[16];
int First_Open=1;

class account
{
    private:
        char acc_no[50];                                                            //Used for account number to log in 
        char pass[50];
        char holder_name[50];                                                       //Name of account holder
        char email[50];
        char address[50];
        float money;
        int active_stat;                                                            //If active_stat is 1 then account
                                                                                    //is active if active_stat is 0 
                                                                                    //then account will be closed 
    
    public:
        char * ret_hname();
        char * ret_acc_no();
        void showmenu();
        void show_details();
        void withdraw();
        void deposit();
        void transfer();
        void edit();
        void del();
        void enter_details();
};

struct log                                                                          //Structure containg username and 
                                                                                    //password to varify a successfull
                                                                                    //login of user
{
    char acc_number[50];
    char password[50];
};

char * account::ret_acc_no()                                                        //function to return account number
                                                                                    //of the user
{
    return acc_no;
}

char * account :: ret_hname()                                                       //function to return holder name 
                                                                                    //of the user
{
    return holder_name;
}

void unbind_sock()                                                                  //function to unbind socket
{
    closesocket(commsocket);
    cout<<"\nALL WENT HOLY.";
}

void gotoxy(int x, int y)                                                           //function to place curser at 
                                                                                    //any point on the screen
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void show_time()                                                                    //function to show the current date
                                                                                    //and time of the client computer
{
    time(&mytime);
    timeinfo=localtime(&mytime);
    gotoxy(55,1);
    cout<<"DATE AND TIME : ";
    gotoxy(55,2);
    cout<<asctime(timeinfo);
} 
    
void textcolor(int text_color = 7,int paper_color = 0)                              //function to set the fore ground and
                                                                                    //background colour of the screen
{
	int color_total = (text_color+(paper_color*16));
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color_total );
}

void account :: show_details()                                                      //function to show details of the
                                                                                    //logged in user
{
    fflush(stdin);
    system("CLS");
    int i;
    char search[]="Searching Database...";
    char retr[]="Retriving Data List...";
    char acno[]="ACCOUNT NUMBER : ";
    char name[]="NAME           : ";
    char mail[]="MAIL           : ";
    char addr[]="ADDRESS        : ";
    char amnt[]="AMOUNT         : Rs ";
    
    time(&mytime);
    timeinfo=localtime(&mytime);
    gotoxy(0,1);
    cout<<holder_name;
    gotoxy(55,1);
    cout<<"DATE AND TIME : ";
    gotoxy(55,2);
    cout<<asctime(timeinfo);
    system("COLOR 20");
    gotoxy(0,4);
    for(i=0;i<strlen(search);i++)
    {
        cout<<search[i];
        Sleep(30);
    }
    
    gotoxy(0,6);
    for(i=0;i<strlen(retr);i++)
    {
        cout<<retr[i];
        Sleep(30);
    }
    
    for(i=0;i<80;i++)
    {
        gotoxy(i,8);
        cout<<(char)219;
        Sleep(25);
    }
    
    gotoxy(0,10);
    for(i=0;i<strlen(acno);i++)
    {
        cout<<acno[i];
        Sleep(30);
    }
    for(i=0;i<strlen(acc_no);i++)
    {
        cout<<acc_no[i];
        Sleep(30);
    }
    
    gotoxy(0,12);
    for(i=0;i<strlen(name);i++)
    {
        cout<<name[i];
        Sleep(30);
    }
    for(i=0;i<strlen(holder_name);i++)
    {
        cout<<holder_name[i];
        Sleep(30);
    }
    
    gotoxy(0,14);
    for(i=0;i<strlen(mail);i++)
    {
        cout<<mail[i];
        Sleep(30);
    }
    for(i=0;i<strlen(email);i++)
    {
        cout<<email[i];
        Sleep(30);
    }
    
    gotoxy(0,16);
    for(i=0;i<strlen(addr);i++)
    {
        cout<<addr[i];
        Sleep(30);
    }
    for(i=0;i<strlen(address);i++)
    {
        cout<<address[i];
        Sleep(30);
    }
    
    gotoxy(0,18);
    for(i=0;i<strlen(amnt);i++)
    {
        cout<<amnt[i];
        Sleep(30);
    }
    cout<<money;
    gotoxy(56,22);
    cout<<"Press Enter to continue";
    getchar();
    gotoxy(79,22);
    for(i=79;i>=56;i--)
    {
        gotoxy(i,22);
        cout<<" ";
        Sleep(30);
    }
    
    gotoxy(25,18);
    for(i=25;i>=0;i--)
    {
        gotoxy(i,18);
        cout<<" ";
        Sleep(30);
    }
    
    gotoxy(30,16);
    for(i=30;i>=0;i--)
    {
        gotoxy(i,16);
        cout<<" ";
        Sleep(30);
    }
    gotoxy(50,14);
    for(i=50;i>=0;i--)
    {
        gotoxy(i,14);
        cout<<" ";
        Sleep(30);
    }
    gotoxy(50,12);
    for(i=50;i>=0;i--)
    {
        gotoxy(i,12);
        cout<<" ";
        Sleep(30);
    }
    gotoxy(50,10);
    for(i=50;i>=0;i--)
    {
        gotoxy(i,10);
        cout<<" ";
        Sleep(30);
    }
}

void account :: withdraw()                                                          //function that will tell the server
                                                                                    //to deduct money from the account
{
    float withd=-1;
    while(withd<0 || money-1000<withd)
    {
        fflush(stdin);
        system("CLS");
        system("COLOR 1F");
        show_time();
        cout<<"\n\nTotal Amount Available             : Rs "<<money;
        cout<<"\nMinimum amount to be sustained     : Rs 1000";
        cout<<"\nEnter the amount to be withdrawn   : ";
        cin>>withd;
        if(withd<0 || money-1000<withd)
        {
            cout<<"\n\nTransaction Not Possible.Please Try Again "; 
            fflush(stdin);
            getchar();
            system("CLS");
        }
    }
    money=money-withd;
    cout<<"\nMoney Successfully withdrew. ";
    cout<<"\nCurrent Balance : "<<money;
    fflush(stdin);
    getchar();
    
}
    
void account :: deposit()                                                           //function that will tell the server
                                                                                    //to add money to the account 
{
    fflush(stdin);
    float depot=-1;
    while(depot<0)
    {
        system("CLS");
        show_time();
        cout<<"\n\nTotal Amount Available             : Rs "<<money;
        cout<<"\nEnter the amount to be deposited   : Rs ";
        cin>>depot;
        if(depot<0)
        {
            cout<<"\n\nTransaction Not Possible.Please Try Again "; 
            fflush(stdin);
            getchar();
            system("CLS");
        }
    }
    money = money + depot;
    cout<<"\nMoney Successfully Deposited. ";
    cout<<"\nCurrent Balance : Rs "<<money;
    fflush(stdin);
    getchar();
}

void account :: transfer()                                                          //function that will tell the server
                                                                                    //to exchange money between the users
{
    fflush(stdin);
    system("CLS");
    system("COLOR 1F");
    int result;
    struct transaction
    {
        char myaccno[50];
        char rcvraccno[50];
        float money_trans;
    }t;
    float trans=-1;
    while(trans<0 || money-1000<trans)
    {
        system("CLS");
        show_time();
        cout<<"\n\nTotal Amount Available             : Rs "<<money;
        cout<<"\nMinimum amount to be sustained     : Rs 1000";
        cout<<"\nEnter the amount to be tranfered   : Rs ";
        cin>>trans;
        if(trans<0 || money-1000<trans)
        {
            cout<<"\n\nTransaction Not Possible.Please Try Again ";
            fflush(stdin); 
            getchar();
            system("CLS");
        }
    }
    
    fflush(stdin);
    cout<<"\nEnter the account number to which money has to be transfered : ";
    gets(t.rcvraccno);
    strcpy(t.myaccno,acc_no);
    t.money_trans=trans;
    
    nret=send(commsocket,(char*)&t,sizeof(t),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    
    nret=recv(commsocket,(char*)&result,sizeof(result),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    
    if(result==0)
    {
        cout<<"\nWrong account number entered.";
        fflush(stdin);
        getchar();
        return;
    }
    
    if(result==1)
    {
        money=money-trans;
        cout<<"Money has been successfully transfered.";
        fflush(stdin);
        getchar();
    }
    cout<<"\nCurrent Balance : "<<money;
    fflush(stdin);
    getchar();
}

void account :: edit()                                                              //function that allows user to edit
                                                                                    //the present details of user
{
    fflush(stdin);
    system("CLS");
    system("COLOR 1F");
    show_time();
    cout<<"\n\nEnter name of account holder : ";
    gets(holder_name);
    cout<<"\nEnter e-mail id               : ";
    gets(email);
    cout<<"\nEnter address                 : ";
    gets(address);
    cout<<"\n\nData Successfully Edited.\nPlease Log Out And Login Again To Apply Changes.";
    fflush(stdin);
    getchar();
}

void account :: del()                                                               //function that changes the active 
                                                                                    //state of the user acccount to 
                                                                                    //deactivate the account
{
    fflush(stdin);
    system("COLOR 1F");
    char del_stat;
    while(del_stat!='Y' && del_stat!='y' && del_stat!='N' && del_stat!='n')
    {
        system("CLS");
        show_time();
        cout<<"\n\nAre you sure u want to permenently delete your account (Y/N) :";
        del_stat=getchar();
        if(del_stat!='Y' && del_stat!='y' && del_stat!='N' && del_stat!='n')
        {
            cout<<"Invalid Input.Please Provide Valid Input";
            fflush(stdin);
            getchar();
        }
    }
    
    if(del_stat == 'Y' || del_stat == 'y')
    {
        cout<<"\nYour account will be de-activated within 24 hours.Please log out your account.";
        cout<<"\nRs "<<money<<" has been successfully withdrew ";
        money=0;
        active_stat=0;
        fflush(stdin);
        getchar();
    }
}    

void account :: enter_details()                                                     //function to take input of details
                                                                                    //of user to create the account
{
    srand(time(NULL));
    char pass1[50];
    int i,ran;
    system("CLS");
    show_time();
    system("COLOR 1F");
    fflush(stdin);                                      //to flush keyboard input buffer,present in cstdio
    while(1)
    {
        cout<<"\n\nEnter name of account holder  : ";
        gets(holder_name);
        if(strlen(holder_name)>0)
            break;
        else
        {
            cout<<"\nEntry Cannot be left blank.\nRe-enter the Holder Name.";
        }
    }
    cout<<"\nEnter e-mail id               : ";
    gets(email);
    cout<<"\nEnter address                 : ";
    gets(address);
    for(i=0;i<49;i++)
    {
        acc_no[i]=0;
    }
    for(i=0;i<10;i++)
    {
        ran = rand() % 10;
        acc_no[i]='0' + ran;
    }
    
    while(1)
    {
        
        cout<<"\nEnter your password           : ";
        gets(pass);
        cout<<"\nConfirm password              : ";
        gets(pass1);
        if(strcmp(pass,pass1)==0)
            break;
        else
        {
            cout<<"\nPassword do not match.\nRe-enter the password.";
        }
    }
    
    while(1)
    {
        cout<<"\nEnter the initial amount of money to be deposited : Rs ";
        cin>>money;
        if(money>=1000)
            break;
        else
        {
            cout<<"\nMinimum amount of money should be Rs 1000.Please re-enter the amount.";
            fflush(stdin);
            getchar();
        }
    }
    active_stat=1;
    getchar();
}

void account :: showmenu()                                                          //function to show user menu
{
    fflush(stdin);
    int logout=0,choice=8;
    int indication=0;                                                               //indication is an integer which is
                                                                                    //send to server to indicate that
                                                                                    //user wanna log out,so
                                                                                    //prepare yourself
                                                                                    
                                                                                    //it will be received by server in
                                                                                    //main function of it in the integer
                                                                                    //choice ie case 3: logout();
    while(logout==0)
    {
        choice=8;
        system("CLS");
        system("COLOR 1F");
        show_time();
        char wel[]="WELCOME";
        while(choice<1 || choice>7)
        {
            textcolor(15,9);
            gotoxy((40-(strlen(wel)/2)-1),4);
            
            for(i=0;i<=strlen(wel)+1;i++)
            {
                cout<<" ";
                Sleep(50);
            }
            textcolor(15,9);
            gotoxy((40-(strlen(wel)/2)),4);
            
            for(i=0;i<=strlen(wel);i++)
            {
                cout<<wel[i];
                Sleep(100);
            }
            gotoxy((40-(strlen(holder_name)/2)-1),6);
            
            for(i=0;i<=strlen(holder_name)+1;i++)
            {
                cout<<" ";
                Sleep(50);
            }
            textcolor(15,9);
            gotoxy((40-(strlen(holder_name)/2)),6);
            
            for(i=0;i<=strlen(holder_name);i++)
            {
                if(islower(holder_name[i]))
                {
                    cout<<(char)(holder_name[i]-('a'-'A'));
                }
                else
                {
                    cout<<holder_name[i];
                }
                Sleep(100);
            }
            system("COLOR 1F");
            
            cout<<"\n\nPlease Proceed with following options :-";
            cout<<"\n\n1.Check My Details";
            cout<<"\n2.Wihdraw Money";
            cout<<"\n3.Deposite Money";
            cout<<"\n4.Transfer Money";
            cout<<"\n5.Edit My Details";
            cout<<"\n6.Delete My Account";
            cout<<"\n7.Log Out My Account";
            cout<<"\n\nEnter your choice : ";
            textcolor(15,9);
            gotoxy((40-(strlen(wel)/2)-1),4);
            
            for(i=0;i<=strlen(wel)+1;i++)
            {
                cout<<" ";
            }
            textcolor(15,9);
            gotoxy((40-(strlen(wel)/2)),4);
            for(i=0;i<=strlen(wel);i++)
            {
                cout<<wel[i];
            }
            gotoxy((40-(strlen(holder_name)/2)-1),6);
            for(i=0;i<=strlen(holder_name)+1;i++)
            {
                cout<<" ";
            }
            textcolor(15,9);
            gotoxy((40-(strlen(holder_name)/2)),6);
            for(i=0;i<=strlen(holder_name);i++)
            {
                if(islower(holder_name[i]))
                {
                    cout<<(char)(holder_name[i]-('a'-'A'));
                }
                else
                {
                    cout<<holder_name[i];
                }
            }
            gotoxy(21,18);
            cin>>choice;
            if(choice<1 || choice>7)
            {
                cout<<"\nInvalid Choice Input.Please Enter Valid Input";
                fflush(stdin);
                getchar();
                system("CLS");
            }
        }
        
        switch(choice)
        {
            case 1:
                show_details();
                break;
            
            case 2:
                withdraw();
                break;
            
            case 3:
                deposit();
                break;
            
            case 4:
                indication=4;
                nret=send(commsocket,(char*)&indication,sizeof(indication),0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                    WSACleanup();
                    unbind_sock();
                    longjmp(MAYDAY,1);
                }
                transfer();
                cout<<"\nTransfer Ended";
                break;
            
            case 5:
                edit();
                break;
            
            case 6:
                del();
                break;
            
            case 7:
                indication=3;
                nret=send(commsocket,(char*)&indication,sizeof(indication),0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                    WSACleanup();
                    unbind_sock();
                    longjmp(MAYDAY,1);
                }
                nret=send(commsocket,(char*)(this),sizeof(account),0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                    WSACleanup();
                    unbind_sock();
                    longjmp(MAYDAY,1);
                }
                nret=recv(commsocket,(char *)&logout,sizeof(logout),0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                    WSACleanup();
                    unbind_sock();
                    longjmp(MAYDAY,1);
                }
                if(logout==1)
                {
                    cout<<"\nLogged Out Successfully";
                    logout=1;
                    getchar();
                    break;
                }
                else 
                {
                    cout<<"Server Error.Log Out Failed.Try Again.";
                    fflush(stdin);
                    getchar();
                }
        }
    }
}    

void login_disp(log u,int result)                                                   //function to display login success
                                                                                    //or failure
{
   system("CLS");
   int i,j,t;
   textcolor(2,0);
   char user[]="ACCOUNT NUMBER : ";
   char pass[]="PASS           : ";
   char dot[]="..........";
   char log0[]="Incorrect Username Or Password";
   char log1[]="Logged In Successfully";
   system("CLS");
   Sleep(200);
   textcolor(10);
   system("CLS");
   textcolor(2);
   cout<<"\n\t\t      L O G G I N G   I N   A C C O U N T";
   
   for(i=15;i<65;i++)
   {
       gotoxy(i,5);
	   cout<<(char)223<<(char)223;
	   Sleep(25);
   }
   
   for(j=5;j<=22;j++)
   {
	   gotoxy(66,j);
	   cout<<(char)221;
	   Sleep(25);
   }
   
   for(i=65;i>=15;i--)
   {
	   gotoxy(i,23);
	   cout<<(char)223<<(char)223;
	   Sleep(25);
   }
   
   for(j=22;j>=5;j--)
   {
	   gotoxy(15,j);
	   cout<<(char)222;
	   Sleep(25);
   }
   
   gotoxy(20,8);
   cout<<"ACCOUNT NUMBER : ";
   for(t=0;t<strlen(u.acc_number);t++)
   {
	   cout<<u.acc_number[t];
	   Sleep(100);
   }
   gotoxy(20,10);
   cout<<"PASSWORD : ";
   
   for(t=0;t<strlen(u.password);t++)
   {
	   cout<<'*';
	   Sleep(100);
   }
   gotoxy(20,14);
   cout<<"SIGNING IN";
   gotoxy(30,14);
   
   for(t=0;t<=9;t++)
   {
	   cout<<dot[t];
	   Sleep(200);
   }
   
   if(result==1)
   {
       gotoxy(30,18);
       for(t=0;t<strlen(log1);t++)
       {
    	   cout<<log1[t];
    	   Sleep(50);
       }
   }
   
   if(result==0)
   {
       gotoxy(27,18); 
       for(t=0;t<strlen(log0);t++)
       {
    	   cout<<log0[t];
    	   Sleep(50);
       }
   }
   
   gotoxy(34,20);
   cout<<"Press Enter Key To Continue...";
   fflush(stdin);
   getchar();
}

void user_log(account ac)                                                           //function to display loading screen
{
    system("CLS");
    int i;
    char name[50];
    char myapp[]="PG's ONLINE BANKING SYSTEM";
    char load_det[]="LOADING YOUR DETAILS TO BUFFER";
    char load_line[]="______________________";
    
    strcpy(name,ac.ret_hname());
    textcolor(15,0);
    gotoxy((40-(strlen(myapp)/2)),10);
    
    for(i=0;i<strlen(myapp);i++)
    {
        cout<<myapp[i];
        Sleep(50);
    }
    textcolor(15,9);
    gotoxy((40-(strlen(name)/2)-1),12);
    
    for(i=0;i<=strlen(name)+1;i++)
    {
        cout<<" ";
        Sleep(50);
    }
    textcolor(15,9);
    gotoxy((40-(strlen(name)/2)),12);
    
    for(i=0;i<=strlen(name);i++)
    {
        if(islower(name[i]))
        {
            cout<<(char)(name[i]-('a'-'A'));
        }
        else
        {
            cout<<name[i];
        }
        Sleep(100);
    }
    
    gotoxy((40-(strlen(load_det)/2)),14);
    textcolor(10,0);
    
    for(i=0;i<=strlen(load_det);i++)
    {
        cout<<load_det[i];
        Sleep(50);
    }
    
    textcolor(3,0);
    gotoxy((40-(strlen(load_line)/2)),15);
    cout<<load_line;
    gotoxy((40-(strlen(load_line)/2)),15);
    textcolor(15,0);
    
    for(i=0;i<=strlen(load_line);i++)
    {
        cout<<load_line[i];
        Sleep(300);
    }
    gotoxy(80-50,23);
    cout<<"Buffering Completed.Press Enter Key To Continue...";
    getchar();
}

void login()                                                                        //function to check login success or 
                                                                                    //login failure
{
    fflush(stdin);
    log u;
    account ac,acc;
    int result=-2;
    system("CLS");
    system("COLOR 1F");
    show_time();
    cout<<"\n\nEnter account number : ";
    gets(u.acc_number);
    cout<<"\nEnter password : ";
    gets(u.password);
    nret=send(commsocket,(char*)&u,sizeof(u),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    nret=recv(commsocket,(char *)&result,sizeof(result),0);                         //result check weather account 
                                                                                    //logged in successfully(ie 1) 
                                                                                    //or not(ie 0) 
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    login_disp(u,result);
    if(result==1)
    {
        nret=recv(commsocket,(char*)&ac,sizeof(ac),0);                              //ac is the object containing 
                                                                                    //all the information about the 
                                                                                    //account of user 
        if(nret==SOCKET_ERROR)
        {
            cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
            WSACleanup();
            unbind_sock();
            longjmp(MAYDAY,1);
        }
        user_log(ac);
        ac.showmenu();
    }
}

void create()                                                                       //function to deploy a function 
                                                                                    //to take user data and then tell the
                                                                                    //server to create the account
{
    fflush(stdin);
    account new_acc;
    int create_stat=-1;
    char a='N';
    system("COLOR 1F");
    do
    {
        
        new_acc.enter_details();
        fflush(stdin);
        OOPS :
        cout<<"\nPlease Confirm These Details : ";
        cout<<"\nTo Confirm Press Y Or To Exit Press N ";
        cout<<"\n\nChoice : ";
        a=getchar();
        if(a!='Y' && a!='y' && a!='n' && a!='N')
        {
            cout<<"\nWrong Choice Entered "; 
            getchar();
            goto OOPS;
        }
        else if(a!='Y' && a!='y')
        {
            cout<<"\nAccount Has Not Been Created.Please Re Enter Details.";
            fflush(stdin);
            getchar();
        }
    }while(a!='Y' && a!='y');
    
    nret=send(commsocket,(char*)&new_acc,sizeof(new_acc),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    
    nret=recv(commsocket,(char*)&create_stat,4,0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    
    if(create_stat==0)
    {
        cout<<"\nNetwork Malfunctioning.\nCannot create account.\nPage has expired.\nPlease try again";
        fflush(stdin);
        getchar();
    }
    
    else if(create_stat==1)
    {
        cout<<"\nAccount Successfully Created.";
        cout<<"\nYour Account Number : "<<new_acc.ret_acc_no();
        fflush(stdin);
        getchar();
    }
}

void about()                                                                        //function to tell user a little
                                                                                    //about this application
{
    fflush(stdin);
    system("CLS");
    system("COLOR 1F");
    show_time();
    cout<<"\n\nA Banking System Application By PG.";
    cout<<"\nThanks to all the people who are directly \nor indirectly invloved in the project.";
    cout<<"\nSpecial Thanks to following sites :";
    cout<<"\n1. www.MadWizard.com";
    cout<<"\n2. www.HackForums.net";
    cout<<"\n3. www.CPlusPlus.com";
    cout<<"\nSpecial Thanks to following games to help me in making User Interface";
    cout<<"\n1. Need For Speed Most Wanted ";
    cout<<"\n2. Modern Warfare";
    cout<<"\nMeet me on facebook by inviting friend prashantgupta2609@gmail.com";
    fflush(stdin);
    getchar();
    return;
}

void menu()                                                                         //function to display main menu for 
                                                                                    //the user
{
    fflush(stdin);
    int choice=6,temp;                                                              //since choice=4 in server indicates
                                                                                    //transfer function.Therefore some 
                                                                                    //other indication is required to
                                                                                    //indicate that user exits the 
                                                                                    //program.Hence temp is used 
    while(1)
    {
        system("CLS");
        choice=6;
        fflush(stdin);
        int i;
        char welcome[]="WELCOME TO PG's ONLINE BANKING APP"; 
        char log[]="1.Login Account";
        char creat[]="2.Create Account";
        char app[]="3.About Application";
        char exit[]="4.Exit Program";
        system("COLOR 10");
        
        while(choice>4 ||choice<1)
        {
        system("CLS");
        textcolor(15,1);
        show_time();
        choice=6;
        fflush(stdin);
        textcolor(0,15);
        gotoxy((40-(strlen(welcome)/2)-1),5);
        
        for(i=0;i<=strlen(welcome)+1;i++)
        {
            cout<<" ";
            Sleep(30);
        }
        textcolor(0,15);
        gotoxy((40-(strlen(welcome)/2)),5);
        
        for(i=0;i<=strlen(welcome);i++)
        {
            cout<<welcome[i];
            Sleep(40);
        }
        textcolor(15,1);
        gotoxy(0,8);
        
        for(i=0;i<=strlen(log);i++)
        {
            cout<<log[i];
            Sleep(20);
        }
        gotoxy(0,9);
        
        for(i=0;i<=strlen(creat);i++)
        {
            cout<<creat[i];
            Sleep(20);
        }
        gotoxy(0,10);
        
        for(i=0;i<=strlen(app);i++)
        {
            cout<<app[i];
            Sleep(20);
        }
        gotoxy(0,11);
        
        for(i=0;i<=strlen(exit);i++)
        {
            cout<<exit[i];
            Sleep(20);
        }
        cout<<"\n\nEnter your choice : ";
        gotoxy(21,13);
        cin>>choice;
        if(choice>4 ||choice<1)
        {
            cout<<"\nInvalid Choice Input.Please Enter Valid Input";
            fflush(stdin);
            getchar();
            system("CLS");
        }
    }            
    switch(choice)
    {
        case 1:
            nret=send(commsocket,(char*)&choice,sizeof(choice),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                WSACleanup();
                unbind_sock();
                longjmp(MAYDAY,1);
            }
            login();
            break;
        
        case 2:
            nret=send(commsocket,(char*)&choice,sizeof(choice),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                WSACleanup();
                unbind_sock();
                longjmp(MAYDAY,1);
            }
            create();
            break;
        
        case 3:
            about();
            break;
        
        case 4:
            temp=6;
            nret=send(commsocket,(char*)&temp,sizeof(temp),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                WSACleanup();
                unbind_sock();
                longjmp(MAYDAY,1);
            }
            cout<<"\n\nThank you for using PG's Application.";
            MessageBox(0,"THANK YOU FOR USING MY APPLICATION.","PG's BANKING APP",0);
            fflush(stdin);
            getchar();
            return;
        }
    }
}

int bind_socket()                                                                   //function to create and 
                                                                                    //bind the socket
{
    WSADATA ws;
    WSAStartup(0x1010,&ws);
       
    cout<<"\nCreating Socket.....";
    commsocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(commsocket==INVALID_SOCKET)
    {
        cout<<"\nSocket Cannot be created.....";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    else
        cout<<"Socket successfully created";
    
    SOCKADDR_IN serverinfo;
    serverinfo.sin_family=AF_INET;
    if(First_Open==1)
    {
        for(p=0;p<=16;p++)
            IP[p]=0;
        cout<<"\nEnter the IP Address of server : ";
        gets(IP);
        First_Open=0;
    }
    serverinfo.sin_addr.s_addr=inet_addr(IP);
    serverinfo.sin_port=htons(2609);
    
    cout<<"\nConnecting to server program......";
    nret=connect(commsocket,(LPSOCKADDR)&serverinfo,sizeof(struct sockaddr));
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nCannot connect to server......";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    else
        cout<<"Connected to server successfully\n";
    return NETWORK_OK;
}
int WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lp,int n)             //WinMain function to initialise the 
                                                                                    //program 
{
    setjmp(MAYDAY);
    bind_socket();
    menu();
}
