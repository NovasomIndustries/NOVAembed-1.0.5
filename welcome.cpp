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

extern int initrd_size;

extern QWidget *current_stab;

QString HUM_P="NI150316-HUM-P-V1.0.pdf";
QString QSG_P="NI150316-QSG-P-V1.0.pdf";


void NOVAembed::Board_comboBox_setText(const QString &arg1)
{
    ui->P_HUM_pushButton->setText(arg1+" Hardware User Manual");
    ui->P_QSG_pushButton->setText(arg1+" Hardware User Manual");
}

void NOVAembed::on_Board_comboBox_currentIndexChanged(const QString &arg1)
{
int kernelok=0,bootok=0;
QFileInfo check_file1;
QString file_exists_path;
    if (( arg1 == "P Series") && ( CurrentBSPF_Tab == "P BSP Factory"))
        return;
    if (( arg1 == "U5") && ( CurrentBSPF_Tab == "U BSP Factory"))
        return;
    if (( arg1 == "M8") && ( CurrentBSPF_Tab == "M8 BSP Factory"))
        return;
    if (( arg1 == "M9") && ( CurrentBSPF_Tab == "M9 BSP Factory"))
        return;
    _Board_comboBox = arg1;
    file_exists_path = BLOBS_PATH;

    Board_comboBox_setText(arg1);



    if ( arg1 == "U5")
    {
        CurrentBSPF_Tab = "U BSP Factory";
        current_stab = UBSP_stab;
        Kernel=NXP_U_KERNEL;
        SourceMeFile=NXP_U_SOURCEME;
        file_exists_path += NXP_U_BLOB_NAME;
        check_file1 = QFileInfo(file_exists_path);
        if (check_file1.exists() && check_file1.isFile())
            kernelok=1;
        check_file1 = QFileInfo("/Devel/NOVAsom_SDK/Bootloader/u-boot-novasomU-2016.03/u-boot.imx");
        if (check_file1.exists() && check_file1.isFile())
            bootok=1;
        ui->brand_label->setPixmap(QPixmap(":/Icons/NXP-Logo.png"));
        ui->FileSystemSelectedlineEdit->setText("");
        ui->BoardNameBKF->setText(arg1);
        ui->BrandNameBKF->setPixmap(QPixmap(":/Icons/NXP-Logo.png"));
    }
    if ( arg1 == "P Series")
    {
        ui->FileSystemSelectedlineEdit->setText("");
        CurrentBSPF_Tab = "P BSP Factory";
        current_stab = PBSP_stab;
        ui->UserBSPFSelect_pushButton->setVisible(true);
        ui->UserBSPFselectedlineEdit->setVisible(true);
        Kernel=NXP_P_KERNEL;
        SourceMeFile=NXP_P_SOURCEME;
        file_exists_path += NXP_P_BLOB_NAME;
        check_file1 = QFileInfo(file_exists_path);
        if (check_file1.exists() && check_file1.isFile())
            kernelok=1;
        check_file1 = QFileInfo("/Devel/NOVAsom_SDK/Bootloader/u-boot-novasomP-2015.04/u-boot.img");
        if (check_file1.exists() && check_file1.isFile())
            bootok=1;
        ui->brand_label->setPixmap(QPixmap(":/Icons/NXP-Logo.png"));
        ui->BrandNameBKF->setPixmap(QPixmap(":/Icons/NXP-Logo.png"));
        ui->BoardNameBKF->setText(arg1);
    }
    if ( arg1 == "M8")
    {
        ui->FileSystemSelectedlineEdit->setText("");
        CurrentBSPF_Tab = "M8 BSP Factory";
        current_stab = M8BSP_stab;
        ui->UserBSPFSelect_pushButton->setVisible(true);
        ui->UserBSPFselectedlineEdit->setVisible(true);
        Kernel=QUALCOMM_KERNEL;
        SourceMeFile=QUALCOMM_SOURCEME;
        file_exists_path += QUALCOMM_BLOB_NAME;
        check_file1 = QFileInfo(file_exists_path);
        if (check_file1.exists() && check_file1.isFile())
            kernelok=1;
        check_file1 = QFileInfo("/Devel/NOVAsom_SDK/Bootloader/u-boot-novasomM8-2017.11/u-boot.bin");
        if (check_file1.exists() && check_file1.isFile())
            bootok=1;
        ui->brand_label->setPixmap(QPixmap(":/Icons/Qualcomm_Snapdragon_logo.png"));
        ui->BrandNameBKF->setPixmap(QPixmap(":/Icons/Qualcomm_Snapdragon_logo.png"));
        ui->BoardNameBKF->setText(arg1);
    }
    if ( arg1 == "M9")
    {
        ui->FileSystemSelectedlineEdit->setText("");
        CurrentBSPF_Tab = "M9 BSP Factory";
        current_stab = M9BSP_stab;
        ui->UserBSPFSelect_pushButton->setVisible(true);
        ui->UserBSPFselectedlineEdit->setVisible(true);
        Kernel=ALLWINNER_KERNEL;
        SourceMeFile=ALLWINNER_SOURCEME;
        file_exists_path += ALLWINNER_BLOB_NAME;
        check_file1 = QFileInfo(file_exists_path);
        if (check_file1.exists() && check_file1.isFile())
            kernelok=1;
        check_file1 = QFileInfo("/Devel/NOVAsom_SDK/Bootloader/u-boot-NOVAsomH5-2017.11/H5/u-boot.bin");
        if (check_file1.exists() && check_file1.isFile())
            bootok=1;
        ui->brand_label->setPixmap(QPixmap(":/Icons/allwinnerlogo.png"));
        ui->BrandNameBKF->setPixmap(QPixmap(":/Icons/allwinnerlogo.png"));
        ui->BoardNameBKF->setText(arg1);
    }
    manage_kerneldwnld_decompress();

    /* hide Tools for recompose order */
    ui->tab->removeTab(3);

    ui->tab->removeTab(2);
    ui->tab->insertTab(2,current_stab,CurrentBSPF_Tab);

    /* now show tools again */
    ui->tab->insertTab(3,TOOL_stab,"Tools");

    compile_NewFileSystemFileSystemConfigurationcomboBox();

    if ( bootok == 0 )
    {
        ui->uboot_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
        BootValid = "INVALID";
    }
    else
    {
        ui->uboot_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
        BootValid = "OK";
    }
    ui->fs_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
    if ( kernelok == 0 )
    {
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/invalid.png"));
        KernelValid = "INVALID";
    }
    else
    {
        ui->kernel_Valid_label->setPixmap(QPixmap(":/Icons/valid.png"));
        KernelValid = "OK";
    }
    ui->frame_5->setEnabled(false);
    compile_ExtFS_comboBox();
    storeNOVAembed_ini();
}

void NOVAembed::on_P_HUM_pushButton_clicked()
{
QString syscmd;

    if ( ui->Board_comboBox->currentText() == "P Series")
        syscmd = "qpdfview /Devel/NOVAsom_SDK/Doc/"+HUM_P;
    QByteArray ba = syscmd.toLatin1();
    const char *str = ba.data();
    system(str);

}

void NOVAembed::on_P_QSG_pushButton_clicked()
{
    QString syscmd;

        if ( ui->Board_comboBox->currentText() == "P Series")
            syscmd = "qpdfview /Devel/NOVAsom_SDK/Doc/"+QSG_P;
        QByteArray ba = syscmd.toLatin1();
        const char *str = ba.data();
        system(str);
}

void NOVAembed::on_P_HUM_pushButton_3_clicked()
{
    system("qpdfview /Devel/NOVAsom_SDK/Doc/SUM-NOVAembed-V1.0.pdf");
}
