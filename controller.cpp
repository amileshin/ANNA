#include "controller.hpp"

Controller::~Controller()
{
    if(S->isConnect())
        S->close();
    delete S;
}

Corvet::Corvet()
{
    Name_Info = "Corvet Controller";
    freq = 440;
    S = new COMConnect(19200);
    ch = 1;
}

void Corvet::switchchanel(int I)
{
    if (S->isConnect())
        ch = I + 1;
}

void Corvet::sendfreq(double F)
{
    freq = F;
    int Fr = F;
    QString A;
    A.push_back(ch);
    A.push_back(207);//CF
    A.push_back((char)(Fr >> 8));
    A.push_back((char)Fr);
    S->send(A);
    emit frq(freq);
}

/*void Corvet::sendatune(int A)
{
    int AT = atune[A];
    QString R;
    R.push_back(ch);
    R.push_back(202);//CA
    R.push_back((char)(AT >> 8));
    R.push_back((char)AT);
    S->send(R);
    emit atn(A);
}*/

void Corvet::Update()
{
    if (ch == 0)
        ch = 1;
    sendfreq(440);
    emit UpdateUI(Name_Info, freq, ch - 1, 2);
}
