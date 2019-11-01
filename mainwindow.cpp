#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include<iostream>
#include<QMimeData>
#include<QFileDialog>
#include<fstream>
QString filename;
#define DEBUG 0
using namespace  std;

QList<QChar> strm;
int crs=0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    strm.push_back(QChar('_'));
    ui->label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    ui->label->setWordWrap(true);
    ui->label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    crs=0;
    filename=="";
    statusBar()->addPermanentWidget(&cordBlock);
    cordBlock.setText("Ln 1, Col 1");
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
     if(DEBUG) qDebug()<<("You Pressed Key " )<<ev->key();
    QString str(ev->text());
    if(ev->key()==16777237){
        int exst=0;
        int pos=0;
          for(int i=crs+1;i<strm.size();i++){
              if(strm[i]==QChar('\n')){
                    strm.insert(strm.begin()+i+1,QChar('_'));
                    strm.removeAt(crs);
                    crs=i;
                    break;
              }
        }
    }
    else if(ev->key()==32){
        strm.insert(strm.begin()+crs,QChar(' '));
        crs++;
    }
    else if(ev->key()==16777236){
        if(crs!=strm.size()-1){
            swap(strm[crs],strm[crs+1]);
            crs++;
        }
    }
    else if(ev->key()==16777235){
        for(int i=crs-1;i>=0;i--){
            if(strm[i]==QChar('\n')){
                  strm.insert(strm.begin()+i,QChar('_'));
                  strm.removeAt(crs+1);
                  crs=i;
                  break;
            }
      }

    }
    else if(ev->key()==16777234){
        if(crs!=0){
            swap(strm[crs],strm[crs-1]);
            crs--;
        }
    }
    else if(ev->key()==16777233){
        strm.removeAt(crs);
        strm.push_back(QChar('_'));
        crs=strm.size()-1;
    }
    else if(ev->key()==16777238){

    }
    else if(ev->key()==16777223){
        if(crs+1<strm.size())
            strm.removeAt(crs+1);
    }
    else if (ev->key()==16777249) {

    }
    else if(ev->key()==16777239){

    }
    else if(ev->key()==16777248){

    }
    else if(ev->key()==16777252){

    }

    else if(ev->text()=="\u001b"){

    }
    //PASTE
    else if(ev->text()=="\u0016"){

            const QClipboard *clipboard = QApplication::clipboard();
            const QMimeData *mimeData = clipboard->mimeData();
            if(mimeData->hasText()){
                QString str=mimeData->text();
                QList<QChar> mlst;
                for(int i=0;i<str.size();i++){
                    mlst.push_back(str[i]);
                    strm.insert(crs,str[i]);
                    crs++;
                }
            }
    }
    //COPY
    else if (ev->text()=="\u0003") {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->label->selectedText());
    }
    //CUT
    else if (ev->text()=="\u0018") {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->label->selectedText());
        int startSelection=ui->label->selectionStart();
        int endSelection=startSelection+ui->label->selectedText().size()-1;
        if(startSelection>crs){
            int sz=ui->label->selectedText().size();
            while(sz--){
                strm.removeAt(startSelection);
            }
        }
        if(endSelection<crs){
            int sz=ui->label->selectedText().size();
            while(sz--){
                strm.removeAt(startSelection);
                crs--;
            }
        }
    }
    else if(ev->text()=="\b"){
        if(strm.size()){
            if(crs!=0)
           { strm.removeAt(crs-1); crs--;}
        }
    }
    else if(ev->text()=="\r"){
        strm.insert(strm.begin()+crs,QChar('\n'));
        crs++;
    }

   else {
        if(str.at(0)!=16777248){
        strm.insert(strm.begin()+crs,str.at(0));
         if(DEBUG) qDebug()<<"WARNING == "<<str;
        crs++;
        }
        else {
            if(str.size()>1){
                strm.insert(strm.begin()+crs,str.at(1));
                if(DEBUG) qDebug()<<"WARNING == "<<str;
                crs++;
            }
            else {
               if(DEBUG) qDebug()<<"WARNING == "<<str;
            }
        }
    }
    QString s="";
    for(int i=0;i<strm.size();i++){
        s.append(QChar( strm[i]));
    }
     if(DEBUG) qDebug()<<s<<endl;
    ui->label->setText(s);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    int colCount = 0;
    int rowCount = 1;
    qDebug()<<"Hi";
    for(int i=0;i<strm.size();i++){
        colCount++;
        if(strm[i]=='\n'){
            rowCount++;
            colCount = 0;
        }
        else if(strm[i]=='_'){
            break;
        }
    }
    QString str;
    str.sprintf("Ln %2d, Col %2d", rowCount, colCount);
    cordBlock.setText(str);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionCut_triggered()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->label->selectedText());
    int startSelection=ui->label->selectionStart();
    int endSelection=startSelection+ui->label->selectedText().size()-1;
    if(startSelection>crs){
        int sz=ui->label->selectedText().size();
        while(sz--){
            strm.removeAt(startSelection);
        }
    }
    if(endSelection<crs){
        int sz=ui->label->selectedText().size();
        while(sz--){
            strm.removeAt(startSelection);
            crs--;
        }
    }
    QString s="";
    for(int i=0;i<strm.size();i++){
        s.append(QChar( strm[i]));
    }
    if(DEBUG) qDebug()<<s<<endl;
    ui->label->setText(s);
}

void MainWindow::on_actionCopy_triggered()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->label->selectedText());
    QString s="";
    for(int i=0;i<strm.size();i++){
        s.append(QChar( strm[i]));
    }
    if(DEBUG) qDebug()<<s<<endl;
    ui->label->setText(s);
}

void MainWindow::on_actionPaste_triggered()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if(mimeData->hasText()){
        QString str=mimeData->text();
        QList<QChar> mlst;
        for(int i=0;i<str.size();i++){
            mlst.push_back(str[i]);
            strm.insert(crs,str[i]);
            crs++;
        }
    }
    QString s="";
    for(int i=0;i<strm.size();i++){
        s.append(QChar( strm[i]));
    }
     if(DEBUG) qDebug()<<s<<endl;
    ui->label->setText(s);
}

void MainWindow::on_actionSave_triggered()
{
    if(filename==""){
        on_actionSave_As_triggered();
    }
    else {
        std::string myfile = filename.toUtf8().constData();
        fstream f;
        f.open(myfile, ios::out);

            for(int i=0;i<strm.size();i++){
                if(i!=crs)
                {
                    char c=strm[i].unicode();
                    f<<c;
                }
        }
            f.close();
    }

}

void MainWindow::on_actionSave_As_triggered()
{
    QString selFilter="All files (*.*)";
    filename = QFileDialog::getSaveFileName(this,"Save file",QDir::currentPath(),
        "Text files (*.txt);;All files (*.*)",&selFilter);
     QWidget::setWindowTitle ( filename + " - Notepad");

     std::string myfile = filename.toUtf8().constData();
     fstream f;
     f.open(myfile, ios::out);

         for(int i=0;i<strm.size();i++){
             if(i!=crs)
             {
                 char c=strm[i].unicode();
                 f<<c;
             }
     }
         f.close();
}

void MainWindow::on_actionOpen_triggered()
{
        filename = QFileDialog::getOpenFileName(this,
        tr("Open Text File"), QDir::currentPath(), tr("Text Files (*.txt *.td *.gdt)"));
        QWidget::setWindowTitle ( filename + " - Notepad");
        std::string myfile = filename.toUtf8().constData();
        fstream f;
        f.open(myfile);
        strm.clear();
        while(f){
            char c;
            f>>c;
            strm.push_back(QChar(c));
        }
        strm.pop_back();
        crs=strm.size();
        strm.push_back(QChar('_'));
        QString s="";
        for(int i=0;i<strm.size();i++){
            s.append(QChar( strm[i]));
        }
         if(DEBUG) qDebug()<<s<<endl;
        ui->label->setText(s);
}

void MainWindow::on_actionStatus_Bar_toggled(bool arg1)
{
    if(arg1)
        statusBar()->show();
    else
        statusBar()->hide();
}
