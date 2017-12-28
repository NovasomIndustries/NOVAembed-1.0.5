#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

namespace Ui {
class NOVAembed;
}

class NOVAembed : public QMainWindow
{
    Q_OBJECT

public:
    explicit NOVAembed(QWidget *parent = 0);
    ~NOVAembed();

private slots:
    /* Helper Functions */
    void storeNOVAembed_ini();
    void P_load_BSPF_File(QString fileName);
    void compile_NewFileSystemFileSystemConfigurationcomboBox();
    void compile_ExtFS_comboBox();
    void local_sleep(int ms);
    int run_script(void);
    int update_status_bar(QString StatusBarContent);
    void P_save_helper( QString filename, QString Processor_model);
    void NOVAsom_Params_helper();
    void initrd_helper(void);

    /* Helper Functions End */

    void on_actionExit_triggered();

    void on_SelectFileSystem_pushButton_clicked();

    void on_tab_currentChanged(int index);

    void on_KernelXconfig_pushButton_clicked();

    void on_BootLoaderCompile_pushButton_clicked();

    void on_KernelCompile_pushButton_clicked();

    void on_LaunchMenuConfig_pushButton_clicked();

    void on_FileSystemDeploy_pushButton_clicked();

    void on_Board_comboBox_currentIndexChanged(const QString &arg1);

    void on_P_Save_pushButton_clicked();

    void on_P_Load_pushButton_clicked();

    void on_UserPartition_comboBox_currentIndexChanged(const QString &arg1);

    void on_Write_uSD_pushButton_clicked();

    void on_CodeBlock_pushButton_clicked();

    void on_Meld_pushButton_clicked();

    void on_P_PD_100K_checkBox_clicked();

    void on_P_PU_100K_checkBox_clicked();

    void on_P_PU_47K_checkBox_clicked();

    void on_P_PU_22K_checkBox_clicked();

    void on_P_PUE_checkBox_clicked();

    void on_P_SPEED_Low_checkBox_clicked();

    void on_P_SPEED_Mid_checkBox_clicked();

    void on_P_SPEED_High_checkBox_clicked();

    void on_P_SRE_Fast_checkBox_clicked();

    void on_P_SRE_Slow_checkBox_clicked();

    void on_P_DSE_Disable_checkBox_clicked();

    void on_P_DSE_34_checkBox_clicked();

    void on_P_DSE_40_checkBox_clicked();

    void on_P_DSE_48_checkBox_clicked();

    void on_P_DSE_60_checkBox_clicked();

    void on_P_DSE_80_checkBox_clicked();

    void on_P_DSE_120_checkBox_clicked();

    void on_P_DSE_240_checkBox_clicked();

    void on_P_PKE_checkBox_clicked();

    void on_P_ODE_checkBox_clicked();

    void on_P_NO_PAD_CTL_checkBox_clicked();

    void on_KernelReCompile_pushButton_clicked();

    void on_QtCreator_pushButton_clicked();

    void on_P_CreateCFGBits_pushButton_clicked();

    void on_P_DecodeCFGBits_pushButton_clicked();

    void on_UserPartition1Size_lineEdit_textChanged(const QString &arg1);

    void on_UserPartition2Size_lineEdit_textChanged(const QString &arg1);

    void on_P_HUM_pushButton_clicked();

    void on_P_QSG_pushButton_clicked();

    void on_LaunchBusyboxMenuConfig_pushButton_clicked();

    void on_uSD_Device_comboBox_currentIndexChanged(const QString &arg1);

    void on_GenerateFileSystem_pushButton_clicked();

    void on_ExtFS_Write_uSD_pushButton_clicked();

    void on_ExtFS_comboBox_currentIndexChanged(const QString &arg1);

    void on_ThisIsReferenceServer_checkBox_clicked(bool checked);

    void on_CheckUpdate_pushButton_clicked();

    void on_P_Generate_pushButton_clicked();

    void on_UserBSPFSelect_pushButton_clicked();

    void on_UserAutoRun_checkBox_toggled(bool checked);

    void on_UserAutoRunSelect_pushButton_clicked();

    void on_P_SetCFGbits_pushButton_clicked();

    void on_Write_AutoRun_pushButton_clicked();

    void on_P_SPI1_checkBox_toggled(bool checked);
    void on_P_Audio4_checkBox_toggled(bool checked);

    void on_P_SPI2_checkBox_toggled(bool checked);

    void on_P_Audio5_checkBox_toggled(bool checked);

    void on_P_SPI1_2ndSScheckBox_toggled(bool checked);

    void on_P_SPI2_2ndSScheckBox_toggled(bool checked);

    void on_P_SPI3_checkBox_toggled(bool checked);

    void on_P_I2C1_OnJ13checkBox_toggled(bool checked);

    void on_P_SPI4_checkBox_toggled(bool checked);

    void on_P_CCMCLKO1_checkBox_toggled(bool checked);

    void on_P_EPIT1_checkBox_toggled(bool checked);

    void on_P_PWM1_checkBox_toggled(bool checked);

    void on_P_WD1_checkBox_toggled(bool checked);

    void on_P_SPDIF_checkBox_toggled(bool checked);

    void on_P_SPDIF_OnJ13checkBox_toggled(bool checked);

    void on_P_SDCARD3_checkBox_toggled(bool checked);

    void on_P_SDCARD3IS8_checkBox_toggled(bool checked);

    void on_P_CAN1_checkBox_toggled(bool checked);

    void on_P_CAN2_checkBox_toggled(bool checked);

    void on_P_I2C1_checkBox_toggled(bool checked);

    void on_P_UART2_checkBox_toggled(bool checked);

    void on_P_UART4_checkBox_toggled(bool checked);

    void on_P_UART2_4WirescheckBox_toggled(bool checked);

    void on_P_UART4_4WirescheckBox_toggled(bool checked);

    void on_P_Audio6_checkBox_toggled(bool checked);

    void on_P_I2C3_checkBox_toggled(bool checked);

    void on_P_UART1_checkBox_toggled(bool checked);

    void on_P_KHZ32_checkBox_toggled(bool checked);

    void on_P_MHZ24_checkBox_toggled(bool checked);

    void P_pin_decode();
    void on_P_Clear_pushButton_clicked();


    void on_KernelSplash_pushButton_clicked();

    void on_KernelCompileSplash_pushButton_clicked();

    void on_PrimaryVideo_comboBox_currentTextChanged(const QString &arg1);

    void on_ViewBootLog_pushButton_clicked();

    void on_ViewFSLog_pushButton_clicked();

    void on_ViewKernelLog_pushButton_clicked();

    void on_ViewuSDwriteLog_pushButton_clicked();

    void on_ViewPreCompiledLog_pushButton_clicked();

    void on_ViewDtbCompileLog_pushButton_clicked();

    void on_AddFileSystemConfig_pushButton_clicked();

    void on_U_Clear_pushButton_clicked();

    void on_U_SetCFGbits_pushButton_clicked();

    void on_U_SPI4_checkBox_toggled(bool checked);

    void on_U_CAN1_checkBox_toggled(bool checked);

    void on_U_I2C2_checkBox_toggled(bool checked);

    void on_U_UART6_checkBox_toggled(bool checked);

    void on_U_Audio1_checkBox_toggled(bool checked);

    void on_U_SPDIF_checkBox_toggled(bool checked);

    void on_U_PWM2_checkBox_toggled(bool checked);

    void on_UPriVideo_24bit_checkBox_toggled(bool checked);

    void on_U_UART5_4WirescheckBox_toggled(bool checked);

    void on_U_Load_pushButton_clicked();

    void on_U_Save_pushButton_clicked();

    void on_U_Generate_pushButton_clicked();

    void on_U_ViewDtbCompileLog_pushButton_clicked();

    void U_load_BSPF_File(QString fileName);

    void U_save_helper(QString fileName);

    void on_ViewUpdatesLog_pushButton_clicked();

    void on_P_HUM_pushButton_3_clicked();


private:
    Ui::NOVAembed *ui;
};

#endif // MAINWINDOW_H
