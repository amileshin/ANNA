#ifndef MAINGUI_H
#define MAINGUI_H

#include <QMainWindow>
#include <list>
#include <QPainter>
#include <QThread>
#include "generator.hpp"
#include "analyzer.hpp"
#include "controller.hpp"
#include "algorithm.hpp"
#include "ui_maingui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainGUI; }
QT_END_NAMESPACE

class MainGUI : public QMainWindow
{
    Q_OBJECT
private:
    QThread* Second;

    Ui::MainGUI *ui;
    Generator *G;
    Analyzer *A;
    Controller *C;
    Algorithm *Alg;
    std::list <QWidget*> CW;
    std::list <QWidget*> GW;
    std::list <QWidget*> AW;

    QString save_folder;

    Config F;

public:
    MainGUI(QWidget *parent = nullptr);
    ~MainGUI();

signals:
    void UPDG();
    void UPDC();
    void UPDA();
    void StartAnalyze(Config* F);
    void G_frq(double frq);
    void G_pow(double pow);
    void G_RF(bool rf);
    void C_ch(int ch);
    void C_frq(int frq);
    //void C_atune(int i);
    void A_disp(double frqc, double frqs);
    void A_rflevel(double rf_level);
    void A_mark(double frq);
    void A_tracetype(int type);

private slots:
    void UpdateGUI(QString Name, double frq, int frq_t, double pw, bool RF);
    void UpdateCUI(QString Name, double frq, int ch, int A_ch);
    void UpdateAUI(QString Name, double frqc, double frqc_t, double frqs, double frqs_t, int rf_level, int trace_type);
    void OnGUI();
    void OnCUI();
    void OnAUI();
    void OffGUI();
    void OffCUI();
    void OffAUI();
    void CleanGUI();
    void CleanCUI();
    void CleanAUI();
    void setGfreq();
    void setGpow();
    void setGRF();
    void switchGRF(bool RF);
    void setCchannel();
    void setCfreq();
    //void setCatune();
    void setAdisp();
    void setAmark();
    void getAmark(double pow);
    void swithtracetype();
    void AnalyzeStart();
    void AnalyzeError(QString Err);
    void AnalyzeFinish();
    void openconfig();
    void saveresult();
    void savetxtresult();
    void cleanRes();
    void createGraph();
    void savefolder();
    void switchAuimark();
};
#endif // MAINGUI_H
