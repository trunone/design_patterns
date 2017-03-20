// Reference:
// https://sourcemaking.com/design_patterns/builder

#include <iostream>

using std::cout;
using std::endl;
using std::string;

enum PersistenceType
{
    File, Queue, Pathway
};

struct PersistenceAttribute
{
    PersistenceType type;
    string value;
};

class DistrWorkPackage
{
public:
    DistrWorkPackage(string type)
    {
        desc_ = "Distributed Work Package for: " + type;
    }
    void setFile(string f, string v)
    {
        tmp_ = "\n  File(" + f + "): " + v;
        desc_ += tmp_;
    }
    void setQueue(string q, string v)
    {
        tmp_ = "\n  Queue(" + q + "): " + v;
        desc_ += tmp_;
    }
    void setPathway(string p, string v)
    {
        tmp_ = "\n  Pathway(" + p + "): " + v;
        desc_ += tmp_;
    }
    string getState()
    {
        return desc_;
    }

private:
    string desc_, tmp_;
};

class Builder
{
public:
    virtual void configureFile(string) = 0;
    virtual void configureQueue(string) = 0;
    virtual void configurePathway(string) = 0;

    DistrWorkPackage *getResult()
    {
        return _result;
    }

protected:
    DistrWorkPackage *_result;
};

class UnixBuilder: public Builder
{
public:
    UnixBuilder()
    {
        _result = new DistrWorkPackage("Unix");
    }
    void configureFile(string name)
    {
        _result->setFile("flatFile", name);
    }
    void configureQueue(string queue)
    {
        _result->setQueue("FIFO", queue);
    }
    void configurePathway(string type)
    {
        _result->setPathway("thread", type);
    }
};

class VmsBuilder: public Builder
{
public:
    VmsBuilder()
    {
        _result = new DistrWorkPackage("Vms");
    }
    void configureFile(string name)
    {
        _result->setFile("ISAM", name);
    }
    void configureQueue(string queue)
    {
        _result->setQueue("priority", queue);
    }
    void configurePathway(string type)
    {
        _result->setPathway("LWP", type);
    }
};

class Reader
{
public:
    void setBuilder(Builder *b)
    {
        _builder = b;
    }

    void construct(PersistenceAttribute[], int);

private:
    Builder *_builder;
};

void Reader::construct(PersistenceAttribute list[], int num)
{
    for (int i = 0; i < num; i++)
        if (list[i].type == File)
            _builder->configureFile(list[i].value);
        else if (list[i].type == Queue)
            _builder->configureQueue(list[i].value);
        else if (list[i].type == Pathway)
            _builder->configurePathway(list[i].value);
}

const int NUM_ENTRIES = 6;

PersistenceAttribute input[NUM_ENTRIES] =
{
    {
        File, "state.dat"
    },
    {
        File, "config.sys"
    },
    {
        Queue, "compute"
    },
    {
        Queue, "log"
    },
    {
        Pathway, "authentication"
    },
    {
        Pathway, "error processing"
    }
};

int main()
{
    UnixBuilder unixBuilder;
    VmsBuilder vmsBuilder;
    Reader reader;

    reader.setBuilder(&unixBuilder);
    reader.construct(input, NUM_ENTRIES);
    cout << unixBuilder.getResult()->getState() << endl;

    reader.setBuilder(&vmsBuilder);
    reader.construct(input, NUM_ENTRIES);
    cout << vmsBuilder.getResult()->getState() << endl;
}
