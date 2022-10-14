#include "algorithm.hpp"

Algorithm::Algorithm(Generator *Gp, Analyzer *Ap, Controller *Cp)
{
    G = Gp;
    A = Ap;
    C = Cp;
}

void Algorithm::func(Config* F)
{
    if (!G->S->isConnect() || !C->S->isConnect() || !A->S->isConnect())
        emit error("Not all components connected\n");
    if (F->empty())
        emit error("Config is Empty\n");
    QString Ans = "";
    for (PM* I : F->Data)
    {
        I->Metric(G, A, C);
        Ans += I->res_str() + '\n';
    }
    F->result = true;
    emit finish();
}
