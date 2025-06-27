#include "SysInfo.h"

const string SysInfo::author = "Lucas Gabriel, Lucas SaaD, Gabriel Gaudio, João Yokoyama";
const string SysInfo::date = "2025, June";
const string SysInfo::institution = "Universidade Estadual de Campinas (Unicamp)";
const string SysInfo::systemName = "FT_coin C++";
const string SysInfo::version = "1.0";
const string SysInfo::helpFile = "help.txt";

const string& SysInfo::getAuthor()
	{
	return author;
	}

const string& SysInfo::getDate()
	{
	return date;
	}

const string& SysInfo::getInstitution()
	{
	return institution;
	}

const string& SysInfo::getSystemName()
	{
	return systemName;
	}

const string& SysInfo::getVersion()
	{
	return version;
	}

const string& SysInfo::getHelpFile()
	{
	return helpFile;
	}

const string SysInfo::getFullVersion()
	{
	return systemName + " | Ver. " + version + " (" + date + ")";
	}
