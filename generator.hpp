#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <QObject>
#include "connect.hpp"
#include "SCPI.hpp"

class Generator: public QObject
{
    Q_OBJECT
protected:
    QString Name_Info;
    double freq, pow;
public:
    bool RF;
    Connect* S;
    ~Generator();

public slots:
    virtual void sendfreq(double F) = 0;
    virtual void sendpow(double P) = 0;
    virtual void setRF(bool RF) = 0;
    virtual void Update() = 0;

signals:
    void UpdateUI(QString Name, double frq, int frq_t, double pw, bool RF);
    void UpdateFUI(double frq);
    void UpdateFTUI(int frqt);
    void UpdatePUI(double pow);
    void UpdateRF(bool RF);
};

class Spectran: public Generator
{
    Q_OBJECT
public:
    Spectran();
    ~Spectran();

public slots:
    void sendfreq(double F);
    void sendpow(double P);
    void setRF(bool rf);
    void Update();
};


class AgilentMXG: public Generator
{
    Q_OBJECT
public:
    AgilentMXG();
    ~AgilentMXG();

public slots:
    void sendfreq(double F);
    void sendpow(double P);
    void setRF(bool RF);
    void Update();
};

#endif // GENERATOR_HPP
