#include "analyzer.hpp"

Analyzer::~Analyzer()
{
    delete S;
}

AgilentX::AgilentX()
{
    S = new IPConnect("169.254.148.30", 5025);
    center = 0;
    span = 0;
}

AgilentX::~AgilentX()
{
    delete S;
}

void AgilentX::sendfreqspec(double freqstart, double freqfinish)
{
    S->send(SCPI::ANALYZER::setfrequencystart(freqstart));
    S->send(SCPI::ANALYZER::setfrequencystop(freqfinish));
    S->send(SCPI::ANALYZER::getfrequencycenter());
    center = S->enter().toDouble();
    S->send(SCPI::ANALYZER::getspan());
    span = S->enter().toDouble();
    QThread::msleep(100);
    double frqc = center, frqs = span;
    int frqc_t = 0, frqs_t = 0;
    while (frqc > 1000 || frqs > 1000)
    {
        if (frqc > 1000)
        {
            frqc /= 1000;
            frqc_t++;
        }
        if (frqs > 1000)
        {
            frqs /= 1000;
            frqs_t++;
        }
    }
    emit frq_c(frqc);
    emit frq_ct(frqc_t);
    emit frq_s(frqs);
    emit frq_st(frqs_t);
}


void AgilentX::sendfreqcentspec(double freqcenter, double freqspan)
{
    if (center != freqcenter)
    {
        center = freqcenter;
        S->send(SCPI::ANALYZER::setfrequencycenter(center));
        QThread::msleep(10);
        double frq = center;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq /= 1000;
            frq_t++;
        }
        emit frq_c(frq);
        emit frq_ct(frq_t);
    }
    if (span != freqspan)
    {
        span = freqspan;
        S->send(SCPI::ANALYZER::setfrequencyspan(span));
        QThread::msleep(10);
        double frq = span;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq /= 1000;
            frq_t++;
        }
        emit frq_s(span);
        emit frq_st(frq_t);
    }
    
}

void AgilentX::sendfreqcent(double freqcenter)
{
    if (center != freqcenter)
    {
        center = freqcenter;
        S->send(SCPI::ANALYZER::setfrequencycenter(center));
        QThread::msleep(10);
        double frq = center;
        int frq_t = 0;
        while (frq > 1000)
        {
            frq /= 1000;
            frq_t++;
        }
        emit frq_c(frq);
        emit frq_ct(frq_t);
    }
}

void AgilentX::sendrflevel(double rflevel)
{
    if (rf_level != rflevel)
    {
        rf_level = rflevel;
        S->send(SCPI::ANALYZER::setrflevel(rflevel));
        emit rf_lv(rf_level);
    }
}

void AgilentX::sendmark(double freq)
{
    double ans;
    S->send(SCPI::ANALYZER::setmarkertocenter(1));
    if (freq != 0)
        S->send(SCPI::ANALYZER::setmarker(1, freq));
    S->send(SCPI::ANALYZER::getmarkerY(1));
    ans = S->enter().toDouble();
    S->send(SCPI::ANALYZER::offmarker(1));
    emit mark_val(ans);
}

double AgilentX::sendvalue(double freq, bool peak)
{
    double ans;
    if (peak)
        S->send(SCPI::ANALYZER::setmarkertopeak(1));
    else
        S->send(SCPI::ANALYZER::setmarkertocenter(1));
    if (freq != 0)
        S->send(SCPI::ANALYZER::setmarker(1, freq));
    S->send(SCPI::ANALYZER::getmarkerY(1));
    ans = S->enter().toDouble();
    S->send(SCPI::ANALYZER::offmarker(1));
    return ans;
}

double AgilentX::sendpeakfrq()
{
    double ans;
    S->send(SCPI::ANALYZER::setmarkertopeak(1));
    S->send(SCPI::ANALYZER::getmarkerX(1));
    ans = S->enter().toDouble();
    S->send(SCPI::ANALYZER::offmarker(1));
    return ans;
}

void AgilentX::sendtracetype(int type)
{
    switch(type)
    {
    default:
        S->send(SCPI::ANALYZER::valuetracetype(1));
        break;
    case 1:
        S->send(SCPI::ANALYZER::averagetracetype(1));
        QThread::sleep(1);
        break;
    case 2:
        S->send(SCPI::ANALYZER::minhtracetype(1));
        QThread::sleep(1);
        break;
    case 3:
        S->send(SCPI::ANALYZER::maxhtracetype(1));
        QThread::sleep(1);
        break;
    }
    emit tracetype(type);
}

void AgilentX::sendcreatemarker()
{
    S->send(SCPI::ANALYZER::setmarkertocenter(1));
}

void AgilentX::sendpeakmarker()
{
    S->send(SCPI::ANALYZER::setmarkertopeak(1));
}

void AgilentX::sendoffmarker()
{
    S->send(SCPI::ANALYZER::offmarker(1));
}

void AgilentX::Update()
{
    if (S->isConnect())
    {
        S->send(SCPI::getname());
        Name_Info = S->enter();
        S->send(SCPI::ANALYZER::getfrequencycenter());
        center = (S->enter()).toDouble();
        S->send(SCPI::ANALYZER::getspan());
        span = (S->enter()).toDouble();
        S->send(SCPI::ANALYZER::getrflevel());
        rf_level = (S->enter()).toInt();
        double frqc = center, frqs = span;
        int frqc_t = 0, frqs_t = 0;
        while (frqc > 1000 || frqs > 1000)
        {
            if (frqc > 1000)
            {
                frqc /= 1000;
                frqc_t++;
            }
            if (frqs > 1000)
            {
                frqs /= 1000;
                frqs_t++;
            }
        }
        emit UpdateUI(Name_Info, frqc, frqc_t, frqs, frqs_t, rf_level, 0);
    }
}