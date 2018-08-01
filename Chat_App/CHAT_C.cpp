#include<iostream>
#include<winsock.h>
#include<cstdio>
#include<string>
#include<windows.h>
using namespace std;

#define NETWORK_ERROR -1
#define NETWORK_OK 0

void ReportError(int,const char *);

int _stdcall WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lp,int n)
{
    WSADATA ws;
    int nret,i;
    
    WSAStartup(0x1010,&ws);
    SOCKET commsocket;
    
    char clientname[50],servername[50];
    
    for(i=0;i<50;i++)
        clientname[i]=0;
        
    cout<<"Enter Your name : ";
    gets(clientname);
     
    cout<<"\nCreating Socket";
    for(i=0;i<8;i++)
    {
        Sleep(500);
        cout<<'.';
    }
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
    serverinfo.sin_addr.s_addr=inet_addr("192.168.0.20");
    serverinfo.sin_port=htons(2609);
    
    cout<<"\nConnecting to server program";
    for(i=0;i<10;i++)
    {
        Sleep(500);
        cout<<'.';
    }
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
    
    nret=recv(commsocket,servername,50,0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nImproper connection.Exiting ";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    
    nret=send(commsocket,clientname,50,0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"Cannot setup connection Properly.Exiting";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    char sendbuffer[256]="Client: Hello How you ? ";
    char recvbuffer[256];
    while(1)
    {
        for(int i=0;i<=255;i++)
        {
            sendbuffer[i]=0;
            recvbuffer[i]=0;
        }
        cout<<"\n"<<clientname<<" :";
        gets(sendbuffer);
        if(strcmpi(sendbuffer,"BYE")==0)
        {
            cout<<"\n\nClosing chat application";
            WSACleanup();
            getchar();
            break;
        }
        else
        {
            nret=send(commsocket,sendbuffer,strlen(sendbuffer),0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nCannot send data to server.Please Retry";
                WSACleanup();
                getchar();
            }
            else
            {
                nret=recv(commsocket,recvbuffer,255,0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nCannot receive data from server.......";
                    WSACleanup();
                    getchar();
                }
                else
                {
                    cout<<"\n"<<servername<<" :"<<recvbuffer;
                }
            }
        }
    }
    cout<<"\n\nThanks for using PG's Chat Application";
    closesocket(commsocket);
    getchar();
    return NETWORK_OK;
}
