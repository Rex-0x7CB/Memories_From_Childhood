#include<iostream>
#include<winsock.h>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<fstream>
#include<dos.h>
#include<setjmp.h>

#define NETWORK_ERROR -1
#define NETWORK_OK 0

using namespace std;

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
        void del();
        void transadd(float);
        void transred(float);
        char * ret_acc_no();
        void show_details();
        int ret_stat();
        char * ret_acc_pass();
};

//Global data variable declaration
SOCKET commsocket,lsocket;
int nret,i;
char IP[16];
jmp_buf MAYDAY;
int First_Open=1;

//Globle function prototype declaration
void PG(void);
void login(void);
void create(void);
void logout(void);
void transfer(void);
int bind_socket(void);
void show(void);
void unbind_sock(void);

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lp,int n)      //WinMain Function to 
                                                                                    //initialise the program
{
    int choice=0;
    PG();
    while(1)
    {
        setjmp(MAYDAY);                                                             //Jump location set
        lsocket=bind_socket();
        while(1)
        {
            choice=0;
            fflush(stdin);  
            cout<<"\nWaiting for the choice to come ";
            nret=recv(lsocket,(char *)&choice,sizeof(choice),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                WSACleanup();
                unbind_sock();
                longjmp(MAYDAY,1);
            }
            switch(choice)
            {
                case 1:
                    cout<<"\nChoice:"<<choice;
                    login();
                    cout<<"\nlogin() ended";
                    break;
                case 2:
                    cout<<"\nChoice:"<<choice;
                    create();
                    cout<<"\ncreate() ended";
                    break;
                case 3:
                    cout<<"\nChoice:"<<choice;
                    logout();
                    break;
                case 4:
                    cout<<"\nChoice:"<<choice;
                    transfer();
                    break;
                default :
                    cout<<"\nCHOICE :"<<choice;
                    cout<<"\nUser has entered wrong choice";
                    break;
            }   
            if(choice==6)
            {
                unbind_sock();
                break;
            } 
        }
    }
}

char * account::ret_acc_pass()                                                      //function ti return password
                                                                                    //of the user
{
    return pass;
}
char * account::ret_acc_no()                                                        //function to return account
                                                                                    //number of the user
{
    return acc_no;
}

int account::ret_stat()                                                             //function to return active
                                                                                    //state of user
{
    return active_stat;
}
void account::transadd(float a)                                                     //function to add money to 
                                                                                    //the account of the user
{
    money=money+a;
}

void account::transred(float a)                                                     //function to deduct money 
                                                                                    //from the user
{
    money=money-a;
}
void account :: show_details()                                                      //function to show current 
                                                                                    //details of user
{
    fflush(stdin);
    cout<<endl;
    cout<<"\nName Of Account Holder : "<<holder_name;
    cout<<"\nAccount Number         : "<<acc_no;
    cout<<"\nAssigned E-Mail ID     : "<<email;
    cout<<"\nAssigned Address       : "<<address;
    cout<<"\nBalance Available      : "<<money;
    cout<<"\nActive Stats           : "<<active_stat;
}

void PG()                                                                           //function to show startup screen
{
    system("CLS");
    char start[]={"A PROGRAM BY MASTER PRASHANT GUPTA"};
    int i=0;
    cout<<"\n\n\t\t\t\t";
    
    for(i=0;i<=8;i++)
    {
        cout<<start[i];
        Sleep(50);
    }
    Sleep(500);
    
    cout<<"\n\n\n\n\t\t\t\t  ";
    for(i;i<=12;i++)
    {
        cout<<start[i];
        Sleep(50);
    }
    
    cout<<"\n\n\n\n\t\t\t\t ";
    for(i;i<=19;i++)
    {
        cout<<start[i];
        Sleep(50);
    }
    
    cout<<"\n\n\n\n\t\t\t     ";
    for(i;i<=33;i++)
    {
        cout<<start[i];
        Sleep(200);
    }
    Sleep(2000);
    system("CLS");
}

void unbind_sock()                                                                  //function to unbind socket
{
    closesocket(commsocket);
    closesocket(lsocket);
    cout<<"\nALL WENT HOLY.";
} 

int bind_socket()                                                                   //function to bind the socket to 
                                                                                    //statr communication between 
                                                                                    //connected computers
{
    WSADATA ws;
    WSAStartup(0x0101,&ws);
    
    cout<<"\nCreating Socket";
        
    for(i=0;i<6;i++)
    {
        Sleep(500);
        cout<<".";
    }

    commsocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(commsocket==INVALID_SOCKET)
    {
        cout<<"\nCould Not Create Socket.";
        WSACleanup();
        return NETWORK_ERROR;        
    }
    else
    {
        cout<<"SUCCESSFUL";
    }
    
    //Use a SOCKADDR_IN struct to fill addr info
    SOCKADDR_IN serverinfo;
    
    serverinfo.sin_family=AF_INET;
    if(First_Open==1)
    {
        for(i=0;i<=15;i++)
        {
            IP[i]=0;
        }
        cout<<"\nEnter the IP Address of server : ";
        gets(IP);
        First_Open=0;
    }
    serverinfo.sin_addr.s_addr=inet_addr(IP);
    
    //htonl(INADDR_ANY);
    serverinfo.sin_port=htons(2609);
    
    //Bind the socket to our local server addr
    cout<<"\nBinding Socket";
    
    for(i=0;i<10;i++)
    {
        Sleep(500);
        cout<<".";
    }

    nret=bind(commsocket,(SOCKADDR *)&serverinfo,sizeof(struct sockaddr));
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nCould Not Bind Listening Socket";
        WSACleanup();
        return NETWORK_ERROR;
    }
    else
        cout<<"SUCCESSFUL";
    
    //Make the bound socket listen.Up to 10 ma wait at any one time
    cout<<"\nInitializing Listening Mode";
        
    for(i=0;i<8;i++)
    {
        Sleep(500);
        cout<<".";
    }

    nret=listen(commsocket,10);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nCould Not Attain Listen Mode";
        WSACleanup();
        return NETWORK_ERROR;
    }
    else
        cout<<"SUCCESSFULL";
    
    //Wait for client
    cout<<"\nWaiting for Client .......";
    
    lsocket=accept(commsocket,NULL,NULL);
    if(lsocket==INVALID_SOCKET)
    {
        cout<<"\nCould Not Setup Connection On Request";
        WSACleanup();
        return NETWORK_ERROR;
    }
    else
    {
        cout<<"\nConnection Setup Successfull\n";
    }
    Sleep(3000);
    return lsocket;
}

void login()                                                                        //function to make the user loged
                                                                                    //in only on provinding authentified
                                                                                    //username and password
{
    cout<<"\nReached login()";
    fflush(stdin);
    struct log
    {
        char acc_number[50];
        char password[50];
    };
    
    log u;
    account a;
    int result=0;
    cout<<"\nAbout to receive";
    nret=recv(lsocket,(char*)&u,sizeof(u),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    cout<<"\nRecieved...";
    cout<<"Acc_no: "<<u.acc_number;
    cout<<"Password: "<<u.password;
    
    ifstream f;
    f.open("BANK.PG");
    if(!f)
    {
        cout<<"File cannot be opened ";
    }
    while(1)
    {
        f.read((char*)&a,sizeof(a));
        
        if(strcmp(a.ret_acc_no(),u.acc_number)==0 && strcmp(a.ret_acc_pass(),u.password)==0)
        {
            
            result=1;
            cout<<"Match found.Details are...";
            nret=send(lsocket,(char*)&result,sizeof(result),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                WSACleanup();
                unbind_sock();
                longjmp(MAYDAY,1);
            }
     
            a.show_details();
     
            nret=send(lsocket,(char*)&a,sizeof(a),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                WSACleanup();
                unbind_sock();
                longjmp(MAYDAY,1);
            }
            return;
        }
        if(f.eof())
            break;
    }
    result=0;
    cout<<"\nMatch Not Found.";
    nret=send(lsocket,(char*)&result,sizeof(result),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    //Now control will return to main();
}
        
void logout()                                                                       //function to log the user out
{
    account a;                                                                      // a will receive the information 
                                                                                    // about the client account which 
                                                                                    //is sent by cient 
                                                                                    
    account b;                                                                      // b will be used to write the 
                                                                                    //content of BANK.PG to BANK_TEMP.PG
    
    int success=1;                                                                  // success is send to client to 
                                                                                    //confirm file data has been 
                                                                                    //written successfully for reference 
                                                                                    //of success please see the end of
                                                                                    //function showmenu() in client 
                                                                                    //program
    nret=recv(lsocket,(char*)&a,sizeof(a),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    a.show_details();
    cout<<"\na is received";
    ifstream f1;
    f1.open("BANK.PG",ios::binary);
    if(!f1)
    {
        cout<<"\nReading File cannot be opened";
    }
    else
    {
        cout<<"\nReading File opened";
    }
    ofstream f2;
    f2.open("BANK_TEMP.PG",ios::binary);
    if(!f2)
    {
        cout<<"\nWriting File cannot be opened";
    }
    else
    {
        cout<<"\nWriting File opened";
    }
    while(1)
    {
        f1.read((char*)&b,sizeof(b));
        if(f1.eof())
        {
            cout<<"\nEOF Reached";
            break;
        }
        else
        {
            cout<<"\nFile still left.";
        }
        if((strcmp(b.ret_acc_no(),a.ret_acc_no())==0))
        {
            cout<<"\nMatch found.Data skiped.";                                     //if the account no of received 
                                                                                    //account matches with any account 
                                                                                    //no of file then the object is not
                                                                                    //be written in file because the new 
                                                                                    //data of the same account in a 
                                                                                    //will be written at last
        }
        else
        {
            cout<<"\nCase do not match.Data is written.";
            f2.write((char*)&b,sizeof(b));
        }
        cout<<"\nCheaking EOF";
        
    }
    if(a.ret_stat()==1)
    {
        f2.write((char*)&a,sizeof(a));
    }
    f1.close();
    f2.close();
    cout<<"\nFile has been successfully closed down.";
    remove("BANK.PG");
    cout<<"\nBank has been deleted";
    rename("BANK_TEMP.PG","BANK.PG");
    cout<<"\nBank_temp has been renamed."; 
    nret=send(lsocket,(char*)&success,sizeof(success),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    cout<<"\nLogout() module is now closing.";
}

void create()                                                                       //function to create new account 
                                                                                    //for the user on incoming request
{
    cout<<"\nReached create";
    fflush(stdin);
    account new_acc,temp;
    int create_stat;
    ifstream f1;
    ofstream f2;
    cout<<"\nAbout to recv";
    nret=recv(lsocket,(char*)&new_acc,sizeof(new_acc),0);  
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    new_acc.show_details();
    
    f1.open("BANK.PG",ios::binary);
    if(!f1)
    {
        cout<<"\nInput file cannot be opened.";
    }
    
    f2.open("BANK.PG",ios::binary|ios::app);
    if(!f2)
    {
        cout<<"\nOutput file cannot be opened.";
    }
    while(1)
    {
        f1.read((char*)&temp,sizeof(temp));
        {
            if(strcmp(new_acc.ret_acc_no(),temp.ret_acc_no())==0)
            {
                create_stat=0;
                nret=send(lsocket,(char*)&create_stat,sizeof(create_stat),0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                    WSACleanup();
                    unbind_sock();
                    longjmp(MAYDAY,1);
                }
                return;
            }
            if(f1.eof())
            {
                cout<<"\nFile End Reached";
                break;
            }
        }
    }
    
    f2.write((char*)&new_acc,sizeof(new_acc));
    cout<<"\nAccount Has Been Created.";
    create_stat=1;
    nret=send(lsocket,(char*)&create_stat,sizeof(create_stat),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
}

void transfer()                                                                     //function to transfer money from
                                                                                    //one user to another user
{
    ifstream f1;
    ofstream f2;
    account my,rcvr;
    int result,condition=0;                                                         //Whenever the f1.eof is true 
                                                                                    //for rcvr,condition will tell 
                                                                                    //server whether rcvr accno was 
                                                                                    //found or not and hence it will 
                                                                                    //decide to send result=0 or result=1
    f1.open("BANK.PG",ios::binary);
    if(!f1)
    {
        cout<<"\nReading file cannot be opened.";
    }
    
    f2.open("BANK_TEMP.PG",ios::binary);
    if(!f2)
    {
        cout<<"\nWriting file cannot be opened.";
    }
    
    struct t
    {
        char myaccno[50];
        char rcvraccno[50];
        float money_trans;
    }t;
    
    recv(lsocket,(char*)&t,sizeof(t),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
    while(1)
    {
        f1.read((char*)&rcvr,sizeof(rcvr));
        cout<<"\n\nData Read";
        cout<<"\nCheaking EOF";
        if(f1.eof())
        {
            cout<<"\nEOF Reached";
            if(condition==0)                                                        //condition 0 implies rcvr 
                                                                                    //was not found and file is finished
            {
                result=0;
                cout<<"Sending Result"<<result;
                nret=send(lsocket,(char*)&result,sizeof(result),0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
                    WSACleanup();
                    unbind_sock();
                    longjmp(MAYDAY,1);
                }
                f1.close();
                f2.close();
                remove("BANK_TEMP.PG");
                cout<<"\nTemp has been deleted.";
                return;
            }
            f1.close();
            f2.close();
            cout<<"\nFile has been successfully closed down.";
            remove("BANK.PG");
            cout<<"\nBank has been deleted";
            rename("BANK_TEMP.PG","BANK.PG");
            cout<<"\nBank_temp has been renamed.";
            break;
        }
        else
        {
            cout<<"\nFile still left.";
        }
        cout<<"\n\nRead Data is:"<<endl;
        rcvr.show_details();
        if((strcmp(rcvr.ret_acc_no(),t.rcvraccno)==0))
        {
            cout<<"\nReceiver Match Found.";
            rcvr.show_details();
            cout<<"\nCalling Function Add Money.";
            rcvr.transadd(t.money_trans);
            cout<<"\nThe changed details are : ";
            rcvr.show_details();
            f2.write((char*)&rcvr,sizeof(rcvr));
            condition=1;                                                            //condition 1 implies rcvr has 
                                                                                    //been found
        }
        else
        {
            cout<<"\nCase do not match.Data is written.";
            f2.write((char*)&rcvr,sizeof(rcvr));
        }
    }
        
    f1.open("BANK.PG",ios::binary);
    if(!f1)
    {
        cout<<"\nReading file cannot be opened.";
    }
    
    f2.open("BANK_TEMP.PG",ios::binary);
    if(!f2)
    {
        cout<<"\nWriting file cannot be opened.";
    }
    
    while(1)
    {
        f1.read((char*)&my,sizeof(my));
        if(f1.eof())
        {
            cout<<"\nEOF Reached";
            break;
        }
        else
        {
            cout<<"\nFile still left.";
        }
        if((strcmp(my.ret_acc_no(),t.myaccno)==0))
        {
            cout<<"\nUser Match found.";
            my.show_details();
            my.transred(t.money_trans);
            cout<<"\nThe changed details are : ";
            my.show_details();
            f2.write((char*)&my,sizeof(my));
        }
        else
        {
            cout<<"\nCase do not match.Data is written.";
            f2.write((char*)&my,sizeof(my));
        }
        cout<<"\nCheaking EOF";
        
    }
    
    f1.close();
    f2.close();
    cout<<"\nFile has been successfully closed down.";
    remove("BANK.PG");
    cout<<"\nBank has been deleted";
    rename("BANK_TEMP.PG","BANK.PG");
    cout<<"\nBank_temp has been renamed."; 
    result=1;
    cout<<"Sending Result :"<<result;
    nret=send(lsocket,(char*)&result,sizeof(result),0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nNetwork Connection Error.Socket Is Now UnBound";
        WSACleanup();
        unbind_sock();
        longjmp(MAYDAY,1);
    }
}
