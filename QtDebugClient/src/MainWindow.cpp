#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  QPalette p = m_ui->m_textEdit->palette();
  m_thread = new Thread(m_ui->m_pipeName->text(),this);
  p.setColor(QPalette::Base, QColor(120, 120, 120));
  m_ui->m_textEdit->setPalette(p);
  connect(m_ui->m_connect ,static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked),
            [=]()
            {
              if(m_connected == false)
              {
                m_ui->m_connect->setText("disconnect");
                m_connected=true;

                m_thread->start();
                connect(m_thread ,SIGNAL(text(QColor,QString)),this,SLOT(updateText(QColor,QString)));

              }
              else
              {
                m_ui->m_connect->setText("connect");
                m_connected=false;
                m_thread->stopThread();
//                disconnect(m_thread ,SIGNAL(text(QColor,QString)),this,SLOT(updateText(QColour,QString)));
              //
              }
            });




}

MainWindow::~MainWindow()
{
  delete m_ui;

}


void MainWindow::updateText(QColor c,QString txt)
{
  m_ui->m_textEdit->setTextColor(c);
  m_ui->m_textEdit->append(txt);
}
