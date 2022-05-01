// DateToTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>

static inline bool IsLeapYear(int nYear)
{
    return (nYear % 4 == 0) && ((nYear % 100 == 0) ? (nYear % 400 == 0) : true);
}

int	GetDateTime(const char cszString[], time_t nDefault, time_t* pTime)
{
    int     nResult     = 0;
    int     nRetCode    = 0;
    int     nYear       = 0;
    int     nMonth      = 0;
    int     nDay        = 0;
    int     nHour       = 0;
    int     nMinute     = 0;
    int     nSecond     = 0;
    bool    bLeapYear   = false;
    time_t  nTime       = 0;
    char    szAMOrPM[3];

    assert(cszString);
    assert(pTime);

    nResult = -2;

    nRetCode = sscanf_s(cszString, "%d/%d/%d %d:%d:%d %s", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond, &szAMOrPM, (unsigned int)sizeof(szAMOrPM));
    KG_PROCESS_ERROR(nRetCode >= 6);

    szAMOrPM[sizeof(szAMOrPM) - 1] = '\0';

    if (nRetCode == 7)
    {
        if (stricmp(szAMOrPM, "pm") == 0)
            nHour += 12;
        else
        {
            nRetCode = stricmp(szAMOrPM, "am");
            KG_PROCESS_ERROR(nRetCode == 0);
        }
    }

    KG_PROCESS_ERROR(nYear >= 1970);
    KG_PROCESS_ERROR(nMonth >= 1 && nMonth <= 12);
    KG_PROCESS_ERROR(nDay >= 1 && nDay <= 31);
    KG_PROCESS_ERROR(nHour >= 0 && nHour <= 24);
    KG_PROCESS_ERROR(nMinute >= 0 && nMinute < 60);
    if (nHour == 24)
        KG_PROCESS_ERROR(nMinute == 0 && nSecond == 0);
    KG_PROCESS_ERROR(nSecond >= 0 && nSecond < 60);

    bLeapYear = IsLeapYear(nYear);
    static const int s_cnDaysPerMonth[] = { 31, 29, 31, 30, 31,30, 31, 31, 30, 31, 30, 31 };
    static const int s_cnOneDayTime = 3600 * 24;

    KGLOG_PROCESS_ERROR(nDay <= s_cnDaysPerMonth[nMonth - 1]);
    if (!bLeapYear && nMonth == 2)
        KGLOG_PROCESS_ERROR(nDay < 29);

    for (int nY = 1970; nY < nYear; nY++)
    {
        nTime += s_cnOneDayTime * 365;
        if (IsLeapYear(nY))
            nTime += s_cnOneDayTime;
    }

    for (int nM = 1; nM < nMonth; nM++)
        nTime += s_cnDaysPerMonth[nM - 1] * s_cnOneDayTime;

    if (!bLeapYear && nMonth > 2)
        nTime -= s_cnOneDayTime;

    nTime += (nDay - 1) * s_cnOneDayTime;
    nTime += nHour * 3600 + nMinute * 60 + nSecond;

    nTime -= timezone;
    *pTime = nTime;
    nResult = 1;

Exit0:
    if (nResult != 1)
        *pTime = nDefault;
    return nResult;
}


int main()
{
    int nRetCode = 0;
    const char* szStrings[] = {
        "2022/5/1 7:00:00 AM",
        "2022/6/1 24:00:00",
        "2022/5/1 7:00:00 PM"
    };

    for (auto psz : szStrings)
    { 
        time_t nTime;
        nRetCode = GetDateTime(psz, 0, &nTime);
        KGLOG_PROCESS_ERROR(nRetCode == 1);
        printf("%s -> %lld\n", psz, nTime);
    }
Exit0:
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
