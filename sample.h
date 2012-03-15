typedef struct tagGT_BUDDY_DATA                                                                                                                               
{                                                                                                                                                             
	int nID;                                                                                                                                                  
	int nGroupID;                                                                                                                                             
	bool bBuddy;                                                                                                                                              
	GTUserStatusType eStatus;                                                                                                                                 
	GTOSType eOSType;                                                                                                                                         
	BYTE nType;                                                                                                                                               
	GTCHAR strUserID[256];      /* For Local DB Proxy Feature, MUST BE Static Character Array.  (Also equivalent to the following) */
	GTCHAR strName[256]; 	// CPP COMMENT TEST                                                                                                                                     
	GTCHAR strAlias[256];                                                                                                                                     
	GTCHAR strTalkName[256];                                                                                                                                  
	char *ptr;
	GTPSTR ptr1;
	// TEST
	std::string string1;
	/* TEST */
	string string2;
	// TEST
	char strCustomData[MAX_CUSTOM_DATA_SIZE];                                                                                                                 
	char strCustomData1[MAX_CUSTOM_DATA_SIZE1];                                                                                                                 
	/* TEST */
	float testFloat;
	size_t nCustomDataSize;                                                                                                                                   
	bool bMobileUsageFlag;                                                                                                                                    
	DWORD dwProfileImageCRC;                                                                                                                                  
	double aaa;
	char bbb;
}GT_BUDDY_DATA;

