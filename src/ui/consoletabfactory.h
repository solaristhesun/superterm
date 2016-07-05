#ifndef CONSOLETABFACTORY_H
#define CONSOLETABFACTORY_H

class CPortEnumerator;
class CConsoleTab;
class CSession;

class CConsoleTabFactory
{
public:
    static void setPortEnumerator(CPortEnumerator* portEnumerator);
    static CConsoleTab* createTab();
    static CConsoleTab* createTabFromSession(CSession* session);

private:
    static CPortEnumerator* sPortEnumerator;
};

#endif // CONSOLETABFACTORY_H
