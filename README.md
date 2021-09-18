# SampleCopydata
Sample use of the WM_COPYDATA message.

This is a single-program sample, created in Visual Studio as a Windows Desktop Applicaiton. It needs to be executed twice. The first execution will be the receiver. The second execution will be the sender of a string. Each time the Send button is pressed a string with the current time is sent to the other instance.

The data is sent using:

	// Format the time into a string
	TCHAR timeString[30];
	time_t secs = time(0);
	struct tm local;
	localtime_s(&local, &secs);
	swprintf(timeString, sizeof(timeString), L"%02d:%02d:%02d", local.tm_hour, local.tm_min, local.tm_sec);
	// Send the string
	COPYDATASTRUCT cds{};
	cds.dwData = 1;
	cds.cbData = (wcslen(timeString)+1)*sizeof(TCHAR);
	cds.lpData = (PVOID)timeString;
	SendMessage(OtherhWnd, WM_COPYDATA, NULL, (LPARAM)&cds);

The data is received using:

	COPYDATASTRUCT *pMessageCDS = (PCOPYDATASTRUCT)lParam;
	// first save the string
	pSavedString = (TCHAR*)malloc(pMessageCDS->cbData);
	memcpy_s(pSavedString, pMessageCDS->cbData, pMessageCDS->lpData, pMessageCDS->cbData);
	// now use it now and later
	SetDlgItemTextW(hWnd, IDC_MESSAGE_EDIT, pSavedString);
