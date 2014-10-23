#include "journaltemporaire.h"

JournalTemporaire::JournalTemporaire()
: queue<string>(), oss()
{
}

JournalTemporaire::~JournalTemporaire()
{
}

/*JournalTemporaire& JournalTemporaire::operator<<(const string& s)
{
    oss.str(oss.str()+s);
    return *this;
}*/

JournalTemporaire& JournalTemporaire::operator<<(JournalTemporaire& (*pf)(JournalTemporaire&))
{
    return pf(*this);
}

JournalTemporaire& JournalTemporaire::operator<<(ostream& (*pf)(ostream&))
{
    pf(oss);
    return *this;
}

JournalTemporaire& JournalTemporaire::retourLigne()
{
    this->push(oss.str());
    oss.str("");
    oss.clear();
    return *this;
}

JournalTemporaire& JournalTemporaire::synchronise()
{
    if(oss.str()!="");
    {
        retourLigne();
    }
}

void JournalTemporaire::push(const string& s)
{
    queue<string>::push(s);
}

/*JournalTemporaire& JournalTemporaire::flush(JournalTemporaire &jt)
{
    synchronise();
    return *this;
}*/


JournalTemporaire& nl (JournalTemporaire& jt)
{
    jt.retourLigne();
    return jt;
}

JournalTemporaire& endl (JournalTemporaire& jt)
{
    jt << nl;
    return jt;
}

JournalTemporaire& flush (JournalTemporaire& jt)
{
    jt.synchronise();
    return jt;
}


