#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Thread.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  private slots :
    void updateText(QColor c, QString _txt);
  private:
    Ui::MainWindow *m_ui;
    bool m_connected=false;
    Thread *m_thread;
};

#endif // MAINWINDOW_H
