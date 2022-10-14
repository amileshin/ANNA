#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QWidget>
#include <QString>
#include <vector>
#include <list>
#include "connect.hpp"

class Controller: public QObject
{
    Q_OBJECT
protected:
    double freq;
    int ch;
public:
    //std::vector <double> atune;
    QString Name_Info;
    Connect *S;

    ~Controller();

    friend class MainGUI;
public slots:
    virtual void switchchanel(int I) = 0;
    virtual void sendfreq(double F) = 0;
    //virtual void sendatune(int A) = 0;
    virtual void Update() = 0;

signals:
    void UpdateUI(QString Name, double freq, int ch, int A_ch);
    void frq(double frq);
    //void atn(int A);
};

class Corvet: public Controller
{
    Q_OBJECT
public:
    Corvet();
public slots:
    void switchchanel(int I);
    void sendfreq(double F);
    //void sendatune(int A);
    void Update();
};

#endif // CONTROLLER_HPP
