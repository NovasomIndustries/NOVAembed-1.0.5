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
#include <QDirIterator>


extern  QString FileSystemName;
extern  QString DeployedFileSystemName;
extern  QString FileSystemConfigName;
extern  QString CfgBitDefaultValue;
extern  QString _Board_comboBox;
extern  QString Last_M8_BSPFactoryFile;
extern  QString LVDSVideo;
extern  QString Quad;

//QString FileNameNoExtension;


QString M8_getvalue(QString strKey, QSettings *settings , QString entry)
{
    return settings->value( strKey + entry, "r").toString();
}

void NOVAembed::M8_load_BSPF_File(QString fileName)
{
QString strKeyFunc("M8_IOMUX/");
QSettings * func_settings = 0;

    on_M8_Clear_pushButton_clicked();

    Last_M8_BSPFactoryFile = fileName;

    func_settings = new QSettings( fileName, QSettings::IniFormat );
}


void NOVAembed::M8_save_helper(QString fileName)
{
    QFileInfo fin(fileName);
    QString bspfbase = fin.baseName();
    QString fullpathname = "";

    fullpathname = "/Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf/"+ bspfbase+".bspf";

    QFile file(fullpathname);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Unable to open file "),file.errorString());
        return;
    }

    QTextStream out(&file);
    out << QString("[M8_IOMUX]\n");
    out << QString("\n[M8_CONF]\n");
    file.close();
    update_status_bar("File "+Last_M8_BSPFactoryFile+" saved");
    Last_M8_BSPFactoryFile = fileName;
    storeNOVAembed_ini();

}

void NOVAembed::on_M8_Save_pushButton_clicked()
{
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save .bspf"), "/Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf",tr(".bspf (*.bspf)"));
        if ( fileName.isEmpty() )
            return;
        QFileInfo fi(fileName);
        ui->M8_Current_BSPF_File_label->setText(fi.baseName()+".bspf");
        ui->M8_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
        M8_save_helper(fileName);
}




void NOVAembed::on_M8_Generate_pushButton_clicked()
{
    if ( CheckIfKernelsPresent() == 1 )
        return;
    // Save .bspf and Generate .dtb
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save .bspf"), "/Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf",tr(".bspf (*.bspf)"));
    if ( fileName.isEmpty() )
        return;

    QFile scriptfile("/tmp/script");
    QFileInfo fi(fileName);
    ui->M8_Current_BSPF_File_label->setText(fi.baseName()+".bspf");
    ui->M8_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
    M8_save_helper(fileName);
    Last_M8_BSPFactoryFile = fileName;
    QString FileNameNoExtension  = fi.baseName();

    update_status_bar("Generating dtb "+FileNameNoExtension+".dtb ...");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    if ( ui->M8_EditBeforeGenerate_checkBox->isChecked())
        update_status_bar("User editing dtsi file");

    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("[ ! -d /Devel/NOVAsom_SDK/DtbUserWorkArea ] && mkdir /Devel/NOVAsom_SDK/DtbUserWorkArea\n");
    out << QString("cd /Devel/NOVAsom_SDK/Utils\n");
    out << QString("/Devel/NOVAsom_SDK/Qt/NOVAembed/NOVAembed_M8_Parser/bin/Debug/NOVAembed_M8_Parser /Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf/"+FileNameNoExtension+".bspf > /Devel/NOVAsom_SDK/Logs/M8_bspf.log\n");
    if ( ui->M8_EditBeforeGenerate_checkBox->isChecked())
        out << QString("kwrite /Devel/NOVAsom_SDK/DtbUserWorkArea/"+FileNameNoExtension+".dts\n");
    out << QString("./user_dtb_compile "+FileNameNoExtension+" M8 >> /Devel/NOVAsom_SDK/Logs/M8_bspf.log\n");

    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar(fi.baseName()+".dtb compiled, dtb is in /Devel/NOVAsom_SDK/DtbUserWorkArea folder as "+FileNameNoExtension+".dtb");
        const char *str;
        QByteArray ba;

        QString syscmd_quad = "cp /Devel/NOVAsom_SDK/DtbUserWorkArea/"+FileNameNoExtension+".dtb /Devel/NOVAsom_SDK/Deploy/novasomm8.dtb ; chmod 777 /Devel/NOVAsom_SDK/Deploy/novasomm8.dtb";
        ba = syscmd_quad.toLatin1();
        str = ba.data();
        system(str);
        //NOVAsom_Params_helper();
    }
    else
        update_status_bar("Error compiling "+fi.baseName()+".dtb");
    storeNOVAembed_ini();
}

/*****************************************************************************************************************************************************************************************/
/*                                                                             M8 BSP Factory                                                                                             */
/*****************************************************************************************************************************************************************************************/



void NOVAembed::on_M8_SPI1_checkBox_toggled(bool checked)
{

}



void NOVAembed::on_M8_I2C2_checkBox_toggled(bool checked)
{

}



void NOVAembed::on_M8_UART2_checkBox_toggled(bool checked)
{

}


void NOVAembed::on_M8_UART2_4WirescheckBox_toggled(bool checked)
{

}



void NOVAembed::on_M8_PWM2_checkBox_toggled(bool checked)
{

}

void NOVAembed::on_M8_Clear_pushButton_clicked()
{
    on_M8_SPI1_checkBox_toggled(false);
    on_M8_I2C2_checkBox_toggled(false);
    on_M8_UART2_checkBox_toggled(false);
    on_M8_UART2_4WirescheckBox_toggled(false);
    on_M8_PWM2_checkBox_toggled(false);
    ui->M8_PrimaryVideo_24bit_comboBox->setChecked(false);
}


void NOVAembed::on_M8_Load_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Load BSP Factory File"), "/Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf",tr("BSP Factory Files (*.bspf)"));
    if (fileName.isEmpty())
        return;
    else
    {
        M8_load_BSPF_File(fileName);
        update_status_bar("File "+Last_M8_BSPFactoryFile+" loaded");
        QFileInfo fi(fileName);
        ui->M8_Current_BSPF_File_label->setText(fi.baseName()+".bspf");
        ui->M8_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
        Last_M8_BSPFactoryFile = fileName;
        storeNOVAembed_ini();
    }
}
