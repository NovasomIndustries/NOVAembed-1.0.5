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
#include <iostream>
/*****************************************************************************************************************************************************************************************/
/*                                                                              Global variables                                                                                         */
/*****************************************************************************************************************************************************************************************/

QString Version = "1.0.5.1";
QString Configuration = "Standard";
QString FileSystemName = "";
QString DeployedFileSystemName = "";
QString FileSystemConfigName = "";
QString _Board_comboBox = "";
QString Last_M8_BSPFactoryFile = "";
QString Last_M9_BSPFactoryFile = "";
QString Last_U_BSPFactoryFile = "";
QString Last_P_BSPFactoryFile = "";
QString Last_N1_BSPFactoryFile = "";

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
QString Kernel =  NXP_P_KERNEL;
QString SourceMeFile =  NXP_P_SOURCEME;


extern  void storeNOVAembed_ini();
QWidget *PBSP_stab,*UBSP_stab,*M8BSP_stab,*M9BSP_stab,*N1BSP_stab,*TOOL_stab,*TOOL_devel;

/*****************************************************************************************************************************************************************************************/
/*                                                                                    Code                                                                                               */
/*****************************************************************************************************************************************************************************************/

NOVAembed::NOVAembed(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NOVAembed)
{
int     copy_required_files = 0;
QString PixMapName="";

    std::cout << "Starting now" << std::flush;
    /* Initialize user area */
    if ( ! QDir("/Devel/NOVAsom_SDK/Blobs").exists() )
        system("mkdir -p /Devel/NOVAsom_SDK/Blobs");
    if ( ! QDir("/Devel/NOVAsom_SDK/Logs").exists() )
        system("mkdir -p /Devel/NOVAsom_SDK/Logs");
    if ( ! QDir("/Devel/NOVAsom_SDK/Deploy").exists() )
        system("mkdir -p /Devel/NOVAsom_SDK/Deploy");
    if ( ! QDir("/Devel/NOVAsom_SDK/ExternalFileSystems").exists() )
    {
        QMessageBox::information(this, tr("ExternalFileSystems"),"ExternalFileSystems not found. Creating a new one!");
        system("mkdir -p /Devel/NOVAsom_SDK/ExternalFileSystems/P");
        system("mkdir -p /Devel/NOVAsom_SDK/ExternalFileSystems/U");
        system("mkdir -p /Devel/NOVAsom_SDK/ExternalFileSystems/M8");
        system("mkdir -p /Devel/NOVAsom_SDK/ExternalFileSystems/M9");
        copy_required_files = 1;
    }
    if ( ! QDir("/Devel/NOVAsom_SDK/DtbUserWorkArea").exists() )
    {
        QMessageBox::information(this, tr("DtbUserWorkArea"),"DtbUserWorkArea not found. Creating a new one!");
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/PClass_bspf/temp");
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/UClass_bspf/temp");
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf/temp");
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/M9Class_bspf/temp");
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
    if ( ! QDir("/Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf").exists() )
    {
        system("mkdir -p /Devel/NOVAsom_SDK/DtbUserWorkArea/M8Class_bspf/temp");
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
        std::cout << "NOVAembed.ini not found !!" << std::flush;
        QMessageBox::information(this, tr("NOVAembed.ini"),"NOVAembed.ini not found. Creating a new one!");
        if ( ! QDir("/Devel/NOVAsom_SDK/NOVAembed_Settings").exists() )
        {
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
        {
            std::cout << "Configuration !=  Version !!" << std::flush;
            storeNOVAembed_ini();
        }
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
        Last_N1_BSPFactoryFile = settings->value( strKeySettings + "Last_N1_BSPFactoryFile", "r").toString();
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
    //std::cout << "Starting now" << std::flush;

    if ( _Board_comboBox == "")
    {
        _Board_comboBox = "P Series";
        Kernel=NXP_P_KERNEL;
        SourceMeFile=NXP_P_SOURCEME;
        PixMapName=":/Icons/NXP-Logo.png";
    }
    if ( _Board_comboBox == "P Series")
    {
        Kernel=NXP_P_KERNEL;
        SourceMeFile=NXP_P_SOURCEME;
        PixMapName=":/Icons/NXP-Logo.png";
    }
    if ( _Board_comboBox == "U5")
    {
        Kernel=NXP_U_KERNEL;
        SourceMeFile=NXP_U_SOURCEME;
        PixMapName=":/Icons/NXP-Logo.png";
    }
    if ( _Board_comboBox == "N1")
    {
        Kernel=NXP_N1_KERNEL;
        SourceMeFile=NXP_N1_SOURCEME;
        PixMapName=":/Icons/NXP-LayerScapeLogo.png";
    }
    if ( _Board_comboBox == "M8")
    {
        Kernel=QUALCOMM_KERNEL;
        SourceMeFile=QUALCOMM_SOURCEME;
        PixMapName=":/Icons/Qualcomm_Snapdragon_logo.png";
    }
    if ( _Board_comboBox == "M9")
    {
        Kernel=ALLWINNER_KERNEL;
        SourceMeFile=ALLWINNER_SOURCEME;
        PixMapName=":/Icons/allwinnerlogo.png";
    }

    ui->Board_comboBox->setCurrentText(_Board_comboBox);
    Board_comboBox_setText(_Board_comboBox);
    ui->brand_label->setPixmap(QPixmap(PixMapName));
    ui->BrandNameBKF->setPixmap(QPixmap(PixMapName));
    ui->BoardNameBKF->setText(_Board_comboBox);



    QMovie *movie = new QMovie(":/Icons/animation.gif");
    ui->movie_label->setMovie(movie);
    movie->start();

    ui->linklabel->setOpenExternalLinks(true);
    ui->linklabel->setText("<a href=\"http://www.novasomindustries.com\">Novasom Industries</a>");

    ui->FileSystemSelectedlineEdit->setText(FileSystemName);
    ui->UserPartition_comboBox->setCurrentText(NumberOfUserPartitions);
    ui->VersionLabel->setText(Version);
    PBSP_stab=ui->tabBSPFP;
    //SBSP_stab=ui->tabBSPFS;
    UBSP_stab=ui->tabBSPFU;
    M8BSP_stab=ui->tabBSPFM8;
    M9BSP_stab=ui->tabBSPFM9;
    N1BSP_stab=ui->tabBSPFN1;
    TOOL_stab=ui->tabTools;
    ui->tab->removeTab(6);
    ui->tab->removeTab(5);
    ui->tab->removeTab(4);
    ui->tab->removeTab(3);
    ui->tab->removeTab(2);

    if (CurrentBSPF_Tab == "P BSP Factory")
    {
        ui->tab->insertTab(2,PBSP_stab,"P BSP Factory");
    }
    else if (CurrentBSPF_Tab == "M8 BSP Factory")
    {
        ui->tab->insertTab(2,M8BSP_stab,"M8 BSP Factory");
    }
    else if (CurrentBSPF_Tab == "U BSP Factory")
    {
        ui->tab->insertTab(2,UBSP_stab,"U BSP Factory");
    }
    else if (CurrentBSPF_Tab == "M9 BSP Factory")
    {
        ui->tab->insertTab(2,M9BSP_stab,"M9 BSP Factory");
    }
    else if (CurrentBSPF_Tab == "N1 BSP Factory")
    {
        ui->tab->insertTab(2,N1BSP_stab,"N1 BSP Factory");
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
void NOVAembed::disable_kernelbuttons()
{
    ui->KernelXconfig_pushButton->setEnabled(false);
    ui->KernelSplash_pushButton->setEnabled(false);
    ui->KernelCompileSplash_pushButton->setEnabled(false);
    ui->KernelCompile_pushButton->setEnabled(false);
    ui->KernelReCompile_pushButton->setEnabled(false);
    ui->ViewKernelLog_pushButton->setEnabled(false);
}

void NOVAembed::enable_kernelbuttons()
{
    ui->KernelXconfig_pushButton->setEnabled(true);
    ui->KernelSplash_pushButton->setEnabled(true);
    ui->KernelCompileSplash_pushButton->setEnabled(true);
    ui->KernelCompile_pushButton->setEnabled(true);
    ui->KernelReCompile_pushButton->setEnabled(true);
    ui->ViewKernelLog_pushButton->setEnabled(true);
}

void NOVAembed::manage_kerneldwnld_decompress()
{
    if ( QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+"/Makefile").exists() )
    {

        enable_kernelbuttons();
    }
    else
    {

        disable_kernelbuttons();
    }
}


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
    out << QString("Last_N1_BSPFactoryFile="+Last_N1_BSPFactoryFile+"\n");
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
    else if (ui->Board_comboBox->currentText() == "M9")
    {
        str = "M9Class_Buildroot_*.config";
        ui->PrimaryVideo_comboBox->setVisible(true);
        ui->VideoVisible_label->setVisible(true);
    }
    else if (ui->Board_comboBox->currentText() == "U5")
    {
        str = "U5Class_Buildroot_*.config";
        ui->PrimaryVideo_comboBox->setVisible(false);
        ui->VideoVisible_label->setVisible(false);
    }
    else if (ui->Board_comboBox->currentText() == "N1")
    {
        str = "N1Class_Buildroot_*.config";
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
    QDir ExternalFileSystemsDir;
    if ( ui->Board_comboBox->currentText() == "M8")
        ExternalFileSystemsDir="/Devel/NOVAsom_SDK/ExternalFileSystems/M8";
    if ( ui->Board_comboBox->currentText() == "M9")
        ExternalFileSystemsDir="/Devel/NOVAsom_SDK/ExternalFileSystems/M9";
    if ( ui->Board_comboBox->currentText() == "U5")
        ExternalFileSystemsDir="/Devel/NOVAsom_SDK/ExternalFileSystems/U5";
    if ( ui->Board_comboBox->currentText() == "N1")
        ExternalFileSystemsDir="/Devel/NOVAsom_SDK/ExternalFileSystems/N1";
    if ( ui->Board_comboBox->currentText() == "P Series")
        ExternalFileSystemsDir="/Devel/NOVAsom_SDK/ExternalFileSystems/P";

    str = "*.img.bz2";

    QStringList ExternalFileSystemsnameFilter(str);
    QStringList ExternalFileSystemsfilesList = ExternalFileSystemsDir.entryList(ExternalFileSystemsnameFilter);

    ui->ExtFS_comboBox->clear();

    for(int i=0;i<ExternalFileSystemsfilesList.count();i++)
    {
        str = ExternalFileSystemsfilesList[i];
        if (( str != ".") && (str != ".."))
        {
            ui->ExtFS_comboBox->addItem(str);
        }
    }
    ui->ExtFS_comboBox->setCurrentIndex(0);
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
    //system("konsole -e /tmp/script > /Devel/NOVAsom_SDK/Logs/main_log");
    system("xterm -geometry 120x24 -e /tmp/script > /Devel/NOVAsom_SDK/Logs/main_log");

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

int NOVAembed::run_background_script(void)
{
    this->setCursor(Qt::WaitCursor);

    system("rm -f /tmp/result");
    system("chmod 777 /tmp/background_script");
    system("konsole -e /tmp/background_script --background-mode -geometry 350x150 & > /Devel/NOVAsom_SDK/Logs/main_log");

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


int NOVAembed::CheckIfKernelsPresent()
{
    // Check if the kernel exists
    if ( !QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+"/Makefile").exists() )
    {
        if ( !QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+".tar.bz2").exists() )
        {
            system("rm /tmp/wgetpidlen ; pidof wget > /tmp/wgetpidlen");
            int size;
            QFile wgetpidlen("/tmp/wgetpidlen");
            if (wgetpidlen.open(QIODevice::ReadOnly)){
                size = wgetpidlen.size();
                wgetpidlen.close();
            }
            if ( size > 1 )
            {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "Download still in progress" , "If you reply \"Yes\" the current download will be stopped.\n\nDo you want to stop the current download?", QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    system("kill -9 `pidof wget`");
                    update_status_bar("Download of "+Kernel+".tar.bz2 cancelled");
                }
                else
                    return 1;
            }

            QMessageBox::StandardButton reply = QMessageBox::question(this, "Archive not present" , "The archive "+Kernel+".tar.bz2 does not exists.\nThis should be a time consuming task,\nand depends on your internet connection\nand on remote servers load.\n\nIf you reply \"No\" the dtb file cannot be compiled.\n\nDo you want to start the download?", QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QString syscmd = "/Devel/NOVAsom_SDK/Utils/download_kernel "+Kernel+" "+KERNEL_REPO_SERVER;
                const char *str =syscmd.toLocal8Bit().data();
                update_status_bar("Downloading "+Kernel+".tar.bz2 from "+KERNEL_REPO_SERVER+" ...");
                this->setCursor(Qt::WaitCursor);
                system(str);
                this->setCursor(Qt::ArrowCursor);
                return 0;
            }
            else
                return 1;
        }
        if ( QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+".tar.bz2").exists() )
        {
            if ( !QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+"/Makefile").exists() )
            {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "Kernel source not decompressed" , "The archive "+Kernel+".tar.bz2 exists and must be decompressed.\nThis will be a disk space consuming task,\n\nIf you reply \"No\" the dtb file cannot be compiled.\n\nDo you want to decompress the kernel?", QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    QString syscmd = "/Devel/NOVAsom_SDK/Utils/decompress_kernel "+Kernel;
                    const char *str =syscmd.toLocal8Bit().data();
                    update_status_bar("Decompressing "+Kernel+".tar.bz2 ...");
                    this->setCursor(Qt::WaitCursor);
                    system(str);
                    this->setCursor(Qt::ArrowCursor);
                    return 0;
                }
                else
                    return 1;
            }
        }
    }
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
        manage_kerneldwnld_decompress();

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
        ui->frame_5->setEnabled(true);
        if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
        {
            ui->frame_5->setEnabled(false);
        }

        ui->VideoVisible_label->setVisible(true);
        ui->VideoVisible_label_2->setVisible(true);
        ui->PrimaryVideo_comboBox->setVisible(true);
        ui->SecondaryVideo_comboBox->setVisible(true);
        ui->PriVideo_24bit_checkBox->setVisible(true);
        ui->SecVideo_24bit_checkBox->setVisible(true);
        ui->label_61->setVisible(true);
        ui->UserBSPFSelect_pushButton->setVisible(true);
        ui->UserBSPFselectedlineEdit->setVisible(true);

        ui->label_61->setVisible(false);
        ui->UserBSPFSelect_pushButton->setVisible(false);
        ui->UserBSPFselectedlineEdit->setVisible(false);
        ui->label_65->setVisible(false);
        ui->label_64->setVisible(false);
        ui->ExtFSBSPFSelect_pushButton->setVisible(false);
        ui->ExtFSBSPFselectedlineEdit->setVisible(false);

        ui->SplashThumb->setVisible(true);
        ui->SplashImageNameLabel->setVisible(true);
        ui->KernelSplash_pushButton->setVisible(true);
        ui->KernelCompileSplash_pushButton->setVisible(true);

        if ( ui->Board_comboBox->currentText() == "M9")
        {
            ui->label_61->setVisible(true);
            ui->UserBSPFSelect_pushButton->setVisible(true);
            ui->UserBSPFselectedlineEdit->setVisible(true);
            ui->label_65->setVisible(true);
            ui->label_64->setVisible(true);
            ui->ExtFSBSPFSelect_pushButton->setVisible(true);
            ui->ExtFSBSPFselectedlineEdit->setVisible(true);
            Kernel=ALLWINNER_KERNEL;
            SourceMeFile=ALLWINNER_SOURCEME;
            if ( Last_M9_BSPFactoryFile.length() < 2)
            {
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
                ui->frame_5->setEnabled(false);
            }
            else
            {
                ui->UserBSPFselectedlineEdit->setText(Last_M9_BSPFactoryFile);
                ui->frame_5->setEnabled(true);
            }
            ui->brand_label->setPixmap(QPixmap(":/Icons/allwinnerlogo.png"));
        }
        if ( ui->Board_comboBox->currentText() == "M8")
        {
            ui->label_61->setVisible(true);
            ui->UserBSPFSelect_pushButton->setVisible(true);
            ui->UserBSPFselectedlineEdit->setVisible(true);
            ui->label_65->setVisible(true);
            ui->label_64->setVisible(true);
            ui->ExtFSBSPFSelect_pushButton->setVisible(true);
            ui->ExtFSBSPFselectedlineEdit->setVisible(true);
            Kernel=QUALCOMM_KERNEL;
            SourceMeFile=QUALCOMM_SOURCEME;
            if ( Last_M8_BSPFactoryFile.length() < 2)
            {
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
                ui->frame_5->setEnabled(false);
            }
            else
            {
                ui->UserBSPFselectedlineEdit->setText(Last_M8_BSPFactoryFile);
                ui->frame_5->setEnabled(true);
            }
            ui->brand_label->setPixmap(QPixmap(":/Icons/Qualcomm_Snapdragon_logo.png"));
        }
        if ( ui->Board_comboBox->currentText() == "P Series")
        {
            ui->label_61->setVisible(true);
            ui->UserBSPFSelect_pushButton->setVisible(true);
            ui->UserBSPFselectedlineEdit->setVisible(true);
            ui->label_65->setVisible(true);
            ui->label_64->setVisible(true);
            ui->ExtFSBSPFSelect_pushButton->setVisible(true);
            ui->ExtFSBSPFselectedlineEdit->setVisible(true);
            Kernel=NXP_P_KERNEL;
            SourceMeFile=NXP_P_SOURCEME;
            if ( Last_P_BSPFactoryFile.length() < 2)
            {
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
                ui->frame_5->setEnabled(false);
            }
            else
            {
                P_load_BSPF_File(Last_P_BSPFactoryFile);
                ui->UserBSPFselectedlineEdit->setText(Last_P_BSPFactoryFile);
                ui->frame_5->setEnabled(true);
            }
            ui->brand_label->setPixmap(QPixmap(":/Icons/NXP-Logo.png"));
        }
        if ( ui->Board_comboBox->currentText() == "U5")
        {
            ui->label_61->setVisible(true);
            ui->UserBSPFSelect_pushButton->setVisible(true);
            ui->UserBSPFselectedlineEdit->setVisible(true);
            ui->label_65->setVisible(true);
            ui->label_64->setVisible(true);
            ui->ExtFSBSPFSelect_pushButton->setVisible(true);
            ui->ExtFSBSPFselectedlineEdit->setVisible(true);
            Kernel=NXP_U_KERNEL;
            SourceMeFile=NXP_U_SOURCEME;
            if ( Last_U_BSPFactoryFile.length() < 2)
            {
                ui->UserBSPFselectedlineEdit->setText("Not Initialized");
                ui->frame_5->setEnabled(false);
            }
            else
            {
                ui->UserBSPFselectedlineEdit->setText(Last_U_BSPFactoryFile);
                ui->frame_5->setEnabled(true);
            }
            ui->brand_label->setPixmap(QPixmap(":/Icons/NXP-Logo.png"));
        }
        if ( ui->Board_comboBox->currentText() == "N1")
        {
            ui->SplashThumb->setVisible(false);
            ui->SplashImageNameLabel->setVisible(false);
            ui->KernelSplash_pushButton->setVisible(false);
            ui->KernelCompileSplash_pushButton->setVisible(false);
            Kernel=NXP_N1_KERNEL;
            SourceMeFile=NXP_N1_SOURCEME;
            ui->brand_label->setPixmap(QPixmap(":/Icons/NXP-LayerScapeLogo.png"));
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
            if (( ui->PrimaryVideo_comboBox->currentText() == "HDMI 1920x1080") || ( ui->PrimaryVideo_comboBox->currentText() == "HDMI 1280x720"))
                ui->PriVideo_spwg_checkBox->setEnabled(false);
            else
                ui->PriVideo_spwg_checkBox->setEnabled(true);

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
                ui->U_I2C2Speed_lineEdit->setText("100000");

                ui->U_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
                update_status_bar("BSP Factory : Loaded file "+Last_U_BSPFactoryFile);
            }
        }
        if (CurrentBSPF_Tab == "M8 BSP Factory")
        {
            QFileInfo fi(Last_M8_BSPFactoryFile);
            if ( ! fi.exists())
            {
                update_status_bar("BSP Factory : File "+fi.baseName()+".bspf not found, reverting to default");
            }
            else
            {
                QString base = fi.baseName();
                if ( base != "" )
                    ui->M8_Current_BSPF_File_label->setText(base+".bspf");
                M8_load_BSPF_File(Last_M8_BSPFactoryFile);
                ui->M8_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
                update_status_bar("BSP Factory : Loaded file "+Last_M8_BSPFactoryFile);
            }
        }
        if (CurrentBSPF_Tab == "N1 BSP Factory")
        {
                update_status_bar("BSP Factory : N1 has no BSP factory options");
        }
        if (CurrentBSPF_Tab == "M9 BSP Factory")
        {
            QFileInfo fi(Last_M9_BSPFactoryFile);
            if ( ! fi.exists())
            {
                update_status_bar("BSP Factory : File "+fi.baseName()+".bspf not found, reverting to default");
            }
            else
            {
                QString base = fi.baseName();
                if ( base != "" )
                    ui->M9_Current_BSPF_File_label->setText(base+".bspf");
                M9_load_BSPF_File(Last_M9_BSPFactoryFile);
                ui->M9_Generate_pushButton->setText("Save "+fi.baseName()+".bspf and Generate "+fi.baseName()+".dtb");
                update_status_bar("BSP Factory : Loaded file "+Last_M9_BSPFactoryFile);
            }
        }
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




void NOVAembed::on_actionVersion_triggered()
{
    QString whole="NOVAembed V."+Version;
    char msg1[]="Copyright 2017-2018 Novasomindustries.\nAll rights reserved.\n\nThe program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n";

    QByteArray ba = whole.toLatin1();
    const char *msg0 = ba.data();
    QMessageBox::information(
        this,
        tr(msg0),
        tr(msg1)
    );
}

void NOVAembed::on_M9_Generate_pushButton_clicked()
{
    if ( CheckIfKernelsPresent() == 1 )
        return;
}



