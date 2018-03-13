#include "ui/consoletabfactory.h"
#include "ui/consoletab.h"
#include "serial/portenumerator.h"
#include "session/session.h"

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
    tab->setLabel(session->getTabLabel());

    return tab;
}

// EOF <stefan@scheler.com>
