#ifndef CONSOLETABFACTORY_H
#define CONSOLETABFACTORY_H

class PortEnumerator;
class ConsoleTab;
class Session;

class ConsoleTabFactory
{
public:
    static void        setPortEnumerator(PortEnumerator* portEnumerator);
    static ConsoleTab* createTab();
    static ConsoleTab* createTabFromSession(Session* session);

private:
    static PortEnumerator* sPortEnumerator;
};

#endif // CONSOLETABFACTORY_H
