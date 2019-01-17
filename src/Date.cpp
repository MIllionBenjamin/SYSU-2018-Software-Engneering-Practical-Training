//
//  Date.cpp
//  AgendaXcode
//
//  Created by MillionBenjamin on 2018/10/19.
//  Copyright © 2018 中山大学. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Date.hpp"
using namespace std;

Date::Date()
{
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}
/**
 * @brief constructor with arguments
 */
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
{
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
}
/**
 * @brief constructor with a string
 */
Date::Date(const std::string &dateString)
{
    if(dateString.length() != 16)
    {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            m_hour = 0;
            m_minute = 0;
            return;
    }
    for(int i = 0; i < 16; i++)
    {
        if(i == 4 && dateString[i] != '-')
        {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            m_hour = 0;
            m_minute = 0;
            return;
        }
        else if(i == 7 && dateString[i] != '-')
        {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            m_hour = 0;
            m_minute = 0;
            return;
        }
        else if(i == 10 && dateString[i] != '/')
        {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            m_hour = 0;
            m_minute = 0;
            return;
        }
        else if(i == 13 && dateString[i] != ':')
        {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            m_hour = 0;
            m_minute = 0;
            return;
        }
        else if( i != 4 && i != 7 && i != 10 && i != 13 && (dateString[i] < '0' || dateString[i] > '9'))
        {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            m_hour = 0;
            m_minute = 0;
            return;
        }
    }
    string s_year = dateString.substr(0, 4);
    string s_month = dateString.substr(5, 2);
    string s_day = dateString.substr(8, 2);
    string s_hour = dateString.substr(11, 2);
    string s_minute = dateString.substr(14, 2);
    m_year = stoi(s_year);
    m_month = stoi(s_month);
    m_day = stoi(s_day);
    m_hour = stoi(s_hour);
    m_minute = stoi(s_minute);
    if (isValid(*this) == 0)
    {
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_hour = 0;
        m_minute = 0;
    }
}
/**
 * @brief return the year of a Date
 * @return   a integer indicate the year of a date
 */
int Date::getYear(void) const
{
    return m_year;
}

/**
 * @brief set the year of a date
 * @param a integer indicate the new year of a date
 */
void Date::setYear(const int t_year)
{
    m_year = t_year;
}

/**
 * @brief return the month of a Date
 * @return   a integer indicate the month of a date
 */
int Date::getMonth(void) const
{
    return m_month;
}
/**
 * @brief set the month of a date
 * @param a integer indicate the new month of a date
 */
void Date::setMonth(const int t_month)
{
    m_month = t_month;
}

/**
 * @brief return the day of a Date
 * @return   a integer indicate the day of a date
 */
int Date::getDay(void) const
{
    return m_day;
}

/**
 * @brief set the day of a date
 * @param a integer indicate the new day of a date
 */
void Date::setDay(const int t_day)
{
    m_day = t_day;
}

/**
 * @brief return the hour of a Date
 * @return   a integer indicate the hour of a date
 */
int Date::getHour(void) const
{
    return m_hour;
}

/**
 * @brief set the hour of a date
 * @param a integer indicate the new hour of a date
 */
void Date::setHour(const int t_hour)
{
    m_hour = t_hour;
};

/**
 * @brief return the minute of a Date
 * @return   a integer indicate the minute of a date
 */
int Date::getMinute(void) const
{
    return m_minute;
};

/**
 * @brief set the minute of a date
 * @param a integer indicate the new minute of a date
 */
void Date::setMinute(const int t_minute)
{
    m_minute = t_minute;
};

/**
 *   @brief check whether the date is valid or not
 *   @return the bool indicate valid or not
 */
bool Date::isValid(const Date &t_date)
{
    if (t_date.getYear() > 9999 || t_date.getYear() < 1000)
        return false;
    if (t_date.getMonth() > 12 || t_date.getMonth() < 1)
        return false;
    switch (t_date.getMonth())
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(t_date.getDay() > 31 || t_date.getDay() < 1)
                return false;
            break;
        case 2:
            if(((t_date.getYear()) % 100) == 0)
            {
                if(t_date.getYear() % 400 == 0)
                {
                    if(t_date.getDay() > 29 || t_date.getDay() < 1)
                        return false;
                }
                else
                {
                    if(t_date.getDay() > 28 || t_date.getDay() < 1)
                        return false;
                }
            }
            else if((t_date.getYear()) % 4 == 0)
            {
                if(t_date.getDay() > 29 || t_date.getDay() < 1)
                        return false;
            }
            else
            {
                if(t_date.getDay() > 28 || t_date.getDay() < 1)
                        return false;
            }
        case 4:
        case 6:
        case 9:
        case 11:
            if(t_date.getDay() > 30 || t_date.getDay() < 1)
                return false;
                break;
        default:
            break;
    }
    if (t_date.getHour() > 23 || t_date.getHour() < 0)
        return false;
    else if (t_date.getMinute() > 59 || (t_date.getMinute() < 0))
        return false;
    return true;
};

/**
 * @brief convert a string to date, if the format is not correct return
 * 0000-00-00/00:00
 * @return a date
 */
Date Date::stringToDate(const std::string &t_dateString)
{
    Date deft(0, 0, 0, 0, 0);
    if (t_dateString.length() != 16)
        return deft;
    else
    {
        Date ret(t_dateString);
        if (isValid(ret) == 0)
            return deft;
        else
            return ret;
    }
};

/**
 * @brief convert a date to string, if the format is not correct return
 * 0000-00-00/00:00
 */
std::string Date::dateToString(const Date &t_date)
{
    if(isValid(t_date))
    {
        stringstream dS;
        dS << setw(4) << setfill('0') << t_date.getYear()
                            << "-" << setw(2) << setfill('0') << t_date.getMonth()
                            << "-" << setw(2) << setfill('0') << t_date.getDay()
                            << "/" << setw(2) << setfill('0') << t_date.getHour()
                            << ":" << setw(2) << setfill('0') << t_date.getMinute();
        string dateString = dS.str();
        return dateString;
    }
    else
        return "0000-00-00/00:00";
};

/**
 *  @brief overload the assign operator
 */
Date & Date::operator=(const Date &t_date)
{
    Date ret(0, 0, 0, 0, 0);
    ret.m_year = t_date.m_year;
    ret.m_month = t_date.m_month;
    ret.m_day = t_date.m_day;
    ret.m_hour = t_date.m_hour;
    ret.m_minute = t_date.m_minute;
    this->setYear(t_date.m_year);
    this->setMonth(t_date.m_month);
    this->setDay(t_date.m_day);
    this->setHour(t_date.m_hour);
    this->setMinute(t_date.m_minute);
    return *this;
};

/**
 * @brief check whether the CurrentDate is equal to the t_date
 */
bool Date::operator==(const Date &t_date) const
{
    if (m_year == t_date.m_year &&
        m_month == t_date.m_month &&
        m_day == t_date.m_day &&
        m_hour == t_date.m_hour &&
        m_minute == t_date.m_minute)
        return true;
    else
        return false;
};

/**
 * @brief check whether the CurrentDate is  greater than the t_date
 */
bool Date::operator>(const Date &t_date) const
{
    if(m_year > t_date.m_year)
        return true;
    else if (m_month > t_date.m_month)
        return true;
    else if (m_day > t_date.m_day)
        return true;
    else if (m_hour > t_date.m_hour)
        return true;
    else if (m_minute > t_date.m_minute)
        return true;
    else
        return false;
};

/**
 * @brief check whether the CurrentDate is  less than the t_date
 */
bool Date::operator<(const Date &t_date) const
{
    if(m_year < t_date.m_year)
        return true;
    else if (m_month < t_date.m_month)
        return true;
    else if (m_day < t_date.m_day)
        return true;
    else if (m_hour < t_date.m_hour)
        return true;
    else if (m_minute < t_date.m_minute)
        return true;
    else
        return false;
};

/**
 * @brief check whether the CurrentDate is  greater or equal than the t_date
 */
bool Date::operator>=(const Date &t_date) const
{
    if(m_year > t_date.m_year)
        return true;
    else if (m_month > t_date.m_month)
        return true;
    else if (m_day > t_date.m_day)
        return true;
    else if (m_hour > t_date.m_hour)
        return true;
    else if (m_minute > t_date.m_minute)
        return true;
    else if (m_year == t_date.m_year &&
             m_month == t_date.m_month &&
             m_day == t_date.m_day &&
             m_hour == t_date.m_hour &&
             m_minute == t_date.m_minute)
        return true;
    else
        return false;
};

/**
 * @brief check whether the CurrentDate is  less than or equal to the t_date
 */
bool Date::operator<=(const Date &t_date) const
{
    if(m_year < t_date.m_year)
        return true;
    else if (m_month < t_date.m_month)
        return true;
    else if (m_day < t_date.m_day)
        return true;
    else if (m_hour < t_date.m_hour)
        return true;
    else if (m_minute < t_date.m_minute)
        return true;
    else if (m_year == t_date.m_year &&
             m_month == t_date.m_month &&
             m_day == t_date.m_day &&
             m_hour == t_date.m_hour &&
             m_minute == t_date.m_minute)
        return true;
    else
        return false;
};
