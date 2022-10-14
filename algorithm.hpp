#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <QObject>
#include <cmath>
#include <fstream>
#include "windows.h"
#include "analyzer.hpp"
#include "generator.hpp"
#include "controller.hpp"
#include "config.hpp"
#include "dm.hpp"

class Algorithm: public QObject
{
    Q_OBJECT
private:
    Generator *G;
    Analyzer *A;
    Controller *C;

public:
    Algorithm(Generator *Gp, Analyzer *Ap, Controller *Cp);

public slots:
    void func(Config* F);

signals:
    void finish();
    void error(QString Error);

    friend class MainGUI;
};

#endif // ALGORITHM_HPP
