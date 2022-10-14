#include "generator.hpp"

Generator::~Generator()
{
    if(S->isConnect())
        S->close();
    delete S;
}

Spectran::Spectran()
{
    S = new IPConnect("192.168.1.126", 5023);
    RF = false;
    freq = 0;
    pow = 0;
}

Spectran::~Spectran()
{
    if(S->isConnect())
        S->close();
    delete S;
}

void Spectran::Update()
{
    if (S->isConnect())
    {
        S->send(SCPI::getname());
        Name_Info = S->enter();
        S->send(SCPI::GENERATOR::getfrequency());
        freq = (S->enter()).toDouble();
        S->send(SCPI::GENERATOR::getpower());
        pow = (S->enter()).toDouble();
        S->send(SCPI::GENERATOR::getoutput());
        RF = (S->enter() == "ON");

        double frq = freq;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq = frq / 1000;
            frq_t++;
        }
        UpdateUI(Name_Info, frq, frq_t, pow, RF);
    }
}

void Spectran::sendfreq(double F)
{
    if (freq != F)
    {
        freq = F;
        S->send(SCPI::GENERATOR::setfrequency(F));
        double frq = freq;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq = frq / 1000;
            frq_t++;
        }
        emit UpdateFUI(frq);
        emit UpdateFTUI(frq_t);
    }
}

void Spectran::sendpow(double P)
{
    if (pow != P)
    {
        pow = P;
        S->send(SCPI::GENERATOR::setpower(P));
        emit UpdatePUI(pow);
    }
}

void Spectran::setRF(bool rf)
{
    if (rf)
    {
        S->send(SCPI::GENERATOR::ON());
        RF = true;
    }
    else
    {
        S->send(SCPI::GENERATOR::OFF());
        RF = false;
    }
    emit UpdateRF(rf);
}

AgilentMXG::AgilentMXG()
{
    S = new IPConnect("169.254.172.83", 5025);
    RF = false;
    freq = 0;
    pow = 0;
}

AgilentMXG::~AgilentMXG()
{
    if(S->isConnect())
        S->close();
    delete S;
}

void AgilentMXG::Update()
{
    if (S->isConnect())
    {
        S->send(SCPI::getname());
        Name_Info = S->enter();
        S->send(SCPI::GENERATOR::getfrequency());
        freq = (S->enter()).toDouble();
        S->send(SCPI::GENERATOR::getpower());
        pow = (S->enter()).toDouble();
        S->send(SCPI::GENERATOR::getoutput());
        RF = (S->enter() == "1");

        if (pow > -10)
        {
            pow = -10;
            S->send(SCPI::GENERATOR::setpower(pow));
        }

        double frq = freq;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq = frq / 1000;
            frq_t++;
        }
        emit UpdateUI(Name_Info, frq, frq_t, pow, RF);
    }
}

void AgilentMXG::sendfreq(double F)
{
    if (freq != F)
    {
        freq = F;
        S->send(SCPI::GENERATOR::setfrequency(F));
        double frq = freq;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq = frq / 1000;
            frq_t++;
        }
        emit UpdateFUI(frq);
        emit UpdateFTUI(frq_t);
    }
}

void AgilentMXG::sendpow(double P)
{
    if (pow != P && P <= -10)
    {
        pow = P;
        S->send(SCPI::GENERATOR::setpower(P));
    }
    emit UpdatePUI(pow);
}

void AgilentMXG::setRF(bool rf)
{
    if (rf)
    {
        S->send(SCPI::GENERATOR::ON());
        RF = true;
    }
    else
    {
        S->send(SCPI::GENERATOR::OFF());
        RF = false;
    }
    emit UpdateRF(rf);
}
