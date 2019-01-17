#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "AgendaUI.hpp"
using namespace std;

AgendaUI::AgendaUI() :m_userName(""), m_userPassword(""), m_agendaService(AgendaService()) {}

void AgendaUI::startAgenda()
{
     
}

void AgendaUI::OperationLoop() {
    while (true) {
        for (int i = 0; i <= 40; i++) cout << "-";
        cout << "Agenda";
        for (int i = 0; i <= 40; i++) cout << "-";
        cout << endl;
        cout << "Action :" << endl;
        if (m_userName == "") {
            cout << "l    - log in Agenda by user name and password" << endl;
            cout << "r    - register an Agenda account" << endl;
            cout << "q    - quit Agenda" << endl;
        }
        else {
            cout << "o    - log out Agenda" << endl;
            cout << "dc   - delete Agenda account" << endl;
            cout << "lu   - list all Agenda user" << endl;
            cout << "cm   - create a meeting" << endl;
            cout << "la   - list all meetings" << endl;
            cout << "las  - list all sponsor meetings" << endl;
            cout << "lap  - list all participator meetings" << endl;
            cout << "qm   - query meeting by title" << endl;
            cout << "qt   - query meeting by time interval" << endl;
            cout << "dm   - delete meeting by title" << endl;
            cout << "da   - delete all meetings" << endl;
            cout << "amp  - add meeting participator" << endl;
            cout << "rmp  - remove meeting participator" << endl;
            cout << "rqm  - quit meeting" << endl;
        }
        for (int i = 0; i < 80; i++) cout << "-";
        cout << endl << endl;
        if (m_userName == "") cout << "Agenda :~$ ";
        else cout << "Agenda@" << m_userName << " :# ";
        if (!executeOperation(getOperation())) break;
    }
}

string AgendaUI::getOperation() {
    string op;
    cin >> op;
    return op;
}

bool AgendaUI::executeOperation(string t_operation) {
    if (t_operation == "l" && m_userName == "") {
        userLogIn();
        return true;
    }
    if (t_operation == "r" && m_userName == "") {
        userRegister();
        return true;
    }
    if (t_operation == "q" && m_userName == "") {
        quitAgenda();
        return false;
    }
    if (t_operation == "dc" && m_userName != "") {
        deleteUser();
        return true;
    }
    if (t_operation == "lu" && m_userName != "") {
        listAllUsers();
        return true;
    }
    if (t_operation == "cm" && m_userName != "") {
        createMeeting();
        return true;
    }
    if (t_operation == "la" && m_userName != "") {
        listAllMeetings();
        return true;
    }
    if (t_operation == "las" && m_userName != "") {
        listAllSponsorMeetings();
        return true;
    }
    if (t_operation == "lap" && m_userName != "") {
        listAllParticipateMeetings();
        return true;
    }
    if (t_operation == "qm" && m_userName != "") {
        queryMeetingByTitle();
        return true;
    }
    if (t_operation == "qt" && m_userName != "") {
        queryMeetingByTimeInterval();
        return true;
    }
    if (t_operation == "dm" && m_userName != "") {
        deleteMeetingByTitle();
        return true;
    }
    if (t_operation == "da" && m_userName != "") {
        deleteAllMeetings();
        return true;
    }
    if (t_operation == "o" && m_userName != "") {
        userLogOut();
        return true;
    }
    if (t_operation == "amp" && m_userName != "") {
        addMeetingParticipator();
        return true;
    }
    if (t_operation == "rmp" && m_userName != "") {
        removeMeetingParticipator();
        return true;
    }
    if (t_operation == "rqm" && m_userName != "") {
        quitMeeting();
        return true;
    }
    cout << endl;
    cout << "Please enter correct command!" << endl;
    cout << endl;
    return true;
}

void AgendaUI::userLogIn() {
    cout << endl;
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    cin >> m_userName >> m_userPassword;
    if (m_agendaService.userLogIn(m_userName, m_userPassword)) {
        cout << "[log in] succeed!" << endl;
    }
    else {
        cout << "[log in] Password error or user doesn't exist" << endl;
        m_userName = "";
        m_userPassword = "";
    }
    cout << endl;
}

void AgendaUI::userRegister() {
    cout << endl;
    cout << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string userName, password, email, phone;
    cin >> userName >> password >> email >> phone;
    if (m_agendaService.userRegister(userName, password, email, phone)) {
        cout << "[register] succeed!" << endl;
    }
    else {
        cout << "[register] This username has been registered!" << endl;
    }
    cout << endl;
}

void AgendaUI::quitAgenda() {
    m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut() {
    m_userName = "";
    m_userPassword = "";
    cout << endl;
    cout << endl;
}

void AgendaUI::deleteUser() {
    cout << endl;
    m_agendaService.deleteUser(m_userName, m_userPassword);
    m_userName = "";
    m_userPassword = "";
    cout << "[delete agenda account] succeed!" << endl;
    cout << endl;
}

void AgendaUI::listAllUsers() {
    cout << endl;
    cout << "[list all users] " << endl;
    cout << endl;
    cout << setiosflags(ios::left);
    cout << setw(20) << "name" << setw(30) << "email" << "phone" << endl;
    list<User> userList = m_agendaService.listAllUsers();
    for(auto &x: userList) {
        cout << setw(20) << x.getName() << setw(30) << x.getEmail() << x.getPhone() << endl;
    }
    cout << resetiosflags(ios::left);
    cout << endl;
}

void AgendaUI::createMeeting() {
    cout << endl;
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int num;
    cin >> num;
    vector<string> partList;
    string temp;
    string result;
    
    for (int i = 1; i <= num; i++) {
        stringstream ss;
        ss << i;
        ss >> result;
        cout << "[create meeting] [please enter the participator " + result + " ]"
        << endl << "[create meeting] ";
        cin >> temp;
        partList.push_back(temp);
    }
    cout << "[create meeting] [title] "
    << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
    << endl;
    cout << "[create meeting] ";
    string sTime, eTime, title;
    cin >> title >> sTime >> eTime;
    if (m_agendaService.createMeeting(m_userName, title, sTime, eTime, partList)) {
        cout << "[create meeting] succeed!" << endl;
    }
    else {
        cout << "[create meeting] error!" << endl;
    }
    cout << endl;
}

void AgendaUI::listAllMeetings() {
    cout << endl;
    cout << "[list all meetings] " << endl;
    cout << endl;
    list<Meeting> meetingList(m_agendaService.listAllMeetings(m_userName));
    if (meetingList.empty()) {
        cout << "None" << endl;
        cout << endl;
    }
    else {
        printMeetings(meetingList);
    }
}

void AgendaUI::listAllSponsorMeetings() {
    cout << endl;
    cout << "[list all sponsor meetings] " << endl;
    cout << endl;
    list<Meeting> meetingList(m_agendaService.listAllSponsorMeetings(m_userName));
    if (meetingList.empty()) {
        cout << "None" << endl;
        cout << endl;
    }
    else {
        printMeetings(meetingList);
    }
}

void AgendaUI::listAllParticipateMeetings() {
    cout << endl;
    cout << "[list all participator meetings] " << endl;
    cout << endl;
    list<Meeting> meetingList(m_agendaService.listAllParticipateMeetings(m_userName));
    if (meetingList.empty()) {
        cout << "None" << endl;
        cout << endl;
    }
    else {
        printMeetings(meetingList);
    }
}

void AgendaUI::queryMeetingByTitle() {
    cout << endl;
    string title;
    cout << "[query meeting] " << "[title]:" << endl;
    cout << "[query meeting] ";
    cin >> title;
    list<Meeting> meetingList(m_agendaService.meetingQuery(m_userName, title));
    if (meetingList.empty()) {
        cout << "None" << endl;
        cout << endl;
    }
    else {
        printMeetings(meetingList);
    }
}

void AgendaUI::queryMeetingByTimeInterval() {
    cout << endl;
    string startTime, endTime;
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    cin >> startTime >> endTime;
    list<Meeting> meetingList(m_agendaService.meetingQuery(m_userName, startTime, endTime));
    if (meetingList.empty()) {
        cout << "None" << endl;
        cout << endl;
    }
    else {
        printMeetings(meetingList);
    }
}

void AgendaUI::deleteMeetingByTitle() {
    cout << endl;
    string title;
    cout << "[delete meeting] [title]:" << endl;
    cout << "[delete meeting] ";
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << "[delete meeting by title] succeed!" << endl;
    }
    else {
        cout << "[error] delete meeting fail!" << endl;
    }
    cout << endl;
}

void AgendaUI::deleteAllMeetings() {
    cout << endl;
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << "[delete all meetings] succeed!" << endl;
    }
    else {
        cout << "[error] delete all meeting fail!" << endl;
    }
    cout << endl;
}

void AgendaUI::addMeetingParticipator() {
    cout << endl;
    string title;
    string participator;
    cout << "[add participator] [title] [participator]:" << endl;
    cout << "[add participator] ";
    cin >> title >> participator;
    if (m_agendaService.addMeetingParticipator(m_userName, title, participator)) {
        cout << "[add meeting participator] succeed!" << endl;
    }
    else {
        cout << "[error] add meeting participator fail!" << endl;
    }
    cout << endl;
}

void AgendaUI::removeMeetingParticipator() {
    cout << endl;
    string title;
    string participator;
    cout << "[remove participator] [title] [participator]:" << endl;
    cout << "[remove participator] ";
    cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator)) {
        cout << "[remove meeting participator] succeed!" << endl;
    }
    else {
        cout << "[error] remove meeting participator fail!" << endl;
    }
    cout << endl;
}

void AgendaUI::quitMeeting() {
    cout << endl;
    string title;
    cout << "[quit meeting] [title]:" << endl;
    cout << "[quit meeting] ";
    cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title)) {
        cout << "[quit meeting] succeed!" << endl;
    }
    else {
        cout << "[error] quit meeting fail!" << endl;
    }
    cout << endl;
}

string vectorToString(vector<string> participatorlist) {
    string result = "";
    for (auto &x : participatorlist) {
        result += x;
        result += ',';
    }
    result = result.substr(0, result.size() - 1);
    return result;
}

void AgendaUI::printMeetings(list<Meeting> t_meetings) {
    cout << setiosflags(ios::left);
    cout << setw(15) << "title";
    cout << setw(15) << "sponsor";
    cout << setw(20) << "start time";
    cout << setw(20) << "end time";
    cout << "participators";
    cout << endl;
    for(auto &x : t_meetings) {
        cout << setw(15) << x.getTitle();
        cout << setw(15) << x.getSponsor();
        cout << setw(20) << Date::dateToString(x.getStartDate());
        cout << setw(20) << Date::dateToString(x.getEndDate());
        cout << vectorToString(x.getParticipator());
        cout << endl;
    }
    cout << resetiosflags(ios::left);
    cout << endl;
}
