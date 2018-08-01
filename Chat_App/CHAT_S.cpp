#include<iostream>
#include<winsock.h>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<dos.h>

using namespace std;

#define NETWORK_ERROR -1
#define NETWORK_OK 0

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lp,int n)
{
    WSADATA ws;
    int nret,i;
    
    WSAStartup(0x0101,&ws);
    
    SOCKET lsocket;
    
    char servername[50],clientname[50];
    for(i=0;i<50;i++)
    {
        servername[i]=0;
        clientname[i]=0;
    }
    cout<<"Enter Your Name : ";
    gets(servername);
    
    cout<<"\nCreating Socket";
    
    for(int i=0;i<6;i++)
    {
        Sleep(500);
        cout<<".";
    }
    
    lsocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(lsocket==INVALID_SOCKET)
    {
        cout<<"\nCould Not Creat Listening Socket.";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;        
    }
    else
        cout<<"Socket Created";
    
    //Use a SOCKADDR_IN struct to fill addr info
    SOCKADDR_IN serverinfo;
    
    serverinfo.sin_family=AF_INET;
    serverinfo.sin_addr.s_addr=inet_addr("192.168.0.20");
    serverinfo.sin_port=htons(2609);
    
    //Bind the socket to our local server addr
    cout<<"\nBinding Socket";
    
    for(i=0;i<10;i++)
    {
        Sleep(500);
        cout<<".";
    }


    nret=bind(lsocket,(SOCKADDR *)&serverinfo,sizeof(struct sockaddr));
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nCould Not Bind Listening Socket";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    else
        cout<<"Socket Bound";
    
    //Make the bound socket listen.Up to 10 ma wait at any one time
    cout<<"\nPutting Bound Socket to Listening Mode";
    
    for(i=0;i<8;i++)
    {
        Sleep(500);
        cout<<".";
    }
    

    nret=listen(lsocket,10);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nCould Not Attain Listen Mode";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    else
        cout<<"Entered Listening Mode";
    //Wait for client
    cout<<"\nWaiting for Client .......";
    
    SOCKET commsocket;
    commsocket=accept(lsocket,NULL,NULL);
    if(commsocket==INVALID_SOCKET)
    {
        cout<<"\nCould Not Setup Connection On Request";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    else
    {
        cout<<"\nConnection Setup Successfull\n";
    }
    
    nret=send(commsocket,servername,50,0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nConnection Error. Exiting";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    nret=recv(commsocket,clientname,50,0);
    if(nret==SOCKET_ERROR)
    {
        cout<<"\nImproper commection.Exiting";
        WSACleanup();
        getchar();
        return NETWORK_ERROR;
    }
    
    char sendbuffer[256]="\nThis is server,reporting on port 2609";
    char recvbuffer[256];
    while(1)
    {
        for(i=0;i<255;i++)
        {
            sendbuffer[i]=0;
            recvbuffer[i]=0;
        }
        cout<<"\n"<<servername<<" :";
        gets(sendbuffer);
        if(strcmpi(sendbuffer,"BYE")==0)
        {
            cout<<"\n\nSwitching off the chat and exiting";
            WSACleanup();
            getchar();
            break;
        }
        else
        {
            nret=send(commsocket,sendbuffer,255,0);
            if(nret==SOCKET_ERROR)
            {
                cout<<"\nUnable to send.Please Retry";
                WSACleanup();
                getchar();
            }
            else
            {
                nret=recv(commsocket,recvbuffer,255,0);
                if(nret==SOCKET_ERROR)
                {
                    cout<<"\nCould Not Hear Client";
                    WSACleanup();
                    getchar();
                }
                else
                {
                    cout<<"\n"<<clientname<<" :"<<recvbuffer;
                }
            }
        }
    }
    cout<<"\n\nThank You For Using PG's Application.";
    getchar();
    return NETWORK_OK;
}
