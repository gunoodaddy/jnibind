typedef struct tagSampleData
{
	int nID;
	bool bFlag;
	GTUserStatusType eStatus;
	GTCHAR strUserID[256];      /* comment test */
	char *ptr;
	// TEST
	std::string string1;
	/* TEST */
	string string2;
	// TEST
	char strCustomData[MAX_CUSTOM_DATA_SIZE];
	/* TEST */
	float testFloat;      
	size_t nCustomDataSize;
	bool bMobileUsageFlag;
	DWORD dwProfileImageCRC;
}SAMPLE_DATA;



