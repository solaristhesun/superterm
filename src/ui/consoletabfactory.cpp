#include "ui/consoletabfactory.h"
#include "ui/consoletab.h"
#include "serial/portenumerator.h"
#include "session/session.h"

PortEnumerator* ConsoleTabFactory::sPortEnumerator = nullptr;

void ConsoleTabFactory::setPortEnumerator(PortEnumerator* portEnumerator)
{
    sPortEnumerator = portEnumerator;
}

ConsoleTab* ConsoleTabFactory::createTab()
{
    return new ConsoleTab(sPortEnumerator, nullptr);
}

ConsoleTab* ConsoleTabFactory::createTabFromSession(Session* session)
{
    ConsoleTab* tab = new ConsoleTab(sPortEnumerator, session);
    tab->setLabel(session->getTabLabel());

    return tab;
}

// EOF <stefan@scheler.com>
