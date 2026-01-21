#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QFile>
#include <QMessageBox>
#include "driver/mgapi.h"
#include "driver/driverload.h"
#include "process/process.h"
#include "mem.h"
#include "gamecall.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUI();

    // 初始化日志控件
    logger = new LogViewer(ui->textEdit);

    // 加载驱动
    initDriver();

    // 初始化游戏
    initGame();

    // 示例日志
    logger->log(LogLevel::Warning, "变色龙保护盾已开启");
    logger->log(LogLevel::Error, "进程伪装已完成");
    logger->log(LogLevel::Debug, "读写功能已开启");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    ui->pushButton_2->setProperty("class","danger");

    // 表格
    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels({"怪物指针","怪物阵营", "怪物类型", "怪物代码","怪物名称","怪物血量","怪物坐标","宽度","高度","碰撞检测","建筑属性"});

    // 菜单
    ui->listWidget->setProperty("class","leftMenu");
    QListWidgetItem *item1 = new QListWidgetItem(QIcon(":/assets/images/icons/home.svg"), "启动");
    ui->listWidget->addItem(item1);
    QListWidgetItem *item2 = new QListWidgetItem(QIcon(":/assets/images/icons/vip.svg"), "定制");
    ui->listWidget->addItem(item2);
    QListWidgetItem *item3 = new QListWidgetItem(QIcon(":/assets/images/icons/safe.svg"), "配置");
    ui->listWidget->addItem(item3);
    QListWidgetItem *item4 = new QListWidgetItem(QIcon(":/assets/images/icons/sata.svg"), "功能");
    ui->listWidget->addItem(item4);
    ui->listWidget->setCurrentRow(0);

    // tab组件
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->hide();

    ui->comboBox->addItem("组包秒杀");
    ui->comboBox->addItem("搬砖");
    ui->comboBox->addItem("升级");

    qApp->setStyleSheet(qApp->styleSheet()); // 重新设置样式表
}

void MainWindow::initDriver()
{
    MGAPI drvControl;

    // 自动加载驱动
    if (!drvControl.isLoadDriver())
    {
        DriverLoad drvLoad;
        MGAPI drvControl;

        QString service_name = "MicrosoftGuard";
        // service_name = Rand::nameWithoutSymbol();

        // QString currentDir = QCoreApplication::applicationDirPath();
        // QString drvPath = currentDir + "/MicrosoftGuard.sys"; // 驱动文件路径
        QString drvPath = "C:/Windows/System32/drivers/MicrosoftGuard.sys"; // 驱动文件路径

        QFile local_driver_file(drvPath);
        if(local_driver_file.exists()){

            // 判断是否可写
            if (!local_driver_file.isWritable()) {
                qDebug() << "驱动文件不可写";

                QFile::Permissions currentPerms = local_driver_file.permissions();
                qDebug() << "修改前权限：" << currentPerms;

                // 添加写权限（移除非写权限）
                if (local_driver_file.setPermissions(currentPerms | QFile::WriteOwner)) {
                    qDebug() << "使用Qt成功修改文件权限";
                } else {
                    qDebug() << "使用Qt修改权限失败：" << local_driver_file.errorString();
                }
            }

            // 判断是否停止
            DriverLoad drvLoad;
            // 停止驱动
            drvLoad.stopDriver(service_name);
            drvLoad.unInstallDriver(service_name);

            qDebug() << "驱动文件已存在，准备删除";
            if(!local_driver_file.remove())
            {
                QMessageBox::information(NULL,"Vega","驱动缓存文件删除失败！");


                QFile::FileError error = local_driver_file.error();
                QString errorStr = local_driver_file.errorString();

                QString message = QString("驱动缓存文件删除失败！\n"
                                          "文件: %1\n"
                                          "错误代码: %2\n"
                                          "错误描述: %3")
                                      .arg(drvPath)
                                      .arg(static_cast<int>(error))
                                      .arg(errorStr);

                // 添加更具体的错误分析
                switch (error) {
                case QFile::PermissionsError:
                    message += "\n原因: 权限不足，请以管理员身份运行程序";
                    break;
                // case QFile::AccessError:
                //     message += "\n原因: 文件可能正在被其他程序使用";
                //     break;
                case QFile::ResizeError:
                    message += "\n原因: 文件系统错误";
                    break;
                case QFile::RemoveError:
                    message += "\n原因: 无法删除文件，检查文件属性和安全设置";
                    break;
                default:
                    break;
                }

                QMessageBox::critical(nullptr, "Vega", message);

                ProcessHelper::deleteViaCommandLine(drvPath);

                return;
            }
        }

        // 释放文件
        QString driver_file_path = ":assets/MicrosoftGuard.sys";
        QFile driver_sys(driver_file_path);
        if(!driver_sys.exists())
        {
            qDebug() << "驱动文件丢失！";
            QMessageBox::information(NULL,"Vega","驱动文件丢失！");
            qApp->quit();
            return;
        }

        qDebug() << "驱动文件已获取，准备下发";

        if (!QFile::copy(driver_file_path, drvPath))
        {
            qDebug() << "驱动文件下发失败！";
            QMessageBox::information(NULL,"Vega","驱动文件下发失败！");
            qApp->quit();
            return;
        }

        qDebug() << "驱动文件已下发";


        if(!drvLoad.installDriver(service_name, drvPath)){
            qDebug() << "驱动安装失败";
            drvLoad.stopDriver(service_name);
            drvLoad.unInstallDriver(service_name);
            QMessageBox::information(NULL,"Vega","驱动安装失败");
            qApp->quit();
            return;
        }

        if(!drvLoad.startDriver(service_name))
        {
            qDebug() << "驱动启动失败";
            drvLoad.stopDriver(service_name);
            drvLoad.unInstallDriver(service_name);
            QMessageBox::information(NULL,"Vega","驱动启动失败");
            qApp->quit();
            return;
        }

        if (!drvControl.isLoadDriver())
        {
            qDebug() << "驱动加载失败！";
            QMessageBox::information(NULL,"Vega","驱动加载失败");
            qApp->quit();
            return;
        }
    }

    if (!drvControl.isLoadDriver())
    {
        logger->log(LogLevel::Info, "变色龙驱动安装失败");
        this->setEnabled(false);
    }else{
        logger->log(LogLevel::Info, "变色龙驱动已安装");
    }



}

void MainWindow::initGame()
{
    Global::game_pid = ProcessHelper::getProcessPID("dnf.exe");
    if(Global::game_pid == NULL){
        logger->log(LogLevel::Info, "未检测到游戏进程");
        this->setEnabled(false);
    }
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    int cur = index.row();
    if(cur > -1 && cur < 4)
    {
        ui->tabWidget->setCurrentIndex(cur);
    }
}

void MainWindow::showFloatingNotice(const QString &message, NoticeWidget::NoticeType type)
{
    auto* notice = new NoticeWidget(message, type, this);
    notice->setAutoClose(3000);  // 自动关闭
    notice->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    notice->setAttribute(Qt::WA_TranslucentBackground);
    notice->setAttribute(Qt::WA_ShowWithoutActivating);

    // 设置大小
    QSize noticeSize = QSize(400, 50);
    notice->resize(noticeSize);

    // 确定主窗口右上角的位置（偏移量可调整）
    const int margin = 20;
    QPoint topRight = mapToGlobal(rect().topRight());
    QPoint pos = QPoint(topRight.x() - noticeSize.width() - margin,
                        topRight.y() + margin);

    notice->move(pos);
    notice->show();
}


void MainWindow::on_pushButton_clicked()
{
    showFloatingNotice("秒杀检测已被干掉！", NoticeWidget::Info);
}


void MainWindow::on_pushButton_2_clicked()
{
    showFloatingNotice("打金检测已被干掉！", NoticeWidget::Success);
}


void MainWindow::on_pushButton_3_clicked()
{
    // Mem::encrypt(Mem::readInt(C_USER_BASE) + C_USER_CUR_HP,100);

    // int value = Mem::memoryAssambly({0xFF, 0x15, 0x88, 0xA5, 0x55, 0x01});
    // GameCall::storeCall();
    Mem::encrypt(0x30EAA508,1);

    // qDebug() << value;
}

