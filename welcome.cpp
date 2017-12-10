#include "novaembed.h"
#include "ui_novaembed.h"
#include <QFileDialog>
#include <QTreeView>
#include <QDebug>
#include <QDir>
#include <QStatusBar>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QUrl>
#include <QtCore>
#include <QDesktopServices>
//#include <QHostInfo>
#include <QDirIterator>

void NOVAembed::on_P_HUM_pushButton_clicked()
{
    system("qpdfview /Devel/NOVAsom_SDK/Doc/NI150316-HUM-P-V1.0.pdf");
}


void NOVAembed::on_P_QSG_pushButton_clicked()
{
    system("qpdfview /Devel/NOVAsom_SDK/Doc/NI150316-QSG-P-V1.0.pdf");

}

void NOVAembed::on_P_HUM_pushButton_3_clicked()
{
    system("qpdfview /Devel/NOVAsom_SDK/Doc/SUM-NOVAembed-V1.0.pdf");
}
