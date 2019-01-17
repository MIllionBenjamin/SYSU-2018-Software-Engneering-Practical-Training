#include <iostream>
#include <fstream>
#include <sstream>
#include "Storage.hpp"
#include "Path.hpp"

using namespace std;

std::shared_ptr<Storage> Storage::m_instance = NULL;

Storage::Storage()
{
    m_dirty = false;
    this -> readFromFile();
}


/**
 *   read file content into memory
 *   @return if success, true will be returned
 */
bool Storage::readFromFile(void)
{
    ifstream readUser(Path::userPath);
    ifstream readMeeting(Path::meetingPath);
    m_userList.clear();
    m_meetingList.clear();
    if(!readUser || !readMeeting)
        return false;
    else
    {
        string everyUline;
        int i = 0;
        int j = 0;
        
        while(getline(readUser, everyUline))
        {
            i = 0;
            stringstream EULine(everyUline);
            string userInfo[4];
            
            while(getline(EULine, userInfo[i], ','))
            {
                i++;
            }
            for(j = 0; j < 4 ;j++)
            {
                userInfo[j] = userInfo[j].substr(1, userInfo[j].length() - 2);
            }
            User u1(userInfo[0], userInfo[1], userInfo[2], userInfo[3]);
            m_userList.push_back(u1);
        }
        
        string everyMLine;
        
        
        while(getline(readMeeting, everyMLine))
        {
            
            i = 0;
            stringstream EMLine(everyMLine);
            string meetingInfo[5];
            while(getline(EMLine, meetingInfo[i], ','))
            {
                i++;
            }
            for(j = 0; j < 5; j++)
            {
                meetingInfo[j] = meetingInfo[j].substr(1, meetingInfo[j].length() - 2);
            }
            stringstream EPName(meetingInfo[1]);
            string everyPName;
            vector<string> participartorName;
            while(getline(EPName, everyPName, '&'))
            {
                participartorName.push_back(everyPName);
            }
            Date startDate(meetingInfo[2]);
            Date endDate(meetingInfo[3]);
            Meeting meet1(meetingInfo[0], participartorName, startDate, endDate, meetingInfo[4]);
            m_meetingList.push_back(meet1);
        }
    }
    
    return true;
};

/**
 *   write file content from memory
 *   @return if success, true will be returned
 */
bool Storage::writeToFile(void)
{
    if(m_dirty = false)
        return false;
    ofstream users;
    ofstream meetings;
    if(!users || !meetings)
        return false;
    users.open(Path::userPath);
    meetings.open(Path::meetingPath);
    for(auto const& i : m_userList)
    {
        users << "\"" << i.getName() << "\"" << "," << "\"" << i.getPassword() << "\"" << "," << "\"" << i.getEmail() << "\"" << "," << "\"" << i.getPhone() << "\"" << endl;
    }
    for(auto const& j : m_meetingList)
    {
        meetings << "\"" << j.getSponsor() << "\"" << "," << "\"";
        string parters;
        vector<string> allParters = j.getParticipator();
        for(auto const& k : allParters)
        {
            parters += k;
            parters += "&";
        }
        parters = parters.substr(0, parters.length() - 1);
        meetings << parters << "\"" << "," << "\"" << Date::dateToString(j.getStartDate()) << "\"" << "," << "\"" << Date::dateToString(j.getEndDate()) << "\"" << "," << "\"" << j.getTitle() << "\"" << endl;
    }
    users.close();
    meetings.close();
    m_dirty = false;
    return true;
};


/**
 * get Instance of storage
 * @return the pointer of the instance
 */
std::shared_ptr<Storage> Storage::getInstance(void)
{
    if(m_instance == NULL)
        m_instance = shared_ptr<Storage>(new Storage);
    return m_instance;
};

/**
 *   destructor
 */
Storage::~Storage()
{
    if(m_dirty == true)
        writeToFile();
};

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
 * create a user
 * @param a user object
 */
void Storage::createUser(const User &t_user)
{
    m_userList.push_back(t_user);
    m_dirty = true;
};

/**
 * query users
 * @param a lambda function as the filter
 * @return a list of fitted users
 */
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
{
    list<User> QUser;
    for(auto const& i : m_userList)
    {
        if(filter(i))
            QUser.push_back(i);
        else
            continue;
    }
    return QUser;
};

/**
 * update users
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the user
 * @return the number of updated users
 */
int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher)
{
    int updateNum = 0;
    for(auto & i : m_userList)
    {
        if(filter(i))
        {
            switcher(i);
            updateNum++;
            m_dirty = true;
        }
    }
    return updateNum;
};

/**
 * delete users
 * @param a lambda function as the filter
 * @return the number of deleted users
 */
int Storage::deleteUser(std::function<bool(const User &)> filter)
{
    int deleteNum = 0;
    for(auto it = m_userList.begin(); it != m_userList.end();)
    {
        if(filter(*it))
        {
            it = m_userList.erase(it);
            deleteNum++;
            m_dirty = true;
        }
        else
            it++;
    }
    return deleteNum;
};

/**
 * create a meeting
 * @param a meeting object
 */
void Storage::createMeeting(const Meeting &t_meeting)
{
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
};

/**
 * query meetings
 * @param a lambda function as the filter
 * @return a list of fitted meetings
 */
std::list<Meeting> Storage::queryMeeting(
                                         std::function<bool(const Meeting &)> filter) const
{
    list<Meeting> QMeeting;
    for(auto const& i : m_meetingList)
    {
        if(filter(i))
            QMeeting.push_back(i);
        else
            continue;
    }
    return QMeeting;
};

/**
 * update meetings
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the meeting
 * @return the number of updated meetings
 */
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                           std::function<void(Meeting &)> switcher)
{
    int updateNum = 0;
    for(auto & i : m_meetingList)
    {
        if(filter(i))
        {
            switcher(i);
            updateNum++;
            m_dirty = true;
        }
        else
            continue;
    }
    return updateNum;
}
/**
 * delete meetings
 * @param a lambda function as the filter
 * @return the number of deleted meetings
 */
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
{
    int deleteNum = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end();)
    {
        if(filter(*it))
        {
            it = m_meetingList.erase(it);
            deleteNum++;
            m_dirty = true;
        }
        else
            it++;
    }
    return deleteNum;
};

/**
 * sync with the file
 */
bool Storage::sync(void)
{
    return writeToFile();
};



