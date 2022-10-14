#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <list>
#include <fstream>
#include <string>
#include <QString>
#include <QStringList>
#include "dm.hpp"

class Config: public QObject
{
    Q_OBJECT
private:
    bool fromfile_nr(std::ifstream* FIN);
    bool fromfile_vr(std::ifstream* FIN);

public:
    bool result;
    std::list <PM*> Data;
    Config(const Config &A);
    Config(std::string FileName = "");
    ~Config();
    bool empty() const;
    bool fromfile(std::string FileName);
    bool tofile(std::string FileName) const;
    bool totxtfile(std::string FileName) const;
    QString out(int filter = -1) const;
    void clear();

    Config& operator=(const Config& A);

    friend class Algorithm;
};

#endif // CONFIG_HPP
