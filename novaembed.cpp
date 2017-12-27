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
#include <QMovie>

/*****************************************************************************************************************************************************************************************/
/*                                                                              Global variables                                                                                         */
/*****************************************************************************************************************************************************************************************/

QString Version = "1.0.5.0rc3";
QString Configuration = "Standard";
QString FileSystemName = "";
QString DeployedFileSystemName = "";
QString FileSystemConfigName = "";
QString _Board_comboBox = "";
QString Last_M8_BSPFactoryFile = "";
QString Last_U_BSPFactoryFile = "";
QString Last_P_BSPFactoryFile = "";
QString CfgBitDefaultValueDefault = "0x4001b8b1";
QString CfgBitDefaultValue = "0x4001b8b1";
QString NumberOfUserPartitions = "-";
QString UserPartition1Size = "1";
QString UserPartition2Size = "1";
QString uSD_Device = "sdb";
QString CurrentBSPF_Tab = "";
QString CurrentPrimaryVideo = "";
QString CurrentSecondaryVideo = "";
QString AutoRunSelected = "";
QString AutoRunFolder = "";
QString BootValid = "INVALID";
QString FSValid = "INVALID";
QString KernelValid = "INVALID";
QString CurrentDevelopment = "Stable";
QString uSDwriteValid = "INVALID";
QString CurrentSplashName =  "NOVAsomP800x480";
QString Kernel =  "linux-imx_4.1.15_1.2.0_ga";
QString SourceMeFile =  "SourceMe32_5";


extern  void storeNOVAembed_ini();
QWidget *PBSP_stab,*UBSP_stab,*SBSP_stab,*TOOL_stab,*TOOL_devel;

/*****************************************************************************************************************************************************************************************/
/*                                                                                    Code                                                                                               */
/*****************************************************************************************************************************************************************************************/

NOVAembed::NOVAembed(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NOVAembed)
{
int     copy_required_files = 0;
    qDebug() << "Starting";

    /* Initialize user area */
    if ( ! QDir("/Devel/NOVAsom_SDK/Logs").exists() )
        system("mkdir -p /Devel/NOVAsom_SDK/Logs");
    if ( ! QDir("/Devel/NOVAsom_SDK/Deploy").exists() )
        system("mkdir -p /Devel/NOVAsom_SDK/Deploy");
    if ( ! QDir("/Devel/NOVAsom_SDK/DtbUserWorkArea").exists() )
    {
        QMessageBox::information(this, tr("DtbUserWorkArea"),"DtbUserWorkArea not found. Creating a new one!");
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/PClass_bspf/temp");
        copy_required_files = 1;
    }
    if ( ! QDir("/Devel/NOVAsom_SDK/DtbUserWorkArea/PClass_bspf").exists() )
    {
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/PClass_bspf/temp");
        copy_required_files = 1;
    }
    if ( ! QDir("/Devel/NOVAsom_SDK/DtbUserWorkArea/UClass_bspf").exists() )
    {
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/UClass_bspf/temp");
        copy_required_files = 1;
    }

    if ( copy_required_files == 1 )
    {
        system("cp /Devel/NOVAsom_SDK/Utils/DtbBaseFiles/* /Devel/NOVAsom_SDK/DtbUserWorkArea/.");
    }
    QString fileName = "/Devel/NOVAsom_SDK/NOVAembed_Settings/NOVAembed.ini";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        //qDebug() << "NOVAembed.ini not found !!";
        QMessageBox::information(this, tr("NOVAembed.ini"),"NOVAembed.ini not found. Creating a new one!");
        if ( ! QDir("/Devel/NOVAsom_SDK/NOVAembed_Settings").exists() )
        {
            //qDebug() << "mkdir /Devel/NOVAsom_SDK/NOVAembed_Settings";
            QDir().mkdir("/Devel/NOVAsom_SDK/NOVAembed_Settings");
        }
        storeNOVAembed_ini();
    }
    else
    {
        QString strKeyConf("NOVAembed Configuration/");
        QSettings * config = 0;
        config = new QSettings( fileName, QSettings::IniFormat );
        Configuration = config->value( strKeyConf + "Configuration", "r").toString();
        if ( Configuration !=  Version )
            storeNOVAembed_ini();
        QString strKeySettings("NOVAembed General Settings/");
        QSettings * settings = 0;
        settings = new QSettings( fileName, QSettings::IniFormat );
        FileSystemName = settings->value( strKeySettings + "FileSystemName", "r").toString();
        DeployedFileSystemName = settings->value( strKeySettings + "DeployedFileSystemName", "r").toString();
        FileSystemConfigName = settings->value( strKeySettings + "FileSystemConfigName", "r").toString();
        _Board_comboBox = settings->value( strKeySettings + "Board_comboBox", "r").toString();
        Last_M8_BSPFactoryFile = settings->value( strKeySettings + "Last_M8_BSPFactoryFile", "r").toString();
        Last_P_BSPFactoryFile = settings->value( strKeySettings + "Last_P_BSPFactoryFile", "r").toString();
        Last_U_BSPFactoryFile = settings->value( strKeySettings + "Last_U_BSPFactoryFile", "r").toString();
        CfgBitDefaultValue = settings->value( strKeySettings + "CfgBitDefaultValue", "r").toString();
        if ( CfgBitDefaultValue == "r" )
             CfgBitDefaultValue = CfgBitDefaultValueDefault;
        NumberOfUserPartitions = settings->value( strKeySettings + "NumberOfUserPartitions", "r").toString();
        UserPartition1Size = settings->value( strKeySettings + "UserPartition1Size", "r").toString();
        UserPartition2Size = settings->value( strKeySettings + "UserPartition2Size", "r").toString();
        uSD_Device = settings->value( strKeySettings + "uSD_Device", "r").toString();
        CurrentBSPF_Tab = settings->value( strKeySettings + "CurrentBSPF_Tab", "r").toString();
        CurrentPrimaryVideo = settings->value( strKeySettings + "CurrentPrimaryVideo", "r").toString();
        CurrentSecondaryVideo = settings->value( strKeySettings + "CurrentSecondaryVideo", "r").toString();
        AutoRunSelected = settings->value( strKeySettings + "AutoRunSelected", "r").toString();
        AutoRunFolder = settings->value( strKeySettings + "AutoRunFolder", "r").toString();
        BootValid = settings->value( strKeySettings + "BootValid", "r").toString();
        FSValid = settings->value( strKeySettings + "FSValid", "r").toString();
        KernelValid = settings->value( strKeySettings + "KernelValid", "r").toString();
        CurrentSplashName = settings->value( strKeySettings + "CurrentSplashName", "r").toString();
        Kernel = settings->value( strKeySettings + "Kernel", "r").toString();
        CurrentDevelopment = settings->value( strKeySettings + "CurrentDevelopment", "r").toString();
        if ( _Board_comboBox == "P Series")
        {
            Kernel="linux-imx_4.1.15_1.2.0_ga";
            SourceMeFile="SourceMe32_5";
        }
        if ( _Board_comboBox == "M8")
        {
            Kernel="linux-4.11.0-QualcommLinaro";
            SourceMeFile="SourceMe32_6";
        }
        if ( _Board_comboBox == "U5")
        {
            Kernel="linux-imx_4.1.43";
            SourceMeFile="SourceMe32_5";
        }
    }
    if ( ! QDir("/Devel/NOVAsom_SDK/NOVAembed_Settings/PClass_bspf").exists() )
    {
        QDir().mkdir("/Devel/NOVAsom_SDK/NOVAembed_Settings/PClass_bspf");
    }
    if ( ! QDir("/Devel/NOVAsom_SDK/NOVAembed_Settings/SClass_bspf").exists() )
    {
        QDir().mkdir("/Devel/NOVAsom_SDK/NOVAembed_Settings/SClass_bspf");
    }
    if ( ! QDir("/Devel/NOVAsom_SDK/NOVAembed_Settings/UClass_bspf").exists() )
    {
        QDir().mkdir("/Devel/NOVAsom_SDK/NOVAembed_Settings/UClass_bspf");
    }

    ui->setupUi(this);

    QMovie *movie = new QMovie(":/Icons/animation.gif");
    ui->movie_label->setMovie(movie);
    movie->start();

    ui->linklabel->setOpenExternalLinks(true);
    ui->linklabel->setText("<a href=\"http://www.novasomindustries.com\">Novasom Industries</a>");

    ui->FileSystemSelectedlineEdit->setText(FileSystemName);
    ui->UserPartition_comboBox->setCurrentText(NumberOfUserPartitions);
    ui->VersionLabel->setText(Version);
    PBSP_stab=ui->tabBSPFP;
    SBSP_stab=ui->tabBSPFS;
    UBSP_stab=ui->tabBSPFU;
    TOOL_stab=ui->tabTools;
    ui->tab->removeTab(4);
    ui->tab->removeTab(3);
    ui->tab->removeTab(2);

    if (CurrentBSPF_Tab == "P BSP Factory")
    {
        ui->tab->insertTab(2,PBSP_stab,"P BSP Factory");
    }
    else if (CurrentBSPF_Tab == "S BSP Factory")
    {
        ui->tab->insertTab(2,SBSP_stab,"S BSP Factory");
    }
    else if (CurrentBSPF_Tab == "U BSP Factory")
    {
        ui->tab->insertTab(2,UBSP_stab,"U BSP Factory");
    }
    else
    {
        ui->tab->insertTab(2,PBSP_stab,"P BSP Factory");
        CurrentBSPF_Tab = "P BSP Factory";
    }
    ui->tab->insertTab(3,TOOL_stab,"Tools");
}

NOVAembed::~NOVAembed()
{
    delete ui;
}

void NOVAembed::on_actionExit_triggered()
{
    qApp->exit();
}



/*****************************************************************************************************************************************************************************************/
/*                                                                              Helper Functions                                                                                         */
/*****************************************************************************************************************************************************************************************/
/*
    qDebug() << "Hello world.";
    qWarning() << "Uh, oh...";
    qCritical() << "Oh, noes!";
    qFatal( "AAAAAAAAAH!" );
*/

void NOVAembed::storeNOVAembed_ini()
{
    QString fileName = "/Devel/NOVAsom_SDK/NOVAembed_Settings/NOVAembed.ini";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("NOVAembed.ini"),"Unable to create NOVAembed.ini");
        return;
    }
    QTextStream out(&file);
    out << QString("[NOVAembed Configuration]\n");
    out << QString("Configuration="+Version+"\n");
    out << QString("[NOVAembed General Settings]\n");
    out << QString("FileSystemName="+FileSystemName+"\n");
    out << QString("DeployedFileSystemName="+DeployedFileSystemName+"\n");
    out << QString("FileSystemConfigName="+FileSystemConfigName+"\n");
    out << QString("Board_comboBox="+_Board_comboBox+"\n");
    out << QString("Last_M8_BSPFactoryFile="+Last_M8_BSPFactoryFile+"\n");
    out << QString("Last_P_BSPFactoryFile="+Last_P_BSPFactoryFile+"\n");
    out << QString("Last_U_BSPFactoryFile="+Last_U_BSPFactoryFile+"\n");
    out << QString("CfgBitDefaultValue="+CfgBitDefaultValue+"\n");
    out << QString("NumberOfUserPartitions="+NumberOfUserPartitions+"\n");
    out << QString("UserPartition1Size="+UserPartition1Size+"\n");
    out << QString("UserPartition2Size="+UserPartition2Size+"\n");
    out << QString("uSD_Device="+uSD_Device+"\n");
    out << QString("AutoRunSelected="+AutoRunSelected+"\n");
    out << QString("AutoRunFolder="+AutoRunFolder+"\n");
    out << QString("CurrentBSPF_Tab="+CurrentBSPF_Tab+"\n");
    out << QString("CurrentPrimaryVideo="+CurrentPrimaryVideo+"\n");
    out << QString("CurrentSecondaryVideo="+CurrentSecondaryVideo+"\n");
    out << QString("BootValid="+BootValid+"\n");
    out << QString("FSValid="+FSValid+"\n");
    out << QString("KernelValid="+KernelValid+"\n");
    out << QString("CurrentSplashName="+CurrentSplashName+"\n");
    out << QString("Kernel="+Kernel+"\n");
    out << QString("CurrentDevelopment="+CurrentDevelopment+"\n");

    file.close();
}

void NOVAembed::compile_NewFileSystemFileSystemConfigurationcomboBox()
{
    QString str;
    QDir BuildrootDir("/Devel/NOVAsom_SDK/Utils/Configurations");
    if (ui->Board_comboBox->currentText() == "P Series")
    {
        str = "PClass_Buildroot_*.config";
        ui->PrimaryVideo_comboBox->setVisible(true);
        ui->VideoVisible_label->setVisible(true);
    }
    else if (ui->Board_comboBox->currentText() == "M8")
    {
        str = "M8Class_Buildroot_*.config";
        ui->PrimaryVideo_comboBox->setVisible(true);
        ui->VideoVisible_label->setVisible(true);
    }
    else if (ui->Board_comboBox->currentText() == "U5")
    {
        str = "U5Class_Buildroot_*.config";
        ui->PrimaryVideo_comboBox->setVisible(false);
        ui->VideoVisible_label->setVisible(false);
    }
    else
        str = "Buildroot_*.config";

    QStringList BuildrootnameFilter(str);
    QStringList BuildrootfilesList = BuildrootDir.entryList(BuildrootnameFilter);

    ui->NewFileSystemConfigurationcomboBox->clear();
    for(int i=0;i<BuildrootfilesList.count();i++)
    {
        str = BuildrootfilesList[i];
        /*qDebug() << str;*/
        ui->NewFileSystemConfigurationcomboBox->addItem(str);
    }
    ui->NewFileSystemConfigurationcomboBox->setCurrentIndex(1);
}

void NOVAembed::compile_ExtFS_comboBox()
{
    QString str;
    QDir ExternalFileSystemsDir("/Devel/NOVAsom_SDK/ExternalFileSystems");
    str = "*";

    QStringList ExternalFileSystemsnameFilter(str);
    QStringList ExternalFileSystemsfilesList = ExternalFileSystemsDir.entryList(ExternalFileSystemsnameFilter);

    for(int i=0;i<ExternalFileSystemsfilesList.count();i++)
        ui->ExtFS_comboBox->removeItem(i);
    for(int i=0;i<ExternalFileSystemsfilesList.count();i++)
    {
        str = ExternalFileSystemsfilesList[i];
        if (( str != ".") && (str != ".."))
        {
            //qDebug() << str;
            ui->ExtFS_comboBox->addItem(str);
        }
    }
    ui->ExtFS_comboBox->setCurrentIndex(0);
    QPixmap fspixmap (":/Icons/"+ui->ExtFS_comboBox->currentText()+".png");
    ui->FileSystemLogo->setPixmap(fspixmap);
}

void NOVAembed:: local_sleep(int ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

int NOVAembed::run_script(void)
{
    this->setCursor(Qt::WaitCursor);

    system("rm -f /tmp/result");
    system("chmod 777 /tmp/script");
    system("konsole -e /tmp/script > /Devel/NOVAsom_SDK/Logs/main_log");

    QFile file("/tmp/result");
    while( file.open(QIODevice::ReadOnly) == false )
        local_sleep(100);
    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();
    content.chop(1);
    this->setCursor(Qt::ArrowCursor);
    return content.toInt();
}

int NOVAembed::update_status_bar(QString StatusBarContent)
{
    ui->statusBar->showMessage(StatusBarContent);
    return 0;
}

/*****************************************************************************************************************************************************************************************/
/*                                                                          Helper Functions End                                                                                         */
/*****************************************************************************************************************************************************************************************/

/*****************************************************************************************************************************************************************************************/
/*                                                                           Top tab switches                                                                                            */
/*****************************************************************************************************************************************************************************************/
void NOVAembed::on_tab_currentChanged(int index)
{
    switch ( index)
    {
    case 0 : // Welcome Tab
        update_status_bar("Welcome");
        break;
    case 1 : // BKF Tab
        /* File system config files */

        ui->Board_comboBox->setCurrentText(_Board_comboBox);
        ui->UserPartition_comboBox->setCurrentText(NumberOfUserPartitions);
        ui->SplashImageNameLabel->setText(CurrentSplashName+".png");
        ui->SplashThumb->setPixmap(QPixmap("/Devel/NOVAsom_SDK/Utils/LinuxSplashLogos/"+CurrentSplashName+".png") );

        compile_NewFileSystemFileSystemConfigurationcomboBox();
        compile_ExtFS_comboBox();
        on_ThisIsReferenceServer_checkBox_clicked(true);
        initrd_helper();

        ui->iperror_label->setVisible(false);
        ui->REFERENCE_SERVER_label->setEnabled(false);
        ui->REFERENCE_SERVER_lineEdit->setEnabled(false);
        ui->ThisIsReferenceServer_checkBox->setChecked(true);
        if ( BootValid == "OK")
        {
            ui->uboot_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
        }
        else
        {
            ui->uboot_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
        }
        if ( FSValid == "OK")
        {
            ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
        }
        else
        {
            ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
        }
        if ( KernelValid == "OK")
        {
            ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
        }
        else
        {
            ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
        }
        if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
            ui->frame_5->setEnabled(false);
        else
            ui->frame_5->setEnabled(true);


        ui->VideoVisible_label->setVisible(true);
        ui->VideoVisible_label_2->setVisible(true);
        ui->PrimaryVideo_comboBox->setVisible(true);
        ui->SecondaryVideo_comboBox->setVisible(true);
        ui->PriVideo_24bit_checkBox->setVisible(true);
        ui->SecVideo_24bit_checkBox->setVisible(true);
        ui->label_61->setVisible(true);
        ui->UserBSPFSelect_pushButton->setVisible(true);
        ui->UserBSPFselectedlineEdit->setVisible(true);

        if ( ui->Board_comboBox->currentText() == "M8")
        {
            Kernel="linux-4.11.0-QualcommLinaro";
            SourceMeFile="SourceMe32_6";
            if ( Last_M8_BSPFactoryFile.length() < 2)
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
            else
                ui->UserBSPFselectedlineEdit->setText(Last_M8_BSPFactoryFile);
            ui->PreCompiledFileSystem_frame->setVisible(true);
        }
        if ( ui->Board_comboBox->currentText() == "P Series")
        {
            Kernel="linux-4.11.0-QualcommLinaro";
            SourceMeFile="SourceMe32_6";
            if ( Last_P_BSPFactoryFile.length() < 2)
            {
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
                Last_P_BSPFactoryFile = "/Devel/NOVAsom_SDK/DtbUserWorkArea/PClass_bspf/uninitialized.bspf";
            }
            else
            {
                P_load_BSPF_File(Last_P_BSPFactoryFile);
                ui->UserBSPFselectedlineEdit->setText(Last_P_BSPFactoryFile);
            }
            ui->PreCompiledFileSystem_frame->setVisible(true);
        }
        if ( ui->Board_comboBox->currentText() == "U5")
        {
            Kernel="linux-imx_4.1.43";
            SourceMeFile="SourceMe32_5";
            if ( Last_U_BSPFactoryFile.length() < 2)
            {
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
                Last_U_BSPFactoryFile = "/Devel/NOVAsom_SDK/DtbUserWorkArea/UClass_bspf/uninitialized.bspf";
            }
            else
                ui->UserBSPFselectedlineEdit->setText(Last_U_BSPFactoryFile);

            ui->VideoVisible_label->setVisible(false);
            ui->VideoVisible_label_2->setVisible(false);
            ui->PrimaryVideo_comboBox->setVisible(false);
            ui->SecondaryVideo_comboBox->setVisible(false);
            ui->PriVideo_24bit_checkBox->setVisible(false);
            ui->SecVideo_24bit_checkBox->setVisible(false);
            ui->label_61->setVisible(false);
            ui->PreCompiledFileSystem_frame->setVisible(false);
        }

        if ( AutoRunSelected == "true" )
        {
            ui->UserAutoRun_checkBox->setChecked(true);
            ui->UserAutoRunSelect_pushButton->setEnabled(true);
            ui->UserAutoRunSelectedlineEdit->setEnabled(true);
            ui->Write_AutoRun_pushButton->setEnabled(true);

            if ( AutoRunFolder.length() < 2)
            {
                ui->UserAutoRunSelectedlineEdit->setText("Not Initialized");
                ui->UserAutoRun_checkBox->setChecked(false);
                ui->UserAutoRunSelectedlineEdit->setEnabled(false);
                ui->Write_AutoRun_pushButton->setEnabled(false);
            }
            else
                ui->UserAutoRunSelectedlineEdit->setText(AutoRunFolder);
        }
        else
        {
            ui->UserAutoRun_checkBox->setChecked(false);
            ui->UserAutoRunSelect_pushButton->setEnabled(false);
            ui->UserAutoRunSelectedlineEdit->setEnabled(false);
            ui->UserAutoRunSelectedlineEdit->setText(AutoRunFolder);
            ui->Write_AutoRun_pushButton->setEnabled(false);
        }


        update_status_bar("BKF");
        ui->UserPartition1Size_lineEdit->setText(UserPartition1Size);
        ui->UserPartition2Size_lineEdit->setText(UserPartition2Size);

        if ( NumberOfUserPartitions == "-")
        {
            ui->label_78->setVisible(false);
            ui->UserPartition1Size_lineEdit->setVisible(false);
            ui->label_79->setVisible(false);
            ui->UserPartition2Size_lineEdit->setVisible(false);
        }
        if ( NumberOfUserPartitions == "1")
        {
            ui->label_78->setVisible(true);
            ui->UserPartition1Size_lineEdit->setVisible(true);
            ui->label_79->setVisible(false);
            ui->UserPartition2Size_lineEdit->setVisible(false);
        }
        if ( NumberOfUserPartitions == "2")
        {
            ui->label_78->setVisible(true);
            ui->UserPartition1Size_lineEdit->setVisible(true);
            ui->label_79->setVisible(true);
            ui->UserPartition2Size_lineEdit->setVisible(true);
        }
        ui->uSD_Device_comboBox->setCurrentText(uSD_Device);
        break;
    case 2 : // BSP Factory
        if (CurrentBSPF_Tab == "P BSP Factory")
        {
            //qDebug() << "P BSP Factory";
            ui->P_SATA_checkBox->setChecked(false);
            ui->P_PCIe_checkBox->setChecked(true);
            ui->P_PUPD_frame->setEnabled(false);
            ui->P_Speed_frame->setEnabled(false);
            ui->P_SRE_frame->setEnabled(false);
            ui->P_DSE_frame->setEnabled(false);
            ui->P_PUE_checkBox->setEnabled(false);
            ui->P_SION_checkBox->setEnabled(false);
            ui->P_ODE_checkBox->setEnabled(false);
            ui->P_PKE_checkBox->setEnabled(false);
            ui->P_DSE_Disable_checkBox->setEnabled(false);
            ui->P_HYS_PAD_CTL_checkBox->setEnabled(false);
            ui->P_NO_PAD_CTL_checkBox->setChecked(true);
            ui->P_I2C1Speed_lineEdit->setText("100000");
            ui->P_I2C3Speed_lineEdit->setText("100000");

            ui->P_cbit_lineEdit->setText(CfgBitDefaultValue);
            ui->P_Decoded_CFG_Bits_lineEdit->setText("0x00000000");
            QFileInfo fi(Last_P_BSPFactoryFile);
            if ( ! fi.exists())
            {
                update_status_bar("BSP Factory : File "+fi.baseName()+".bspf not found, reverting to default");
            }
            else
            {
                QString base = fi.baseName();
                if ( base != "" )
                    ui->P_Current_BSPF_File_label->setText(base+".bspf");
                P_load_BSPF_File(Last_P_BSPFactoryFile);
                ui->P_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
                update_status_bar("BSP Factory : Loaded file "+Last_P_BSPFactoryFile);
            }
        }
        if (CurrentBSPF_Tab == "U BSP Factory")
        {
            QFileInfo fi(Last_U_BSPFactoryFile);
            if ( ! fi.exists())
            {
                update_status_bar("BSP Factory : File "+fi.baseName()+".bspf not found, reverting to default");
            }
            else
            {
                QString base = fi.baseName();
                if ( base != "" )
                    ui->U_Current_BSPF_File_label->setText(base+".bspf");
                U_load_BSPF_File(Last_U_BSPFactoryFile);
                ui->U_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
                update_status_bar("BSP Factory : Loaded file "+Last_U_BSPFactoryFile);
            }
        }
        /*
        else
        {
            P_load_BSPF_File(Last_P_BSPFactoryFile);
            QFileInfo fi(Last_P_BSPFactoryFile);
            if ( ! fi.exists())
            {
                update_status_bar("BSP Factory : File "+fi.baseName()+".bspf not found, reverting to default");
            }
            else
            {
                QString base = fi.baseName();
                if ( base != "" )
                    ui->P_Current_BSPF_File_label->setText(base+".bspf");
                update_status_bar("BSP Factory : Loaded file "+Last_P_BSPFactoryFile);
            }
        }
        */
        break;
    }
}

/*****************************************************************************************************************************************************************************************/
/*                                                                          Top tab switches END                                                                                         */
/*****************************************************************************************************************************************************************************************/


void NOVAembed::on_CheckUpdate_pushButton_clicked()
{
    update_status_bar("Checking updates");

    QFile scriptfile("/tmp/script");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/Utils\n");
    out << QString("./UpdateUtils\n");
    out << QString("./CheckGitHubRepo\n");
    out << QString("echo $? > /tmp/result\n");

    scriptfile.close();
    int result = run_script();
    if (  result >= 0)
    {
        if ( result > 0 )
        {
            QString s = QString::number(result);
            if ( result > 1 )
                update_status_bar("Found "+s+" updates, system updated");
            else
                update_status_bar("Found "+s+" update, system updated");
        }
        else
            update_status_bar("No updates found");
    }
    else
        update_status_bar("Update error");

}


void NOVAembed::on_ViewUpdatesLog_pushButton_clicked()
{
    system("kwrite /Devel/NOVAsom_SDK/Logs/update.log");
}

