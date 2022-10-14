#ifndef PM_HPP
#define PM_HPP

#include <iostream>
#include <list>
#include <QThread>
#include "analyzer.hpp"
#include "generator.hpp"
#include "controller.hpp"


class PM
{
protected:
//    unsigned frq;
public:
    virtual const int type() = 0;
    virtual bool is_many() = 0;
    virtual const std::string type_str() = 0;
    virtual QString save_str() = 0;
    virtual QString res_str() = 0;
    virtual std::string file_txt_str() = 0;
    virtual std::string file_str() = 0;
    //virtual bool cmp(PM& A) = 0;//Compare Same type
    //virtual bool cmp(PM* A) = 0;


    /*static bool cmp_PM(PM* A, PM* B)
    {
        if (A->type() == B->type())
            return A->cmp(B);
        else
            return A->type() > B->type();
    }*/

    virtual void Metric(Generator* G, Analyzer* A, Controller* C) = 0;
    //friend void cmp1(PM* A, PM* B);
    friend class Config;
};

class T1 : public PM
{
private:
    unsigned frq;
    double pw_in, pw_out;
    double pw_ds;
    std::list <std::pair<int, int>> At;

public:
    T1(unsigned F, double P, double P_D)
    {
        frq = F;
        pw_in = P;
        pw_ds = P_D;
        //pw_dd = P_Dd;
    }

    T1(unsigned F, double Pin, double Pout, double P_D)
    {
        frq = F;
        pw_in = Pin;
        pw_out = Pout;
        pw_ds = P_D;
        //pw_dd = P_Dd;
    }

    const int type()
    {
        return 0;
    }

    bool is_many()
    {
        return true;
    }

    const std::string type_str()
    {
        return "-dots -d " + QString::number(pw_ds).toStdString();
    }

    QString save_str()
    {
        QString Ans = QString::number(frq) + ' ' + QString::number(pw_in);
        return Ans;
    }

    QString res_str()
    {
        QString Ans = QString::number(frq) + " : " + QString::number(pw_out - pw_in);
        if ((pw_out - pw_in) >= pw_ds)
            Ans = "<span style = 'color:green;'>" + Ans + "<\span>\n";
        else
            Ans = "<span style = 'color:red;'>" + Ans + "<\span>\n";
        return Ans;
    }

    std::string file_str()
    {
        QString Ans = QString::number(frq) + ' ' + QString::number(pw_in) + ' ' + QString::number(pw_out);
        return Ans.toStdString();
    }

    std::string file_txt_str()
    {
        QString res = "Gain " + QString::number(frq) + ':' + QString::number(pw_out - pw_in);
        return res.toStdString();
    }

    void Metric(Generator* G, Analyzer* A, Controller* C)
    {
        G->setRF(true);
        A->sendfreqcentspec(70000000, 20000000);
        G->sendfreq((double)this->frq * 1000000);
        G->sendpow(this->pw_in);
        C->sendfreq(this->frq);
        pw_out = A->sendvalue();
        G->setRF(false);
    }
};

class T2 : public PM
{
private:
    unsigned frq;
    double frq_delta;
public:
    T2(int F = 440)
    {
        frq = F;
    }

    T2(int F, double F_delta)
    {
        frq = F;
        frq_delta = F_delta;
    }

    const int type()
    {
        return 1;
    }

    bool is_many()
    {
        return false;
    }

    const std::string type_str()
    {
        return "-freq";
    }

    QString save_str()
    {
        return "-freq " + QString::number(frq);
    }

    QString res_str()
    {
        double F = frq;
        F *= pow(10, 6);
        return "-freq " + QString::number(frq) + " : " + QString::number(frq_delta / F);
    }

    std::string file_str()
    {
        QString Ans = "-freq " + QString::number(frq) + ' ' + QString::number(frq_delta);
        return Ans.toStdString();
    }

    std::string file_txt_str()
    {
        double F = frq;
        F *= pow(10, 6);
        QString res = "Tuning accuracy:" + QString::number(frq_delta / F);
        return res.toStdString();
    }

    void Metric(Generator* G, Analyzer* A, Controller* C)
    {
        A->sendfreqcentspec(70000000, 10000);
        G->sendfreq((double)this->frq * 1000000);
        C->sendfreq(this->frq);
        G->setRF(true);
        frq_delta = A->sendpeakfrq();
        G->setRF(false);
    }
};

class T3: public PM
{
private:
    unsigned frq;
    double pw_in, pw_out;
public:
    T3(int F = 3000)
    {
        frq = F;
    }

    T3(int F, double PW_in)
    {
        frq = F;
        pw_in = PW_in;
    }

    const int type()
    {
        return 2;
    }

    bool is_many()
    {
        return false;
    }

    const std::string type_str()
    {
        return "-comp";
    }

    QString save_str()
    {
        return "-comp " + QString::number(frq);
    }

    QString res_str()
    {
        return "Compresion(" + QString::number(frq) + "): " + QString::number(pw_in);
    }

    std::string file_str()
    {
        QString res = "-comp " + QString::number(frq) + ' ' + QString::number(pw_in);
        return res.toStdString();
    }

    std::string file_txt_str()
    {
        QString res = "Compression: In:" + QString::number(pw_in) + " Out:" + QString::number(pw_out);
        return res.toStdString();
    }

    void Metric(Generator* G, Analyzer* A, Controller* C)
    {
        A->sendfreqcentspec(70000000, 20000000);
        G->sendfreq((double)this->frq * 1000000);
        C->sendfreq(this->frq);
        G->setRF(true);
        pw_in = -47;
        G->sendpow(pw_in);
        QThread::msleep(100);
        pw_out = A->sendvalue();
        double dpw = pw_out - pw_in;
        do
        {
            pw_in += 1;
            G->sendpow(pw_in);
            QThread::msleep(100);
            pw_out = A->sendvalue();
        }
        while(dpw - 1 < pw_out - pw_in);
        G->setRF(false);
    }
};

class T4 : public PM
{
private:
    double frq_st, frq_fin;
public:
    unsigned frq;
    T4(int F = 440)
    {
        frq = F;
    }

    T4(int F, double F_st, double F_fin)
    {
        frq = F;
        frq_st = F_st;
        frq_fin = F_fin;
    }

    const int type()
    {
        return 3;
    }

    bool is_many()
    {
        return false;
    }

    const std::string type_str()
    {
        return "-fltr";
    }

    QString save_str()
    {
        return "-fltr " + QString::number(frq);
    }

    QString res_str()
    {
        return "Filter(" + QString::number(frq) + "): " + QString::number(frq_st + frq_fin) + '(' + QString::number(frq_fin) + ';' + QString::number(frq_st) + ')';
    }

    std::string file_str()
    {
        QString res = "-fltr " + QString::number(frq) + ' ' + QString::number(frq_st) + ' ' + QString::number(frq_fin);
        return res.toStdString();
    }

    std::string file_txt_str()
    {
        QString res = "Filter: " + QString::number(frq_st + frq_fin);
        return res.toStdString();
    }

    void Metric(Generator* G, Analyzer* A, Controller* C)
    {
        A->sendfreqcentspec(70000000, 20000000);
        G->sendfreq((double)this->frq * 1000000);
        G->sendpow(-40);
        C->sendfreq(this->frq);
        G->setRF(true);
        A->sendtracetype(3);
        double cpw, dpw;
        frq_st = 0;
        frq_fin = 0;
        cpw = A->sendvalue();
        do
        {
            frq_st += 0.1;
            G->sendfreq(((double)frq - frq_st) * 1000000);
            QThread::msleep(50);
            dpw = A->sendvalue((70 + frq_st) * 1000000);
        } while (cpw - dpw < 3);

        do
        {
            frq_fin += 0.1;
            G->sendfreq(((double)frq + frq_fin) * 1000000);
            QThread::msleep(50);
            dpw = A->sendvalue((70 - frq_fin) * 1000000);
        } while (cpw - dpw < 3);
        A->sendoffmarker();
        A->sendtracetype(0);
        G->setRF(false);
    }
};

class T5: public PM
{
private:
    double res;
public:
    T5()
    {}

    T5(double ampl)
    {
        res = ampl;
    }

    const int type()
    {
        return 4;
    }

    bool is_many()
    {
        return false;
    }

    const std::string type_str()
    {
        return "-mc";
    }

    QString save_str()
    {
        return "-mc";
    }

    QString res_str()
    {
        return "Mirror channel: " + QString::number(res);
    }

    std::string file_str()
    {
        QString rs = "-mc " + QString::number(res);
        return rs.toStdString();
    }

    std::string file_txt_str()
    {
        QString rs = "Mirror channel: " + QString::number(res);
        return rs.toStdString();
    }

    void Metric(Generator* G, Analyzer* A, Controller* C)
    {
        double Amp, rs, PW = -107;
        A->sendfreqcentspec(70000000, 500000);
        G->sendfreq(500000000);
        G->sendpow(PW);
        C->sendfreq(500);
        G->setRF(true);
        QThread::sleep(1);
        Amp = A->sendvalue(0, true);
        G->sendfreq(640000000);
        QThread::msleep(100);
        while ((rs = A->sendvalue(0, true)) < Amp)
        {
            PW += 1;
            G->sendpow(PW);
        }
        G->setRF(false);
        res = PW - (-107);
    }
};


class T6 : public PM
{
private:
    double IF1, IF2;
public:
    T6()
    {}

    T6(double ampl1, double ampl2)
    {
        IF1 = ampl1;
        IF2 = ampl2;
    }

    const int type()
    {
        return 5;
    }

    bool is_many()
    {
        return false;
    }

    const std::string type_str()
    {
        return "-if";
    }

    QString save_str()
    {
        return "-if";
    }

    QString res_str()
    {
        return "Intermeiate Frequncy: 1 - " + QString::number(IF1) + ", 2 - " + QString::number(IF2);
    }

    std::string file_str()
    {
        QString rs = "-if " + QString::number(IF1) + QString::number(IF2);
        return rs.toStdString();
    }

    std::string file_txt_str()
    {
        QString rs = "Intermeiate Frequncy:: 1 - " + QString::number(IF1) + ", 2 - " + QString::number(IF2);
        return rs.toStdString();
    }

    void Metric(Generator* G, Analyzer* A, Controller* C)
    {
        double Amp1, Amp2;
        A->sendfreqcentspec(70000000, 10000000);
        G->sendfreq(1499000000);
        G->sendpow(-27);
        C->sendfreq(1499);
        G->setRF(true);
        QThread::sleep(1);
        Amp1 = A->sendvalue(0, true);
        G->sendfreq(2340000000);
        QThread::msleep(100);
        Amp2 = A->sendvalue(0, true);
        G->setRF(false);
        IF1 = Amp1 - Amp2;
        G->sendfreq(1500000000);
        C->sendfreq(1500);
        G->setRF(true);
        QThread::msleep(100);
        Amp1 = A->sendvalue(0, true);
        G->sendfreq(995000000);
        QThread::msleep(100);
        Amp2 = A->sendvalue(0, true);
        G->setRF(false);
        IF2 = Amp1 - Amp2;
    }
};
#endif // PM_HPP
