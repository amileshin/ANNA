#ifndef SCPI_HPP
#define SCPI_HPP

#include <QString>

namespace SCPI
{
    //MainCommand
    static QString getname(){return "*idn?\n";}
    static QString setnormal(){return "*rst\n";}
    static QString save(){return "*sav\n";}
    static QString abort(){return "abor\n";}
    static QString initiate(){return "init\n";}

    namespace GENERATOR
    {
        //ON/OFF block
        static QString ON(){return "outp on\n";}
        static QString OFF(){return "outp off\n";}
        static QString upfrequency(){return "freq up\n";}
        static QString downfrequency(){return "freq down\n";}
        static QString uppower(){return "pow up\n";}
        static QString downpower(){return "pow down\n";}

        //ASK block
        static QString getfrequency(){return "freq?\n";}
        static QString getpower(){return "pow?\n";}
        static QString getoutput(){return "outp?\n";}
        static QString getfrequencystep(){return "freq:step?\n";}
        static QString getpowerstep(){return "pow:step?\n";}

        static QString getcommunicationIP(){return "syst:comm:lan:ip?\n";}
        static QString getcommunicationport(){return "syst:comm:lan:port?\n";}
        static QString getcommunicationbaud(){return "syst:comm:serial:baud?\n";}

        //SET block
        static QString setfrequency(double freq){return "freq " + QString::number(freq, 'f') + " hz\n";}
        static QString setpower(double pow){return "pow " + QString::number(pow, 'f') + " dbm\n";}
        static QString setfrequencystep(double freq){return "freq:step " + QString::number(freq, 'f') + " hz\n";}
        static QString setpowerstep(double pow){return "freq:step " + QString::number(pow, 'f') + " dbm\n";}

        static QString setlanIP(QString IP){return "syst:comm:lan:ip " + IP + "\n";}
        static QString setlanport(int port){return "syst:comm:lan:port " + QString::number(port) + "\n";}
        static QString setserialbaud(int baud){return "syst:comm:ser:baud " + QString::number(baud) + "\n";}
    };

    namespace ANALYZER
    {
        //ON/OFF block
        static QString ontraceupd(int numb){return "trac" + QString::number(numb) + ":upd 1\n";}
        static QString offtraceupd(int numb){return "trac" + QString::number(numb) + ":upd 0\n";}
        static QString averagetracetype(int numb){return "trac" + QString::number(numb) + ":type aver\n";}
        static QString maxhtracetype(int numb){return "trac" + QString::number(numb) + ":type maxh\n";}
        static QString minhtracetype(int numb){return "trac" + QString::number(numb) + ":type minh\n";}
        static QString valuetracetype(int numb){return "trac" + QString::number(numb) + ":type writ\n";}
        static QString offmarker(int numb){return "calc:mark" + QString::number(numb) + ":mode off\n";}

        //ASK block
        static QString getfrequencycenter(){return "freq:cent?\n";}
        static QString getfrequencystart(){return "freq:start?\n";}
        static QString getfrequencystop(){return "freq:stop?\n";}
        static QString getspan(){return "freq:span?\n";}
        static QString getrflevel(){return "disp:wind:trac:y:rlev?\n";}
        static QString getmarkerX(int numb){return "calc:mark" + QString::number(numb) + ":x?\n";}
        static QString getmarkerY(int numb){return "calc:mark" + QString::number(numb) + ":y?\n";}
        static QString gettraceupd(int numb){return "trac" + QString::number(numb) + ":upd?\n";}
        static QString getdatatrace(int numb){return "trac:data? trac" + QString::number(numb) + '\n';}


        //SET block
        static QString setfrequencycenter(double freq){return "freq:cent " + QString::number(freq) + " hz\n";}
        static QString setfrequencystart(double freq){return "freq:start " + QString::number(freq) + " hz\n";}
        static QString setfrequencystop(double freq){return "freq:stop " + QString::number(freq) + " hz\n";}
        static QString setfrequencyspan(double freq){return "freq:span " + QString::number(freq) + " hz\n";}
        static QString setrflevel(double pow){return "disp:wind:trac:y:rlev " + QString::number(pow) + "\n";}
        static QString setmarkertocenter(int numb){return "calc:mark" + QString::number(numb) + ":cent\n";}
        static QString setmarkertopeak(int numb) { return "calc:mark" + QString::number(numb) + ":max\n"; }
        static QString setmarker(int numb, double freq){return "calc:mark" + QString::number(numb) + ":x " + QString::number(freq) + " hz\n";}
    };
};

#endif // SCPI_HPP
