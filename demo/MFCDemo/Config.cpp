#include "Config.h"
#include "StdAfx.h"

CConfig::CConfig(char* fileName)
{
	if (fileName)
	{
		strcpy_s(m_fileName,128,fileName);
		GetPrivateProfileString("server","ip","127.0.0.1",m_serverIP,128,m_fileName);
		GetPrivateProfileString("server","username","admin",m_userName,128,m_fileName);
		GetPrivateProfileString("server","password","123456",m_userPasswd,128,m_fileName);
		m_port = GetPrivateProfileInt("server","port",9701,m_fileName);
	}
	else
	{
		strcpy_s(m_serverIP,128,"127.0.0.1");
		strcpy_s(m_userName,32,"admin");
		strcpy_s(m_userPasswd,32,"123456");
		m_port = 9701;
		sprintf_s(m_fileName,128,"config");
		SaveServer(m_serverIP,m_port,m_userName,m_userPasswd);
	}
}


CConfig::~CConfig(void)
{
}

int  CConfig::SaveServer(char* m_IP, int port, char* name, char* passwd)
{
	if (m_fileName[0] != '\0')
	{
		char  sport[12];
		sprintf_s(sport,12,"%d",port);
		WritePrivateProfileString("server","ip",m_IP,m_fileName);
		WritePrivateProfileString("server","port",sport,m_fileName);
		WritePrivateProfileString("server", "username", name, m_fileName);
		WritePrivateProfileString("server", "password", passwd, m_fileName);
		return 0;
	}
	return -1;
}