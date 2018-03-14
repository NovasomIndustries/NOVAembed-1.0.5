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
#include <QNetworkInterface>
#include <QDialog>
#include <iostream>

extern  QString FileSystemName;
extern  QString DeployedFileSystemName;
extern  QString FileSystemConfigName;
extern  QString _Board_comboBox;
extern  QString Last_M8_BSPFactoryFile;
extern  QString Last_P_BSPFactoryFile;
extern  QString Last_U_BSPFactoryFile;
extern  QString NumberOfUserPartitions;
extern  QString UserPartition1Size;
extern  QString UserPartition2Size;
extern  QString uSD_Device;
extern  QString CurrentBSPF_Tab;
extern  QString CurrentPrimaryVideo;
extern  QString CurrentSecondaryVideo;
extern  QString AutoRunSelected;
extern  QString AutoRunFolder;
extern  QString Kernel;
extern  QString SourceMeFile;

extern  QWidget *PBSP_stab,*UBSP_stab,*SBSP_stab,*M8BSP_stab,*M9BSP_stab,*TOOL_stab;

extern QString BootValid , FSValid , KernelValid , uSDwriteValid;

int initrd_size;

QWidget *current_stab;

/*****************************************************************************************************************************************************************************************/
/*                                                                               BKF Tab                                                                                                 */
/*****************************************************************************************************************************************************************************************/

void NOVAembed::initrd_helper(void)
{
    QFileInfo info1("/Devel/NOVAsom_SDK/FileSystem/"+FileSystemName+"/output/images/rootfs.ext2");
    initrd_size = (info1.size() / 1024) + 32000;
    ui->initRdSize_lineEdit->setText( QString::number(initrd_size) );
}

/* Bootloader */
void NOVAembed::on_BootLoaderCompile_pushButton_clicked()
{
    QFile scriptfile("/tmp/script");
    update_status_bar("Compiling boot loader for "+ui->Board_comboBox->currentText()+" ... ");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/Utils\n");
    if ( ui->Board_comboBox->currentText() == "P Series")
    {
        out << QString("cd nxp\n");
        out << QString("./umakeP > /Devel/NOVAsom_SDK/Logs/umakeP.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "U5")
    {
        out << QString("cd nxp\n");
        out << QString("./umakeU > /Devel/NOVAsom_SDK/Logs/umakeU.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M8")
    {
        out << QString("cd qcom\n");
        out << QString("./umakeM8 > /Devel/NOVAsom_SDK/Logs/umakeM8.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M9")
    {
        out << QString("cd allw\n");
        out << QString("./umakeM9 > /Devel/NOVAsom_SDK/Logs/umakeM9.log\n");
    }

    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("Boot loader for "+ui->Board_comboBox->currentText()+" compiled");
        BootValid = "OK";
        ui->uboot_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
    }
    else
    {
        update_status_bar("Error building boot loader for "+ui->Board_comboBox->currentText());
        BootValid = "INVALID";
        ui->uboot_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));

    }
    if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
        ui->frame_5->setEnabled(false);
    else
        ui->frame_5->setEnabled(true);
    storeNOVAembed_ini();
}

/* Kernel */
void NOVAembed::on_KernelXconfig_pushButton_clicked()
{
    QFile scriptfile("/tmp/script");
    QString config_file;

    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/Kernel/"+Kernel+"\n");
    out << QString(". ../../Utils/"+SourceMeFile+"\n");

    if ( ui->Board_comboBox->currentText() == "P Series")
        config_file = "imx_novasomp_defconfig";
    if ( ui->Board_comboBox->currentText() == "U5")
        config_file = "imx_v7_defconfig";
    if ( ui->Board_comboBox->currentText() == "M8")
        config_file = "qcom_defconfig";
    if ( ui->Board_comboBox->currentText() == "M9")
        config_file = "novasom_m9_defconfig";

    if ( !QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+"/.config").exists() )
    {
        out << QString("make "+config_file+"\n");
    }
    out << QString("make xconfig\n");
    out << QString("echo \"0\" > /tmp/result\n");

    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("Kernel configuration done");
        KernelValid = "OK";
    }
    else
    {
        update_status_bar("Configuration error");
        KernelValid = "INVALID";
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
    }
    if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
        ui->Write_uSD_pushButton->setEnabled(false);
    else
        ui->Write_uSD_pushButton->setEnabled(true);
    storeNOVAembed_ini();
}

QString LinuxSplashLogos =  "/Devel/NOVAsom6_SDK/Utils/LinuxSplashLogos/";
extern QString CurrentSplashName;

void NOVAembed::on_KernelCompileSplash_pushButton_clicked()
{
    QFile scriptfile("/tmp/script");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);

    out << QString("#!/bin/sh\n");
    out << QString("/Devel/NOVAsom_SDK/Utils/CreateLogo /Devel/NOVAsom_SDK/Utils/LinuxSplashLogos/"+CurrentSplashName+".png "+Kernel+"\n");
    out << QString("echo \"0\" > /tmp/result\n");

    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("Splash set");
        on_KernelCompile_pushButton_clicked();
    }
    else
    {
        KernelValid = "INVALID";
        update_status_bar("Error setting splash");
    }
    storeNOVAembed_ini();
}


void NOVAembed::on_KernelSplash_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/Devel/NOVAsom_SDK/Utils/LinuxSplashLogos",tr("Image File (*.png)"));
    if ( fileName == "" ) // Hit cancel
    {
        return;
    }
    QFileInfo fileInfo(fileName );
    QString TheName(fileInfo.baseName());
    CurrentSplashName = TheName;
    ui->SplashImageNameLabel->setText(CurrentSplashName+".png");
    ui->SplashThumb->setPixmap(QPixmap(fileName) );
    storeNOVAembed_ini();
}

void NOVAembed::on_KernelCompile_pushButton_clicked()
{
    if ( !QFile("/Devel/NOVAsom_SDK/Kernel/"+Kernel+"/.config").exists() )
    {
        on_KernelReCompile_pushButton_clicked();
        return;
    }

    QFile scriptfile("/tmp/script");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    update_status_bar("Compiling "+Kernel);
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("/Devel/NOVAsom_SDK/Utils/CreateLogo /Devel/NOVAsom_SDK/Utils/LinuxSplashLogos/"+CurrentSplashName+".png "+Kernel+" > /Devel/NOVAsom_SDK/Logs/kmake.log\n");
    out << QString("cd /Devel/NOVAsom_SDK/Deploy\n");
    if ( ui->Board_comboBox->currentText() == "P Series")
    {
        out << QString("rm zImage ; ln -s ../Kernel/"+Kernel+"/arch/arm/boot/zImage\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/nxp\n");
        out << QString("./kmake "+Kernel+" "+SourceMeFile+" >> /Devel/NOVAsom_SDK/Logs/kmake.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "U5")
    {
        out << QString("rm zImage ; ln -s ../Kernel/"+Kernel+"/arch/arm/boot/zImage\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/nxp\n");
        out << QString("./kmake "+Kernel+" "+SourceMeFile+" >> /Devel/NOVAsom_SDK/Logs/kmake.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M8")
    {
        out << QString("rm Image ; ln -s ../Kernel/"+Kernel+"/arch/arm64/boot/Image\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/qcom\n");
        out << QString("./kmake "+Kernel+" "+SourceMeFile+" >> /Devel/NOVAsom_SDK/Logs/kmake.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M9")
    {
        out << QString("rm Image ; ln -s ../Kernel/"+Kernel+"/arch/arm64/boot/Image\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/allw\n");
        out << QString("./kmake "+Kernel+" "+SourceMeFile+" >> /Devel/NOVAsom_SDK/Logs/kmake.log\n");
    }

    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("Kernel built succesfully");
        KernelValid = "OK";
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
    }
    else
    {
        update_status_bar("Kernel Build error");
        KernelValid = "INVALID";
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
    }
    if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
        ui->frame_5->setEnabled(false);
    else
        ui->frame_5->setEnabled(true);
    storeNOVAembed_ini();
}

void NOVAembed::on_KernelReCompile_pushButton_clicked()
{
    QFile scriptfile("/tmp/script");
    QString config_file;
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    update_status_bar("ReCompiling "+Kernel);

    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("/Devel/NOVAsom_SDK/Utils/CreateLogo /Devel/NOVAsom_SDK/Utils/LinuxSplashLogos/"+CurrentSplashName+".png "+Kernel+" > /Devel/NOVAsom_SDK/Logs/kremake.log\n");
    out << QString("cd /Devel/NOVAsom_SDK/Deploy\n");

    if ( ui->Board_comboBox->currentText() == "P Series")
    {
        out << QString("rm zImage ; ln -s ../Kernel/"+Kernel+"/arch/arm/boot/zImage\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/nxp\n");
        config_file = "imx_novasomp_defconfig";
        out << QString("./kremake "+Kernel+" "+SourceMeFile+" "+config_file+">> /Devel/NOVAsom_SDK/Logs/kremake.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "U5")
    {
        out << QString("rm zImage ; ln -s ../Kernel/"+Kernel+"/arch/arm/boot/zImage\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/nxp\n");
        config_file = "imx_v7_defconfig";
        out << QString("./kremake "+Kernel+" "+SourceMeFile+" "+config_file+">> /Devel/NOVAsom_SDK/Logs/kremake.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M8")
    {
        out << QString("rm Image ; ln -s ../Kernel/"+Kernel+"/arch/arm64/boot/Image\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/qcom\n");
        config_file = "qcom_defconfig";
        out << QString("./kremake "+Kernel+" "+SourceMeFile+" "+config_file+">> /Devel/NOVAsom_SDK/Logs/kremake.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M9")
    {
        out << QString("rm Image ; ln -s ../Kernel/"+Kernel+"/arch/arm64/boot/Image\n");
        out << QString("cd /Devel/NOVAsom_SDK/Utils/allw\n");
        config_file = "novasom_m9_defconfig";
        out << QString("./kremake "+Kernel+" "+SourceMeFile+" "+config_file+">> /Devel/NOVAsom_SDK/Logs/kremake.log\n");
    }
    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("Kernel re-built succesfully");
        KernelValid = "OK";
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
    }
    else
    {
        update_status_bar("Kernel re-build error");
        KernelValid = "INVALID";
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
    }
    if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
        ui->frame_5->setEnabled(false);
    else
        ui->frame_5->setEnabled(true);
    storeNOVAembed_ini();
}

/* File System */
void NOVAembed::on_SelectFileSystem_pushButton_clicked()
{
    //QString directory =  QFileDialog::getOpenFileName(this,"Open File System","/Devel/NOVAsom_SDK/FileSystem/","*.*");
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/Devel/NOVAsom_SDK/FileSystem/",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QFileInfo fileInfo(directory );
    QString filesystemname(fileInfo.fileName());
    ui->FileSystemSelectedlineEdit->setText(filesystemname);
    FileSystemName = filesystemname;
    storeNOVAembed_ini();
}

void NOVAembed::on_LaunchMenuConfig_pushButton_clicked()
{
    if ( ui->FileSystemSelectedlineEdit->text() == "")
    {
       update_status_bar("File system name is empty");
       return;
    }
    update_status_bar("Configuring "+FileSystemName+" ...");
    QFile scriptfile("/tmp/script");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/FileSystem/"+FileSystemName+"\n");
    out << QString("make menuconfig\n");
    out << QString("echo \"0\" > /tmp/result\n");

    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("Buildroot Configuration Finished");
        FSValid = "OK";
        ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));

    }
    else
    {
        update_status_bar("Configuration error");
        FSValid = "INVALID";
        ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
    }
    storeNOVAembed_ini();
}

void NOVAembed::on_LaunchBusyboxMenuConfig_pushButton_clicked()
{
    if ( ui->FileSystemSelectedlineEdit->text() == "")
    {
       update_status_bar("File system name is empty");
       return;
    }
    update_status_bar("Configuring "+FileSystemName+" ...");
    QFile scriptfile("/tmp/script");
    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/FileSystem/"+FileSystemName+"\n");
    out << QString("make busybox-menuconfig\n");
    out << QString("echo \"0\" > /tmp/result\n");

    scriptfile.close();
    if ( run_script() == 0)
        update_status_bar("Busybox Configuration Finished");
    else
        update_status_bar("Configuration error");
}

void NOVAembed::on_ThisIsReferenceServer_checkBox_clicked(bool checked)
{
    QString IP;

    ui->iperror_label->setVisible(false);

    if ( checked == false )
    {
        ui->REFERENCE_SERVER_label->setEnabled(true);
        ui->REFERENCE_SERVER_lineEdit->setEnabled(true);
        IP=ui->REFERENCE_SERVER_lineEdit->text();
    }
    else
    {
        ui->REFERENCE_SERVER_label->setEnabled(false);
        ui->REFERENCE_SERVER_lineEdit->setEnabled(false);
        QNetworkInterface eth1Ip = QNetworkInterface::interfaceFromName("enp0s3");
        QList<QNetworkAddressEntry> entries = eth1Ip.addressEntries();
        if (!entries.isEmpty()) {
            QNetworkAddressEntry entry = entries.first();
            IP=entry.ip().toString();
        }
    }
    ui->REFERENCE_SERVER_lineEdit->setText(IP);

}

void NOVAembed::on_FileSystemDeploy_pushButton_clicked()
{
    if ( ui->FileSystemSelectedlineEdit->text().isEmpty())
    {
        update_status_bar("File system name is empty");
        return;
    }
    QHostAddress myIP;
    QString IP=ui->REFERENCE_SERVER_lineEdit->text();
    if( myIP.setAddress(ui->REFERENCE_SERVER_lineEdit->text()) )
    {
        ui->iperror_label->setVisible(false);
    }
    else
    {
        ui->iperror_label->setVisible(true);
        update_status_bar("Invalid IP address");
        return;
    }
    QFile scriptfile("/tmp/script");
    update_status_bar("Compiling "+FileSystemName+" ...");

    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }

    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    if ( ui->Board_comboBox->currentText() == "P Series")
        out << QString("/Devel/NOVAsom_SDK/Utils/MakeFs "+ui->FileSystemSelectedlineEdit->text()+" "+IP+" P > /Devel/NOVAsom_SDK/Logs/FileSystem_Pmake.log\n");
    if ( ui->Board_comboBox->currentText() == "U5")
        out << QString("/Devel/NOVAsom_SDK/Utils/MakeFs "+ui->FileSystemSelectedlineEdit->text()+" "+IP+" U > /Devel/NOVAsom_SDK/Logs/FileSystem_Umake.log\n");
    if ( ui->Board_comboBox->currentText() == "M8")
        out << QString("/Devel/NOVAsom_SDK/Utils/MakeFs "+ui->FileSystemSelectedlineEdit->text()+" "+IP+" M8 > /Devel/NOVAsom_SDK/Logs/FileSystem_M8make.log\n");
    if ( ui->Board_comboBox->currentText() == "M9")
        out << QString("/Devel/NOVAsom_SDK/Utils/MakeFs "+ui->FileSystemSelectedlineEdit->text()+" "+IP+" M9 > /Devel/NOVAsom_SDK/Logs/FileSystem_M8make.log\n");



    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("File system "+ui->FileSystemSelectedlineEdit->text()+" deployed");
        DeployedFileSystemName = FileSystemName = ui->FileSystemSelectedlineEdit->text();
        FSValid = "OK";
        ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
        initrd_helper();
    }
    else
    {
        update_status_bar("Build error");
        FSValid = "INVALID";
        ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
    }
    if ((BootValid != "OK") || (FSValid != "OK") || (KernelValid != "OK"))
        ui->frame_5->setEnabled(false);
    else
        ui->frame_5->setEnabled(true);
    storeNOVAembed_ini();
}


/* uSD write */

void NOVAembed::on_UserPartition_comboBox_currentIndexChanged(const QString &arg1)
{
    NumberOfUserPartitions = arg1;
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
    storeNOVAembed_ini();
}


void NOVAembed::on_UserPartition1Size_lineEdit_textChanged(const QString &arg1)
{
    UserPartition1Size = arg1;
    storeNOVAembed_ini();
}


void NOVAembed::on_UserPartition2Size_lineEdit_textChanged(const QString &arg1)
{
    UserPartition2Size = arg1;
    storeNOVAembed_ini();
}

void NOVAembed::on_uSD_Device_comboBox_currentIndexChanged(const QString &arg1)
{
    uSD_Device = arg1;
    storeNOVAembed_ini();
}

QString NOVAsomParamsName;
void NOVAembed::NOVAsom_Params_helper()
{
    if ( ui->Board_comboBox->currentText() == "P Series")
    {
        if ( ui->PrimaryVideo_comboBox->currentText() == "HDMI 1920x1080")
        {
            NOVAsomParamsName = "NOVAsomParams_P_HDMI_1920x1080";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "HDMI 1280x720" )
        {
            NOVAsomParamsName = "NOVAsomParams_P_HDMI_1280x720";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 1920x1080 2Ch")
        {
            NOVAsomParamsName = "NOVAsomParams_P_LVDS_1920x10802CH";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 1366x768")
        {
            NOVAsomParamsName = "NOVAsomParams_P_LVDS_1366x768";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 1280x800")
        {
            if ( ui->PriVideo_24bit_checkBox->isChecked() == true)
                NOVAsomParamsName = "NOVAsomParams_P_LVDS_1280x800_24";
            else
                NOVAsomParamsName = "NOVAsomParams_P_LVDS_1280x800_18";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 1024x768")
        {
            NOVAsomParamsName = "NOVAsomParams_P_LVDS_1024x768";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 1024x600")
        {
            NOVAsomParamsName = "NOVAsomParams_P_LVDS_1024x600";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 800x600")
        {
            NOVAsomParamsName = "NOVAsomParams_P_LVDS_800x600";
        }
        if ( ui->PrimaryVideo_comboBox->currentText() == "LVDS 800x480")
        {
            NOVAsomParamsName = "NOVAsomParams_P_LVDS_800x480";
        }
    }
    if ( ui->Board_comboBox->currentText() == "U5")
    {
        if ( ui->U_PrimaryVideo_comboBox->currentText() == "800x480")
        {
            NOVAsomParamsName = "NOVAsomParams_U5_800x480";
        }
        if ( ui->U_PrimaryVideo_comboBox->currentText() == "480x272")
        {
            NOVAsomParamsName = "NOVAsomParams_U5_480x272";
        }
    }
}

void NOVAembed::on_Write_uSD_pushButton_clicked()
{
    /*uSD_Device_comboBox*/

    uSD_Device = ui->uSD_Device_comboBox->currentText();
    QFile scriptfile("/tmp/script");
    update_status_bar("Writing uSD with "+FileSystemName+" ...");

    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }

    NOVAsom_Params_helper();

    QFileInfo fi(ui->UserBSPFselectedlineEdit->text());

    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");

    if ( ui->Board_comboBox->currentText() == "U5")
    {
        out << QString("cd /Devel/NOVAsom_SDK/Utils/nxp\n");
        out << QString("./flashU "+NumberOfUserPartitions+" "+UserPartition1Size+" "+UserPartition2Size+" /dev/"+uSD_Device+" "+fi.baseName()+" "+NOVAsomParamsName+" "+ui->initRdSize_lineEdit->text()+" > /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
        if ( ui->UserAutoRun_checkBox->isChecked())
            out << QString("./store_application_storage "+ui->UserAutoRunSelectedlineEdit->text()+" /dev/"+uSD_Device+" >> /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "P Series")
    {
        out << QString("cd /Devel/NOVAsom_SDK/Utils/nxp\n");
        out << QString("./flashP "+NumberOfUserPartitions+" "+UserPartition1Size+" "+UserPartition2Size+" /dev/"+uSD_Device+" "+"SDL_"+fi.baseName()+".dtb"+" "+"QUAD_"+fi.baseName()+".dtb"+" "+NOVAsomParamsName+" "+ui->initRdSize_lineEdit->text()+" > /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
        if ( ui->UserAutoRun_checkBox->isChecked())
            out << QString("./store_application_storage "+ui->UserAutoRunSelectedlineEdit->text()+" /dev/"+uSD_Device+" >> /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M8")
    {
        out << QString("cd /Devel/NOVAsom_SDK/Utils/qcom\n");
        out << QString("./flashM8 /dev/"+uSD_Device+" > /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
        if ( ui->UserAutoRun_checkBox->isChecked())
            out << QString("./store_application_storage "+ui->UserAutoRunSelectedlineEdit->text()+" /dev/"+uSD_Device+" >> /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
    }
    if ( ui->Board_comboBox->currentText() == "M9")
    {
        out << QString("cd /Devel/NOVAsom_SDK/Utils/allw\n");
        out << QString("./flashM9 "+NumberOfUserPartitions+" "+UserPartition1Size+" "+UserPartition2Size+" /dev/"+uSD_Device+" "+fi.baseName()+" "+NOVAsomParamsName+" "+ui->initRdSize_lineEdit->text()+" > /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
        if ( ui->UserAutoRun_checkBox->isChecked())
            out << QString("./store_application_storage "+ui->UserAutoRunSelectedlineEdit->text()+" /dev/"+uSD_Device+" >> /Devel/NOVAsom_SDK/Logs/uSD_Write.log\n");
    }
    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("uSD successfully written, file system is "+FileSystemName);
        DeployedFileSystemName = FileSystemName;
        storeNOVAembed_ini();
    }
    else
        update_status_bar("Write error");
}

void NOVAembed::on_GenerateFileSystem_pushButton_clicked()
{
    if ( ui->NewFileSystemSelectedlineEdit->text().isEmpty())
    {
        update_status_bar("File System name can't be empty");
        return;
    }
    if ( QDir("/Devel/NOVAsom_SDK/FileSystem/"+ui->NewFileSystemSelectedlineEdit->text()).exists() )
    {
        update_status_bar("A file system called "+ui->NewFileSystemSelectedlineEdit->text()+" already exists");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "A file system called "+ui->NewFileSystemSelectedlineEdit->text()+" already exists      ","Overwrite?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QDir dir( "/Devel/NOVAsom_SDK/FileSystem/"+ui->NewFileSystemSelectedlineEdit->text() );
            dir.removeRecursively();
        }
        else
        {
            return;
        }
    }
    QString testing = ui->NewFileSystemSelectedlineEdit->text().remove(" ");
    if ( testing != ui->NewFileSystemSelectedlineEdit->text() )
    {
        update_status_bar("Spaces in File System name not allowed");
        return;
    }
    QFile scriptfile("/tmp/script");
    update_status_bar("Creating file system "+ui->NewFileSystemSelectedlineEdit->text()+" ...");

    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/Utils\n");
    out << QString("./SetupFs "+ui->NewFileSystemSelectedlineEdit->text()+" \""+ui->Board_comboBox->currentText()+"\" "+ ui->NewFileSystemConfigurationcomboBox->currentText()+"\n");
    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("File System "+ui->NewFileSystemSelectedlineEdit->text()+" generated succesfully");
        DeployedFileSystemName = FileSystemName = ui->NewFileSystemSelectedlineEdit->text();
        ui->FileSystemSelectedlineEdit->setText(DeployedFileSystemName);
        storeNOVAembed_ini();
    }
    else
        update_status_bar("File System Creation error");
}

void NOVAembed::on_AddFileSystemConfig_pushButton_clicked()
{
    QString dest_file;
    if ( ui->Board_comboBox->currentText() == "P Series")
        dest_file = "/Devel/NOVAsom_SDK/Utils/Configurations/PClass_Buildroot_"+ui->FileSystemSelectedlineEdit->text()+".config";
    if ( ui->Board_comboBox->currentText() == "U5")
        dest_file = "/Devel/NOVAsom_SDK/Utils/Configurations/U5Class_Buildroot_"+ui->FileSystemSelectedlineEdit->text()+".config";
    if ( ui->Board_comboBox->currentText() == "M8")
        dest_file = "/Devel/NOVAsom_SDK/Utils/Configurations/M8Class_Buildroot_"+ui->FileSystemSelectedlineEdit->text()+".config";
    QFileInfo check_file(dest_file);
    QString filename(check_file.fileName());

    if (check_file.exists() && check_file.isFile())
    {
        update_status_bar(filename+" already exists");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Already exists !","Overwrite "+filename+"?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            qDebug() << "Yes was clicked";
        }
        else
        {
            return;
        }
    }
    QFile::copy("/Devel/NOVAsom_SDK/FileSystem/"+ui->FileSystemSelectedlineEdit->text()+"/.config", dest_file);

    QFile::copy("/Devel/NOVAsom_SDK/FileSystem/"+ui->FileSystemSelectedlineEdit->text()+"/BusyBox.config", "/Devel/NOVAsom_SDK/Utils/Configurations/BusyBox_"+ui->FileSystemSelectedlineEdit->text()+".config");

    compile_NewFileSystemFileSystemConfigurationcomboBox();
}

/* External file systems */
void NOVAembed::on_ExtFS_Write_uSD_pushButton_clicked()
{
    QFile scriptfile("/tmp/script");
    QString full_path;
    update_status_bar("Creating file system "+ui->NewFileSystemSelectedlineEdit->text()+" ...");

    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/Utils\n");
    if ( ui->Board_comboBox->currentText() == "U5")
        full_path="/Devel/NOVAsom_SDK/ExternalFileSystems/U5/"+ui->ExtFS_comboBox->currentText();
    if ( ui->Board_comboBox->currentText() == "M8")
        full_path="/Devel/NOVAsom_SDK/ExternalFileSystems/M8/"+ui->ExtFS_comboBox->currentText();
    if ( ui->Board_comboBox->currentText() == "P Series")
        full_path="/Devel/NOVAsom_SDK/ExternalFileSystems/P/"+ui->ExtFS_comboBox->currentText();

    out << QString("./flash_extfs /dev/"+ui->ExtFS_uSD_Device_comboBox->currentText()+" "+full_path+" > /Devel/NOVAsom_SDK/Logs/extfs.log \n");
    scriptfile.close();
    if ( run_script() == 0)
    {
        update_status_bar("File System "+ui->ExtFS_comboBox->currentText()+" written");
    }
    else
        update_status_bar("File System Creation error");
}

void NOVAembed::on_UserBSPFSelect_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select BSPF"), "/Devel/NOVAsom_SDK/DtbUserWorkArea/PClass_bspf",tr("BSPF (*.bspf)"));
    if (fileName.isEmpty())
        return;
    else
    {
        ui->UserBSPFselectedlineEdit->setText(fileName);
        if ( ui->Board_comboBox->currentText() == "M8")
            Last_M8_BSPFactoryFile = ui->UserBSPFselectedlineEdit->text();
        if ( ui->Board_comboBox->currentText() == "P Series")
            Last_P_BSPFactoryFile = ui->UserBSPFselectedlineEdit->text();
        if ( ui->Board_comboBox->currentText() == "U5")
            Last_U_BSPFactoryFile = ui->UserBSPFselectedlineEdit->text();
        storeNOVAembed_ini();
    }
}

void NOVAembed::on_UserAutoRun_checkBox_toggled(bool checked)
{
    if ( checked == true)
    {
        AutoRunSelected = "true";
        ui->UserAutoRunSelect_pushButton->setEnabled(true);
        ui->UserAutoRunSelectedlineEdit->setEnabled(true);
        ui->UserAutoRunSelectedlineEdit->setText(AutoRunFolder);
        ui->Write_AutoRun_pushButton->setEnabled(true);
    }
    else
    {
        AutoRunSelected = "false";
        ui->UserAutoRunSelect_pushButton->setEnabled(false);
        ui->UserAutoRunSelectedlineEdit->setEnabled(false);
        ui->Write_AutoRun_pushButton->setEnabled(false);
    }
    storeNOVAembed_ini();
}

void NOVAembed::on_UserAutoRunSelect_pushButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select the AutoRun folder"),"/Devel/NOVAsom_SDK",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->UserAutoRunSelectedlineEdit->setText(directory);
    AutoRunFolder = directory;
    ui->Write_AutoRun_pushButton->setEnabled(true);
    storeNOVAembed_ini();
}

void NOVAembed::on_Write_AutoRun_pushButton_clicked()
{
    uSD_Device = ui->uSD_Device_comboBox->currentText();
    QFile scriptfile("/tmp/script");
    update_status_bar("Writing AutoRun using folder "+ui->UserAutoRunSelectedlineEdit->text()+" ...");

    if ( ! scriptfile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        update_status_bar("Unable to create /tmp/script");
        return;
    }
    QTextStream out(&scriptfile);
    out << QString("#!/bin/sh\n");
    out << QString("cd /Devel/NOVAsom_SDK/Utils\n");
    out << QString("./store_application_storage "+ui->UserAutoRunSelectedlineEdit->text()+" /dev/"+uSD_Device+" >> /Devel/NOVAsom_SDK/Logs/uSD_Write\n");
    scriptfile.close();

    if ( run_script() == 0)
    {
        update_status_bar("AutoRun successfully written with folder "+ui->UserAutoRunSelectedlineEdit->text());
    }
    else
        update_status_bar("AutoRun Write error");
}

void NOVAembed::on_PrimaryVideo_comboBox_currentTextChanged(const QString &arg1)
{
    CurrentPrimaryVideo = ui->PrimaryVideo_comboBox->currentText();
}

void NOVAembed::on_ViewBootLog_pushButton_clicked()
{
    if ( ui->Board_comboBox->currentText() == "P Series")
        system("kwrite /Devel/NOVAsom_SDK/Logs/umakeP.log");
    if ( ui->Board_comboBox->currentText() == "U5")
        system("kwrite /Devel/NOVAsom_SDK/Logs/umakeU.log");
    if ( ui->Board_comboBox->currentText() == "M8")
        system("kwrite /Devel/NOVAsom_SDK/Logs/umakeM8.log");
    if ( ui->Board_comboBox->currentText() == "M9")
        system("kwrite /Devel/NOVAsom_SDK/Logs/umakeM9.log");
}

void NOVAembed::on_ViewFSLog_pushButton_clicked()
{
    if ( ui->Board_comboBox->currentText() == "P Series")
        system("kwrite /Devel/NOVAsom_SDK/Logs/FileSystem_Pmake.log");
    if ( ui->Board_comboBox->currentText() == "U5")
        system("kwrite /Devel/NOVAsom_SDK/Logs/FileSystem_Umake.log");
    if ( ui->Board_comboBox->currentText() == "M8")
        system("kwrite /Devel/NOVAsom_SDK/Logs/FileSystem_M8make.log");
    if ( ui->Board_comboBox->currentText() == "M9")
        system("kwrite /Devel/NOVAsom_SDK/Logs/FileSystem_M9make.log");
}

void NOVAembed::on_ViewKernelLog_pushButton_clicked()
{
    system("kwrite /Devel/NOVAsom_SDK/Logs/kmake.log");
}



void NOVAembed::on_ViewuSDwriteLog_pushButton_clicked()
{
    system("kwrite /Devel/NOVAsom_SDK/Logs/uSD_Write.log");
}



void NOVAembed::on_ViewPreCompiledLog_pushButton_clicked()
{
    system("kwrite /Devel/NOVAsom_SDK/Logs/extfs.log");
}




/*****************************************************************************************************************************************************************************************/
/*                                                                             BKF Tab END                                                                                               */
/*****************************************************************************************************************************************************************************************/
