/*	File			TCP_Demonstrator_General.h
	Purpose			Deffinitions used by both CLIENT and SERVER
	Author			Richard Anthony	(ar26@gre.ac.uk)
	Date			December 2003
*/

#pragma once

#define iPORT 8000  // Decimal
#define MAX_MESSAGE_LENGTH 100
#define MAX_USER_LENGTH 10

struct Message_PDU {
	char szText[MAX_MESSAGE_LENGTH + 1];
	int mType;
	int intValue;
};



