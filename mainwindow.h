#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logviewer.h"
#include "noticewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 初始化UI界面
    void initUI();
    // 初始化驱动
    void initDriver();
    // 初始化游戏
    void initGame();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    // 日志控件
    LogViewer* logger = nullptr;
    void showFloatingNotice(const QString& message, NoticeWidget::NoticeType type);
};
#endif // MAINWINDOW_H
