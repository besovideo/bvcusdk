#pragma once

class CConfig
{
private:
    char  m_fileName[128];

    char  m_serverIP[128];
    int   m_port;
    char  m_userName[32];
    char  m_userPasswd[32];
public:
    CConfig(char* fileName);
    ~CConfig(void);

    int   SaveServer(char* m_IP, int port, char* name, char* passwd);

    char* GetServerIP(){ return m_serverIP; }
    char* GetUserName(){ return m_userName; }
    char* GetPassword(){ return m_userPasswd; }
    int   GetPort()    { return m_port; }
};

