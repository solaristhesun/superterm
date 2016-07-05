#include "consoletabfactory.h"
#include "enumerator/portenumerator.h"
#include "consoletab.h"
#include "obj/session.h"

CPortEnumerator* CConsoleTabFactory::sPortEnumerator = NULL;

void CConsoleTabFactory::setPortEnumerator(CPortEnumerator* portEnumerator)
{
    sPortEnumerator = portEnumerator;
}

CConsoleTab* CConsoleTabFactory::createTab()
{
    return new CConsoleTab(sPortEnumerator, NULL);
}

CConsoleTab* CConsoleTabFactory::createTabFromSession(CSession* session)
{
    CConsoleTab* tab = new CConsoleTab(sPortEnumerator, session);
    tab->setLabel(session->getDeviceName());

    return tab;
}

// EOF <stefan@scheler.com>
