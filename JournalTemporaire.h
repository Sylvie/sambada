#ifndef JOURNALTEMPORAIRE_H
#define JOURNALTEMPORAIRE_H

#include <sstream>
#include <string>
#include <queue>
#include <iostream>

using namespace std;

class JournalTemporaire : public queue<string>
{
public:
    JournalTemporaire();
    virtual ~JournalTemporaire();


    template<class T>
    JournalTemporaire& operator<<(const T& token);

   // JournalTemporaire& operator<<(const string& s);

    JournalTemporaire& operator<<(JournalTemporaire& (*pf)(JournalTemporaire&));

    JournalTemporaire& operator<<(ostream& (*pf)(ostream&));

    JournalTemporaire& retourLigne();

    JournalTemporaire& synchronise();

  //  JournalTemporaire& flush(JournalTemporaire &jt);

protected:
    ostringstream oss;
    void push(const string & s);
   // vector<string> journal;

private:

};

template<class T>
JournalTemporaire& JournalTemporaire::operator<<(const T& token)
{
    cerr << "Bouh" << endl;
    oss << token;
    return *this;
}

JournalTemporaire& nl (JournalTemporaire& jt);

JournalTemporaire& endl (JournalTemporaire& jt);

JournalTemporaire& flush (JournalTemporaire& jt);



#endif // JOURNALTEMPORAIRE_H
