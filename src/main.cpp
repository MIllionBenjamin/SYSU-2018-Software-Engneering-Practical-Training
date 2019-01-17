#include <iostream>
#include <stdlib.h>
#include "User.hpp"
#include "Meeting.hpp"
#include "Date.hpp"
#include "Storage.hpp"
#include "AgendaService.hpp"
#include "AgendaUI.hpp"

using namespace std;

int main()
{
    AgendaUI a;
    a.OperationLoop();
    return 0;
}
