#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <list>
#include <QThread>
#include "connect.hpp"
#include "SCPI.hpp"

class Analyzer: public QObject
{
    Q_OBJECT
protected:
    QString Name_Info;
    double center;
    double span;
    double rf_level;

public:
    Connect *S;
    ~Analyzer();

public slots:
    virtual void sendfreqspec(double freqstart, double freqfinish) = 0;
    virtual void sendfreqcentspec(double freqcenter, double freqspan) = 0;
    virtual void sendfreqcent(double freqcenter) = 0;
    virtual void sendrflevel(double rflevel) = 0;
    virtual void sendmark(double freq = 0) = 0;
    virtual double sendvalue(double freq = 0, bool peak = false) = 0;
    virtual double sendpeakfrq() = 0;
    virtual void sendtracetype(int type) = 0;
    virtual void sendcreatemarker() = 0;
    virtual void sendpeakmarker() = 0;
    virtual void sendoffmarker() = 0;
    virtual void Update() = 0;

signals:
    void UpdateUI(QString Name, double frqc, double frqc_t, double frqs, double frqs_t, int rf_level, int trace_type);
    void frq_c(double frqc);
    void frq_ct(int frqct);
    void frq_s(double frqs);
    void frq_st(int frqst);
    void rf_lv(double rf_lev);
    void mark_val(double pw);
    void tracetype(int tracetype);

};

class AgilentX: public Analyzer
{
    Q_OBJECT
public:
    AgilentX();
    ~AgilentX();

public slots:
    void sendfreqspec(double freqstart, double freqfinish);
    void sendfreqcentspec(double freqcenter, double freqspan);
    void sendfreqcent(double freqcenter);
    void sendrflevel(double rflevel);
    void sendmark(double freq = 0);
    double sendvalue(double freq = 0, bool peak = false);
    double sendpeakfrq();
    void sendtracetype(int type);
    void sendcreatemarker();
    void sendpeakmarker();
    void sendoffmarker();
    void Update();
};
#endif // ANALYZER_HPP
