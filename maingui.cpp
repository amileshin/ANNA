#include "maingui.hpp"

MainGUI::MainGUI(QWidget *parent): QMainWindow(parent), ui(new Ui::MainGUI)
{
    save_folder = "";
    G = new AgilentMXG();
    A = new AgilentX();
    C = new Corvet();
    Alg = new Algorithm(G, A, C);
    ui->setupUi(this);
    GW.push_back(ui->GNL);
    GW.push_back(ui->GFL);
    GW.push_back(ui->GFV);
    GW.push_back(ui->GFVT);
    GW.push_back(ui->GFS);
    GW.push_back(ui->GPL);
    GW.push_back(ui->GPV);
    GW.push_back(ui->GPVT);
    GW.push_back(ui->GPS);
    GW.push_back(ui->GRFL);
    GW.push_back(ui->GRFB);
    CW.push_back(ui->CNL);
    CW.push_back(ui->CFL);
    CW.push_back(ui->CFV);
    CW.push_back(ui->CFVT);
    CW.push_back(ui->CFS);
    CW.push_back(ui->CChB);
    CW.push_back(ui->CAL);
    CW.push_back(ui->CAS);
    AW.push_back(ui->ANL);
    AW.push_back(ui->AFL);
    AW.push_back(ui->AFCL);
    AW.push_back(ui->AFCV);
    AW.push_back(ui->AFCVT);
    AW.push_back(ui->AFSL);
    AW.push_back(ui->AFSV);
    AW.push_back(ui->AFSVT);
    AW.push_back(ui->AFAL);
    AW.push_back(ui->AFAV);
    AW.push_back(ui->AFS);
    AW.push_back(ui->APL);
    AW.push_back(ui->APT);
    AW.push_back(ui->APFL);
    AW.push_back(ui->APFV);
    AW.push_back(ui->APFVT);
    AW.push_back(ui->APV);
    AW.push_back(ui->APVL);
    AW.push_back(ui->ATL);
    AW.push_back(ui->ATT);
    connect(ui->GCB, &QPushButton::clicked, this->G->S, &QWidget::show);
    connect(ui->ACB, &QPushButton::clicked, this->A->S, &QWidget::show);
    connect(ui->CCB, &QPushButton::clicked, this->C->S, &QWidget::show);
    connect(this->G->S, &Connect::Connected, this, &MainGUI::OnGUI);
    connect(this->C->S, &Connect::Connected, this, &MainGUI::OnCUI);
    connect(this->A->S, &Connect::Connected, this, &MainGUI::OnAUI);
    connect(G, &Generator::UpdateUI, this, &MainGUI::UpdateGUI);
    connect(C, &Controller::UpdateUI, this, &MainGUI::UpdateCUI);
    connect(A, &Analyzer::UpdateUI, this, &MainGUI::UpdateAUI);
    connect(this->G->S, &Connect::Disconnected, this, &MainGUI::CleanGUI);
    connect(this->C->S, &Connect::Disconnected, this, &MainGUI::CleanCUI);
    connect(this->A->S, &Connect::Disconnected, this, &MainGUI::CleanAUI);
    connect(ui->GFS, &QPushButton::clicked, this, &MainGUI::setGfreq);
    connect(ui->GPS, &QPushButton::clicked, this, &MainGUI::setGpow);
    connect(ui->GRFB, &QPushButton::clicked, this, &MainGUI::setGRF);
    connect(ui->CChB, &QComboBox::currentTextChanged, this, &MainGUI::setCchannel);
    connect(ui->CFS, &QPushButton::clicked, this, &MainGUI::setCfreq);
    connect(ui->AFS, &QPushButton::clicked, this, &MainGUI::setAdisp);
    connect(ui->APT, &QComboBox::currentTextChanged, this, &MainGUI::switchAuimark);
    connect(ui->ATT, &QComboBox::currentTextChanged, this, &MainGUI::swithtracetype);

    connect(ui->ASB, &QPushButton::clicked, this, &MainGUI::AnalyzeStart);
    connect(this, &MainGUI::StartAnalyze, Alg, &Algorithm::func);
    connect(Alg, &Algorithm::error, this, &MainGUI::AnalyzeError);
    connect(Alg, &Algorithm::finish, this, &MainGUI::AnalyzeFinish);

    connect(ui->actionOpen, &QAction::triggered, this, &MainGUI::openconfig);
    connect(ui->actionSaveCnf, &QAction::triggered, this, &MainGUI::saveresult);
    connect(ui->actionSaveTXT, &QAction::triggered, this, &MainGUI::savetxtresult);
    connect(ui->actionSaveFolder, &QAction::toggled, this, &MainGUI::savefolder);
    connect(ui->actionClean, &QAction::triggered, this, &MainGUI::cleanRes);

    connect(this, &MainGUI::UPDG, this->G, &Generator::Update);
    connect(this, &MainGUI::G_frq, this->G, &Generator::sendfreq);
    connect(this, &MainGUI::G_pow, this->G, &Generator::sendpow);
    connect(this, &MainGUI::G_RF, this->G, &Generator::setRF);
    connect(G, &Generator::UpdateFUI, this->ui->GFV, &QDoubleSpinBox::setValue);
    connect(G, &Generator::UpdateFTUI, this->ui->GFVT, &QComboBox::setCurrentIndex);
    connect(G, &Generator::UpdatePUI, this->ui->GPV, &QDoubleSpinBox::setValue);
    connect(G, &Generator::UpdateRF, this, &MainGUI::switchGRF);

    connect(this, &MainGUI::UPDC, this->C, &Controller::Update);
    connect(this, &MainGUI::C_ch, this->C, &Controller::switchchanel);
    connect(this, &MainGUI::C_frq, this->C, &Controller::sendfreq);
    connect(C, &Controller::frq, this->ui->CFV, &QSpinBox::setValue);

    connect(this, &MainGUI::UPDA, this->A, &Analyzer::Update);
    connect(this, &MainGUI::A_disp, this->A, &Analyzer::sendfreqcentspec);
    connect(this, &MainGUI::A_rflevel, this->A, &Analyzer::sendrflevel);
    connect(this, &MainGUI::A_mark, this->A, &Analyzer::mark_val);
    connect(this, &MainGUI::A_tracetype, this->A, &Analyzer::sendtracetype);
    connect(A, &Analyzer::frq_c, this->ui->AFCV, &QDoubleSpinBox::setValue);
    connect(A, &Analyzer::frq_ct, this->ui->AFCVT, &QComboBox::setCurrentIndex);
    connect(A, &Analyzer::frq_s, this->ui->AFSV, &QDoubleSpinBox::setValue);
    connect(A, &Analyzer::frq_st, this->ui->AFSVT, &QComboBox::setCurrentIndex);
    connect(ui->APVL, &QPushButton::clicked, this, &MainGUI::getAmark);

    this->OffGUI();
    this->OffCUI();
    this->OffAUI();
    this->switchAuimark();
    this->createGraph();
}

void MainGUI::UpdateGUI(QString Name, double frq, int frq_t, double pw, bool RF)
{
    ui->GNL->setText(Name);
    ui->GFV->setValue(frq);
    ui->GFVT->setCurrentIndex(frq_t);
    ui->GPV->setValue(pw);
    ui->GRFB->setText(RF ? "ON" : "OFF");
}

void MainGUI::UpdateCUI(QString Name, double frq, int ch, int A_ch)
{
    ui->CNL->setText(Name);
    ui->CFV->setValue(frq);
    ui->CChB->clear();
    //ui->CAS->clear();
    for (int i = 0; i < A_ch; ++i)
        ui->CChB->addItem(QString::number(i + 1));
    ui->CChB->setCurrentIndex(ch);
}

void MainGUI::UpdateAUI(QString Name, double frqc, double frqc_t, double frqs, double frqs_t, int rf_level, int trace_type)
{
    ui->ANL->setText(Name);
    ui->AFCV->setValue(frqc);
    ui->AFCVT->setCurrentIndex(frqc_t);
    ui->AFSV->setValue(frqs);
    ui->AFSVT->setCurrentIndex(frqs_t);
    ui->AFAV->setValue(rf_level);
    ui->ATT->setCurrentIndex(trace_type);
}

void MainGUI::createGraph()
{
    //ui->AGV->scene()->addText("Hello, World!");
    ui->AGV->setBackgroundBrush(QBrush(QColor(0, 100, 255), Qt::CrossPattern));
    ui->AGV->show();
}

void MainGUI::OnGUI()
{
    for (auto W : GW)
    {
        W->setEnabled(true);
    }
    UPDG();
}

void MainGUI::OnCUI()
{
    for (auto W : CW)
    {
        W->setEnabled(true);
    }
    UPDC();
}

void MainGUI::OnAUI()
{
    for (auto W : AW)
    {
        W->setEnabled(true);
    }
    UPDA();
}

void MainGUI::OffGUI()
{
    for (auto W: GW)
    {
        W->setDisabled(true);
    }
}

void MainGUI::CleanGUI()
{
    this->OffGUI();
    ui->GNL->setText(QString::fromUtf8("None"));
    ui->GFV->setValue(0);
    ui->GFVT->setCurrentIndex(0);
    ui->GPV->setValue(0);
    ui->GRFB->setText(QString::fromUtf8("OFF"));
}

void MainGUI::OffCUI()
{
    for (auto W : CW)
    {
        W->setDisabled(true);
    }
}

void MainGUI::CleanCUI()
{
    this->OffCUI();
    ui->CNL->setText(QString::fromUtf8("None"));
    ui->CFV->setValue(0);
    ui->CChB->clear();
    ui->CAS->clear();
}

void MainGUI::OffAUI()
{
    for (auto W : AW)
    {
        W->setDisabled(true);
    }
}

void MainGUI::CleanAUI()
{
    this->OffAUI();
    ui->ANL->setText(QString::fromUtf8("None"));
    ui->AFCV->setValue(0);
    ui->AFCVT->setCurrentIndex(0);
    ui->AFSV->setValue(0);
    ui->AFSVT->setCurrentIndex(0);
    ui->APV->clear();
    ui->APFV->setValue(0);
    ui->APFVT->setCurrentIndex(0);
    ui->APV->setText("");
    ui->ATT->setCurrentIndex(0);
}

void MainGUI::setGfreq()
{
    emit G_frq(ui->GFV->value() * pow(1000, ui->GFVT->currentIndex()));
}

void MainGUI::setGpow()
{
    emit G_pow(ui->GPV->value());
}

void MainGUI::setGRF()
{
    if (ui->GRFB->text() == "OFF")
        emit G_RF(true);
    else
        emit G_RF(false);
}

void MainGUI::switchGRF(bool RF)
{
    if (RF)
        ui->GRFB->setText("ON");
    else
        ui->GRFB->setText("OFF");
}

void MainGUI::setCchannel()
{
    emit C_ch(ui->CChB->currentIndex());
}

void MainGUI::setCfreq()
{
    emit C_frq(ui->CFV->value());
}

void MainGUI::setAdisp()
{
    emit A_disp(ui->AFCV->value() * pow(1000, ui->AFCVT->currentIndex()), ui->AFSV->value() * pow(1000, ui->AFSVT->currentIndex()));
    emit A_rflevel(ui->AFAV->value());
}

void MainGUI::setAmark()
{
    if (ui->APT->currentIndex())
        emit A_mark(ui->APFV->value() * pow(1000, ui->APFVT->currentIndex()));
    else
        emit A_mark(0);
}

void MainGUI::getAmark(double pw)
{
    ui->APV->setText(QString::number(pw));
}

void MainGUI::swithtracetype()
{
    emit A_tracetype(ui->ATT->currentIndex());
}

void MainGUI::switchAuimark()
{
    ui->APFL->setEnabled(ui->APT->currentIndex());
    ui->APFV->setEnabled(ui->APT->currentIndex());
    ui->APFVT->setEnabled(ui->APT->currentIndex());
}

void MainGUI::AnalyzeStart()
{
    ui->StatusBar->showMessage("Alg Start");
    this->OffGUI();
    this->OffCUI();
    this->OffAUI();
    ui->RTB->clear();
    emit StartAnalyze(&F);
}

void MainGUI::AnalyzeError(QString Err)
{
    this->OnGUI();
    this->OnCUI();
    this->OnAUI();
    ui->RTB->setText(Err);
    ui->StatusBar->showMessage("Alg Erorr");
}

void MainGUI::AnalyzeFinish()
{
    this->OnGUI();
    this->OnCUI();
    this->OnAUI();
    for (PM* I : F.Data)
        ui->RTB->append(I->res_str());
    ui->StatusBar->showMessage("Alg Finish");
}

void MainGUI::openconfig()
{
    QString FileName = QFileDialog::getOpenFileName(this);
    if (F.fromfile(FileName.toStdString()))
    {
        ui->CTB->setText(F.out());
        this->cleanRes();
        if (F.result)
            for (PM* I : F.Data)
                ui->RTB->append(I->res_str());
    }
}

void MainGUI::saveresult()
{
    QString FileName;
    if (ui->actionSaveFolder->isChecked())
        FileName = save_folder + "/" + QString::number(ui->MSV->value()) + "_" + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
    else
        FileName = QFileDialog::getOpenFileName(this, tr("Open file"));
    if (!F.tofile(FileName.toStdString()))
        ui->StatusBar->showMessage("Wrong file to save result");
    else
        ui->StatusBar->showMessage("Result save at " + FileName, 1000);
    if (ui->actionChange_Serial_Number->isChecked())
        ui->MSV->setValue(ui->MSV->value() + 1);
}

void MainGUI::savetxtresult()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open text file"));
    if (!F.totxtfile(FileName.toStdString()))
        ui->StatusBar->showMessage("Wrong file to save result", 1000);
    else
        ui->StatusBar->showMessage("Result save at " + FileName, 1000);
}

void MainGUI::savefolder()
{
    if (!ui->actionSaveFolder->isChecked())
    {
        save_folder = "";
        ui->StatusBar->clearMessage();
    }
    else
    {
        save_folder = QFileDialog::getExistingDirectory(this, tr("Open Folder"));
        ui->StatusBar->showMessage("Result save at folder " + save_folder);
    }
}

void MainGUI::cleanRes()
{
    ui->RTB->clear();
}

MainGUI::~MainGUI()
{
    GW.clear();
    AW.clear();
    CW.clear();
    delete Alg;
    delete ui;
}

