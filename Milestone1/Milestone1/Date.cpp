// Final Project Milestone 1 
// Date Module
// File	Date.cpp
// Version 1.0
// Author	Fardad Soleimanloo
// Revision History
// -----------------------------------------------------------
// Name               Date                 Reason
// 
/////////////////////////////////////////////////////////////////
/* Citation and Sources...
Final Project Milestone 1
Module: Date
Filename: Date.cpp
Version 1.0
Author	Fardad Soleimanloo
Revision History
-----------------------------------------------------------
Date      July 13, 2023
2023/13/7  Preliminary release
2023/3/7  Debugged DMA
-----------------------------------------------------------
I have done all the coding by myself and only copied the code
that my professor provided to complete my workshops and assignments.
-----------------------------------------------------------
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <ctime>
using namespace std;
#include "Date.h"
namespace sdds {
    bool Date::validate() {
        errCode(NO_ERROR);
        if (m_year < MIN_YEAR || m_year > m_CUR_YEAR + 1) {
            errCode(YEAR_ERROR);
        }
        else if (m_mon < 1 || m_mon > 12) {
            errCode(MON_ERROR);
        }
        else if (m_day < 1 || m_day > mdays()) {
            errCode(DAY_ERROR);
        }
        return !bad();
    }
    int Date::mdays()const {
        int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
        int mon = m_mon >= 1 && m_mon <= 12 ? m_mon : 13;
        mon--;
        return days[mon] + int((mon == 1) * ((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0));
    }
    int Date::systemYear()const {
        int theYear = sdds_year;
        if (!sdds_test) {
            time_t t = time(NULL);
            tm lt = *localtime(&t);
            theYear = lt.tm_year + 1900;
        }
        return theYear;
    }
    void Date::setToToday() {
        if (sdds_test) {
            m_day = sdds_day;
            m_mon = sdds_mon;
            m_year = sdds_year;
        }
        else {
            time_t t = time(NULL);
            tm lt = *localtime(&t);
            m_day = lt.tm_mday;
            m_mon = lt.tm_mon + 1;
            m_year = lt.tm_year + 1900;
        }
        errCode(NO_ERROR);
    }
    int Date::daysSince0001_1_1()const { // Rata Die day since 0001/01/01 
        int ty = m_year;
        int tm = m_mon;
        if (tm < 3) {
            ty--;
            tm += 12;
        }
        return 365 * ty + ty / 4 - ty / 100 + ty / 400 + (153 * tm - 457) / 5 + m_day - 306;
    }
    Date::Date() :m_CUR_YEAR(systemYear()) {
        setToToday();
    }
    Date::Date(int year, int mon, int day) : m_CUR_YEAR(systemYear()) {
        m_year = year;
        m_mon = mon;
        m_day = day;
        validate();
    }
    const char* Date::dateStatus()const {
        return DATE_ERROR[errCode()];
    }
    int Date::currentYear()const {
        return m_CUR_YEAR;
    }
    void Date::errCode(int readErrorCode) {
        m_ErrorCode = readErrorCode;
    }
    int Date::errCode()const {
        return m_ErrorCode;
    }
    bool Date::bad()const {
        return m_ErrorCode != 0;
    }
    // This function reads a date from the console in the format YYYY/MM/DD
    std::istream& Date::read(std::istream& is)
    {
        m_ErrorCode = NO_ERROR;

        char delimiter;

        is >> m_year >> delimiter >> m_mon >> delimiter >> m_day;

        if (!is)
        {
            m_ErrorCode = CIN_FAILED;
            is.clear();
            /*is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
           // cin.ignore(1000, '\n');
            return is;
        }

        if (delimiter != '/')
        {
            m_ErrorCode = CIN_FAILED;
            // Skip remaining characters on the current line
           /* is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
            cin.ignore(1000, '\n');
            m_ErrorCode = CIN_FAILED;
        }
        else if (m_year < MIN_YEAR || m_year > m_CUR_YEAR + 1)
        {
            m_ErrorCode = YEAR_ERROR;

            // Skip remaining characters on the current line
       /*     is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
            cin.ignore(1000, '\n');
            m_ErrorCode = YEAR_ERROR;
        }

        else if (m_mon < 1 || m_mon > 12)
        {
            m_ErrorCode = MON_ERROR;
            // Skip remaining characters on the current line
       /*     is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
            cin.ignore(1000, '\n');
            m_ErrorCode = MON_ERROR;
        }
        else if (m_day < 1 || m_day > mdays())
        {
            m_ErrorCode = DAY_ERROR;
            // Skip remaining characters on the current line
          /*  is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
            cin.ignore(1000, '\n');
            m_ErrorCode = DAY_ERROR;
        }

        return is;
    }
       
      /* if (is.get() != '/')
        {
            this->m_ErrorCode = CIN_FAILED;
            return is;
        }
        else if (m_year < MIN_YEAR || m_year > m_CUR_YEAR + 1)
        {
            this->m_ErrorCode = YEAR_ERROR;
        }

        is >> this->m_mon;
        if (is.get() != '/')
        {
            this->m_ErrorCode = CIN_FAILED;
            return is;
        }
        else if (m_mon < 1 || m_mon > 12)
        {
            this->m_ErrorCode = MON_ERROR;
        }
        is >> this->m_ErrorCode;
        
        return is;*/

    std::ostream& Date::write(std::ostream& os)const
    {
        if (bad())
        {
            os << dateStatus();
        }
        else
        {
            char oldFill = os.fill();
            os << m_year << "/";

            //display month 
            os.setf(ios::right);
            os.width(2);
            os.fill('0');
            os << m_mon << "/";

            //display day
            os.setf(ios::right);
            os.width(2);
            os.fill('0');
            os << m_day;

            os.fill(oldFill);
            os.unsetf(ios::right);
        }
        return os;
    }
    //Operators
    bool Date::operator==(const Date& other) const
    {
        return this->daysSince0001_1_1() == other.daysSince0001_1_1();
    }
    bool  Date::operator!=(const Date& other) const
    {
        return this->daysSince0001_1_1() != other.daysSince0001_1_1();
    }
    bool  Date::operator>=(const Date& other) const
    {
        return this->daysSince0001_1_1() >= other.daysSince0001_1_1();
    }
    bool  Date::operator<=(const Date& other) const
    {
        return this->daysSince0001_1_1() <= other.daysSince0001_1_1();
    }
    bool  Date::operator<(const Date& other) const
    {
        return this->daysSince0001_1_1() < other.daysSince0001_1_1();
    }
    bool  Date::operator>(const Date& other) const
    {
        return this->daysSince0001_1_1() > other.daysSince0001_1_1();
    }
    int Date::operator-(const Date& other) const
    {
        return this->daysSince0001_1_1() - other.daysSince0001_1_1();
    }

    //bool type conversion operator
    Date::operator bool() const
    {
        if (!bad())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    ostream& operator<<(ostream& os, const Date& RO) {
        return RO.write(os);
    }
    istream& operator>>(istream& is, Date& RO) {
        return RO.read(is);
    }
    // This function reads a date from the console in the format YYYY/MM/DD
 
}