#include "config.hpp"

Config::Config(const Config& A)
{
    result = false;
    for(PM* I : A.Data)
    {
        this->Data.push_back(I);
    }
}

Config::Config(std::string FileName)
{
    result = false;
    if (FileName != "")
        this->fromfile(FileName);
}

Config::~Config()
{
    if (!this->empty())
    for (PM* I : Data)
        delete I;
}

Config& Config::operator=(const Config& A)
{
    result = A.result;
    this->clear();
    for(PM* I : A.Data)
    {
        this->Data.push_back(I);
    }
    return *this;
}

bool Config::empty() const
{
    return Data.empty();
}

void Config::clear()
{
    result = false;
    Data.clear();
}

QString Config::out(int filter) const
{
    QString Ans = "";
    for (PM* i : Data)
        if (i->type() == filter || filter == -1)
            Ans += i->save_str() + '\n';
    return Ans;
}

bool Config::fromfile_nr(std::ifstream* FIN)
{
    try
    {
        do
        {
            std::string D;
            std::getline(*FIN, D);
            QStringList Arg = QString::fromStdString(D).split(" ", QString::SkipEmptyParts);
            if (Arg[0] == "-dots")
            {
                double pow = -1, pd = 20;
                for (int i = 1; i < Arg.size(); ++i)
                {
                    if (Arg[i] == "-d")
                    {
                        pd = Arg[i + 1].toDouble();
                        i++;
                    }
                    else
                        pow = Arg[i].toDouble();
                }
                std::getline(*FIN, D);
                do
                {
                    QStringList Arg1 = QString::fromStdString(D).split(" ", QString::SkipEmptyParts);
                    PM* N;
                    if (Arg1.size() == 2)
                        N = new T1(Arg1[0].toInt(), Arg1[1].toDouble(), pd);
                    else if (Arg1.size() == 1)
                        N = new T1(Arg1[0].toInt(), pow, pd);
                    else
                        throw "Wrong Argument String";
                    Data.push_back(N);
                    if (FIN->eof())
                        throw "Wrong File Struct";
                    std::getline(*FIN, D);
                } while (D != "-end");
            }
            else if (Arg[0] == "-freq")
            {
                if (Arg.size() != 2)
                    throw "Wrong Argument String";
                unsigned freq;
                freq = Arg[1].toInt();
                PM* N = new T2(freq);
                Data.push_back(N);
            }
            else if (Arg[0] == "-comp")
            {
                if (Arg.size() != 2)
                    throw "Wrong Argument String";
                unsigned freq;
                freq = Arg[1].toInt();
                PM* N = new T3(freq);
                Data.push_back(N);
            }
            else if (Arg[0] == "-fltr")
            {
                if (Arg.size() != 2)
                    throw "Wrong Argument String";
                unsigned freq;
                freq = Arg[1].toInt();
                PM* N = new T4(freq);
                Data.push_back(N);
            }
            else if (Arg[0] == "-mc")
            {
                if (Arg.size() != 1)
                    throw "Wrong Argument String";
                PM* N = new T5();
                Data.push_back(N);
            }
            else if (Arg[0] == "-if")
            {
                if (Arg.size() != 1)
                    throw "Wrong Argument String";
                PM* N = new T6();
                Data.push_back(N);
            }
        } while (!(FIN->eof()));
    }
    catch (const QString i)
    {
        return false;
    }
    return true;
}

bool Config::fromfile_vr(std::ifstream* FIN)
{
    try
    {
        std::string D;
        do
        {
            std::getline(*FIN, D);
            QStringList Arg = QString::fromStdString(D).split(" ", QString::SkipEmptyParts);
            if (Arg.size() > 1)
            {
                if (Arg[0] == "-dots")
                {
                    int pow = -1, pd = 20;
                    for (int i = 1; i < Arg.size(); ++i)
                    {
                        if (Arg[i] == "-d")
                        {
                            pd = Arg[i + 1].toInt();
                            i++;
                        }
                        else
                            pow = Arg[i].toInt();
                    }
                    std::getline(*FIN, D);
                    do
                    {
                        QStringList Arg1 = QString::fromStdString(D).split(" ", QString::SkipEmptyParts);
                        PM* N;
                        if (Arg1.size() == 3)
                            N = new T1(Arg1[0].toInt(), Arg1[1].toDouble(), Arg1[2].toDouble(), pd);
                        else if (Arg1.size() == 1)
                            N = new T1(Arg1[0].toInt(), pow, Arg1[2].toDouble(), pd);
                        else
                            throw "Wrong Argument String";
                        Data.push_back(N);
                        if (FIN->eof())
                            throw "Wrong File Struct";
                        std::getline(*FIN, D);
                    } while (D != "-end");
                }
                else if (Arg[0] == "-freq")
                {
                    if (Arg.size() != 3)
                        throw "Wrong Argument String";
                    PM* N = new T2(Arg[1].toInt(), Arg[2].toDouble());
                    Data.push_back(N);
                }
                else if (Arg[0] == "-comp")
                {
                    if (Arg.size() != 3)
                        throw "Wrong Argument String";
                    PM* N = new T3(Arg[1].toInt(), Arg[2].toDouble());
                    Data.push_back(N);
                }
                else if (Arg[0] == "-fltr")
                {
                    if (Arg.size() != 4)
                        throw "Wrong Argument String";
                    PM* N = new T4(Arg[1].toInt(), Arg[2].toDouble(), Arg[3].toDouble());
                    Data.push_back(N);
                }
                else if (Arg[0] == "-mc")
                {
                    if (Arg.size() != 2)
                        throw "Wrong Argument String";
                    PM* N = new T5(Arg[1].toDouble());
                    Data.push_back(N);
                }
                else if (Arg[0] == "-if")
                {
                    if (Arg.size() != 3)
                        throw "Wrong Argument String";
                    PM* N = new T6(Arg[1].toDouble(), Arg[2].toDouble());
                    Data.push_back(N);
                }
            }
        } while (!(FIN->eof()));
    }
    catch (const QString i)
    {
        return false;
    }
    return true;
}

bool Config::fromfile(std::string FileName)
{
    this->clear();
    std::string D;
    std::ifstream F;
    F.open(FileName);
    F >> D;
    if (F.is_open())
    {
        bool ans;
        if (D == "result")
        {
            ans = this->fromfile_vr(&F);
            this->result = true;
        }
        else
        {
            F.seekg(0);
            ans = this->fromfile_nr(&F);
            this->result = false;
        }
        F.close();
        return ans;
    }
    else
        return false;
}

bool Config::tofile(std::string FileName) const
{
    std::ofstream F;
    F.open(FileName);
    if (!F.is_open())
        return false;
    if (this->result)
        F << "result\n";
    int lt = -1;
    for (PM* i : this->Data)
    {
        if (i->is_many() && lt != i->type())
        {
            lt = i->type();
            F << i->type_str() << '\n';
        }
        if (lt != i->type() && lt != -1)
        {
            lt = -1;
            F << "-end\n";
        }
        if (this->result)
            F << i->file_str() << '\n';
        else
            F << i->save_str().toStdString() << '\n';
    }
    F.close();
    return true;
}

bool Config::totxtfile(std::string FileName) const
{
    std::ofstream F;
    F.open(FileName);
    if (!F.is_open())
        return false;
    for (PM* i : this->Data)
    {
        F << i->file_txt_str() << '\n';
    }
    F.close();
    return true;
}