#include "AgendaService.hpp"
using namespace std;

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

void AgendaService::startAgenda() {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda() {}

bool AgendaService::userLogIn(const string &userName, const string &password) {
    list<User> userList = m_storage->queryUser([&userName](const User& x) {
        if (x.getName() == userName) return true;
        else return false;
    });
    if (userList.empty()) return false;
    if (userList.front().getPassword() == password) return true;
    else return false;
}

bool AgendaService::userRegister(const string &userName,
                                 const string &password,
                                 const string &email,
                                 const string &phone){
    User temp(userName, password, email, phone);
    list<User> userList = m_storage->queryUser([&userName](const User& x) {
        if (x.getName() == userName) return true;
        else return false;
    });
    if (!userList.empty()) return false;
    m_storage->createUser(temp);
    return true;
}

bool AgendaService::deleteUser(const string &userName, const string &password) {
    list<User> userList = m_storage->queryUser([&userName](const User& a) {
        if (a.getName() == userName) return true;
        else return false;
    });
    if (userList.empty()) return false;
    if (password != userList.front().getPassword()) return false;
    m_storage->deleteUser([&userName](const User& x) {
        if (x.getName() == userName) return true;
        else return false;
    });
    m_storage->deleteMeeting([&userName](const Meeting& x) {
        if (x.getSponsor() == userName) return true;
        else return false;
    });
    list<Meeting> meetingList = this->listAllMeetings(userName);
    for (auto &x : meetingList) {
        this->quitMeeting(userName, x.getTitle());
    }
    return true;
}

list<User> AgendaService::listAllUsers() const {
    list<User> res = m_storage->queryUser([](const User& x) {
        return true;
    });
    return res;
}

bool AgendaService::createMeeting(const string &userName,
                                  const string &title,
                                  const string &startDate,
                                  const string &endDate,
                                  const vector<string> &participator) {
    list<Meeting> meetingList = m_storage->queryMeeting([&title](const Meeting& x) {
        if (x.getTitle() == title) return true;
        else return false;
    });
    if (!meetingList.empty()) return false;
    Date start(startDate);
    Date end(endDate);
    Date zero;
    if (!Date::isValid(start) || !Date::isValid(end)) return false;
    if (start >= end || start == zero || end == zero) return false;
    if (m_storage->queryUser([&userName](const User& u) {
        return (u.getName() == userName);
    }).empty()) return false;
    list<Meeting> temp = listAllMeetings(userName);
    for (auto &x : temp) {
        if (!(end <= x.getStartDate() || start >= x.getEndDate())) return false;
    }
    for (auto &x : participator) {
        if (m_storage->queryUser([&x](const User& user) {
            return (user.getName() == x);
        }).empty()) return false;
        if (x == userName) return false;
        temp = listAllMeetings(x);
        for (auto &y : temp) {
            if (!(end <= y.getStartDate() || start >= y.getEndDate())) return false;
        }
    }
    for (int i = 0; i < participator.size(); i++) {
        for (int j = i; j < participator.size() - 1; j++) {
            if (participator[i] == participator[j + 1]) return false;
        }
    }
    Meeting toCreate(userName, participator, start, end, title);
    m_storage->createMeeting(toCreate);
    return true;
}

bool AgendaService::addMeetingParticipator(const string &userName,
                                           const string &title,
                                           const string &participator) {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName, &title, &participator](const Meeting& x) {
        if (x.getSponsor() == userName && x.getTitle() == title && !x.isParticipator(participator)) return true;
        else return false;
    });
    if (meetingList.empty()) return false;
    list<User> userList = this->listAllUsers();
    auto it = userList.begin();
    for (;it != userList.end(); it++) {
        if (it->getName() == participator) break;
    }
    if (it == userList.end()) return false;
    list<Meeting> temp = listAllMeetings(participator);
    Date start = meetingQuery(userName, title).front().getStartDate();
    Date end = meetingQuery(userName, title).front().getEndDate();
    for (auto &x : temp) {
        if (!(end <= x.getStartDate() || start >= x.getEndDate())) return false;
    }
    int count = m_storage->updateMeeting(
                                         [&userName, &title, &participator](const Meeting& x) {
                                             if (x.getSponsor() == userName && x.getTitle() == title && !x.isParticipator(participator)) return true;
                                             else return false;
                                         },
                                         [&participator](Meeting& x) {
                                             x.addParticipator(participator);
                                         });
    return true;
}

bool AgendaService::removeMeetingParticipator(const string &userName,
                                              const string &title,
                                              const string &participator) {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName, &title, &participator](const Meeting& x) {
        if (x.getSponsor() == userName && x.getTitle() == title && x.isParticipator(participator)) return true;
        else return false;
    });
    if (meetingList.empty()) return false;
    Meeting meet = meetingList.front();
    if (meet.getParticipator().size() == 1) {
        int count = m_storage->deleteMeeting([&userName, &title, &participator](const Meeting& x) {
            if (x.getSponsor() == userName && x.getTitle() == title && x.isParticipator(participator)) return true;
            else return false;
        });
    }
    else
        int count = m_storage->updateMeeting(
                                             [&userName, &title, &participator](const Meeting& x) {
                                                 if (x.getSponsor() == userName && x.getTitle() == title && x.isParticipator(participator)) return true;
                                                 else return false;
                                             },
                                             [&participator](Meeting& x) {
                                                 x.removeParticipator(participator);
                                             });
    return true;
}

bool AgendaService::quitMeeting(const string &userName, const string &title) {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName, &title](const Meeting& x) {
        if (x.getTitle() == title && x.isParticipator(userName)) return true;
        else return false;
    });
    if (meetingList.empty()) return false;
    Meeting meet = meetingList.front();
    if (userName == meet.getSponsor()) return false;
    if (meet.getParticipator().size() == 1) {
        int count = m_storage->deleteMeeting([&userName, &title](const Meeting& x) {
            if (x.getTitle() == title && x.isParticipator(userName)) return true;
            else return false;
        });
    }
    else {
        int count = m_storage->updateMeeting(
                                             [&userName, &title](const Meeting& x) {
                                                 if (x.getTitle() == title && x.isParticipator(userName)) return true;
                                                 else return false;
                                             },
                                             [&userName](Meeting& x) {
                                                 x.removeParticipator(userName);
                                             });
    }
    return true;
}

list<Meeting> AgendaService::meetingQuery(const string &userName, const string &title) const {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName, &title](const Meeting& x) {
        if (x.getSponsor() == userName || x.isParticipator(userName)) {
            if (x.getTitle() == title) return true;
            else return false;
        }
        else {
            return false;
        }
    });
    return meetingList;
}

list<Meeting> AgendaService::meetingQuery(const string &userName,
                                          const string &startDate,
                                          const string &endDate) const {
    list<Meeting> meetingList;
    Date start = Date(startDate);
    Date end = Date(endDate);
    if (!(Date::isValid(start) && Date::isValid(end))) return meetingList;
    meetingList = m_storage->queryMeeting([&userName, &start, &end](const Meeting& x) {
        if (x.getSponsor() == userName || x.isParticipator(userName)) {
            if (x.getStartDate() >= start && x.getStartDate() <= end) return true;
            else return false;
        }
        else {
            return false;
        }
    });
    return meetingList;
}

list<Meeting> AgendaService::listAllMeetings(const string &userName) const {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName](const Meeting& x) {
        if (x.getSponsor() == userName || x.isParticipator(userName)) return true;
        else return false;
    });
    return meetingList;
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string &userName) const {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName](const Meeting& x) {
        if (x.getSponsor() == userName) return true;
        else return false;
    });
    return meetingList;
}

list<Meeting> AgendaService::listAllParticipateMeetings(const string &userName) const {
    list<Meeting> meetingList = m_storage->queryMeeting([&userName](const Meeting& x) {
        if (x.isParticipator(userName)) return true;
        else return false;
    });
    return meetingList;
}

bool AgendaService::deleteMeeting(const string &userName, const string &title) {
    int res = m_storage->deleteMeeting([&userName, &title](const Meeting& x) {
        if (x.getSponsor() == userName && x.getTitle() == title) return true;
        else return false;
    });
    if (res > 0) return true;
    else return false;
}

bool AgendaService::deleteAllMeetings(const string &userName) {
    int res = m_storage->deleteMeeting([&userName](const Meeting& x) {
        if (x.getSponsor() == userName) return true;
        else return false;
    });
    if (res > 0) return true;
    else return false;
}
