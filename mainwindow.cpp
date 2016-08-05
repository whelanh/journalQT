#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextDocument>
#include <QMessageBox>
#include <QSettings>

static QString PATH = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Find and remember location of journalSqlite.sqlite database
    QSettings MySettings;
    // qDebug() << MySettings.fileName();
    // qDebug() << MySettings.value("path").toString();
    PATH = MySettings.value("path").toString();
    // qDebug() << PATH << PATH.length();

    if (PATH.length() == 0)
    {
        QString SelectedFile = QFileDialog::getOpenFileName(
                    this, "Select journalSqlite.sqlite" );

        if (!SelectedFile.isEmpty()) {

            MySettings.setValue("path", SelectedFile);
            //qDebug() << MySettings.value("path").toString();
            PATH = SelectedFile;
        }
    }

    db = new DbManager(PATH);

    if (db->isOpen()) {
        db->getLastRecord();
        ui->textEdit->setText(db->lastEntry);
        ui->dateEdit->setDate(QDate::fromString(db->lastDate,"yyyy-MM-dd"));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_writeButton_clicked()
{
    QString dt = ui->dateEdit->date().toString("yyyy-MM-dd");
    int id = ui->dateEdit->date().toJulianDay() - 2455711;
    QString month(QDate::longMonthName(ui->dateEdit->date().month()));
    int day = ui->dateEdit->date().day();
    int year = ui->dateEdit->date().year();
    QString dayOfWeek(QDate::longDayName(ui->dateEdit->date().day()));
    QString entry = ui->textEdit->toPlainText();
    //qDebug() << "id" << id << "date" << dt << "month" << month
    //        << "day" << day << "year" << year << "dayOfWeek" << dayOfWeek;
    db->writeRecord(id, dt, month,day, year, dayOfWeek, entry);
}

void MainWindow::on_dateEdit_dateChanged(const QDate &date)
{
    db->getRecordOnDate(date);
    ui->textEdit->setText(db->lastEntry);
}

void MainWindow::on_similarDatesButton_clicked()
{
    QString result = db->similarDateQuery(ui->dateEdit->date());
    ui->textEdit->setText(result);
}

void MainWindow::on_searchButton_clicked()
{
    QString term = ui->lineEdit->text();
    QString result = db->searchTermQuery(term);
    ui->textEdit->setText(result);
}

void MainWindow::on_weightHistoryButton_clicked()
{
    QMap<QString, QString> map = db->getWeightRecord();
    QMapIterator<QString, QString> i(map);
    QString answer;
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save To CSV", "weightHistory.csv", "CSV files (.csv);;Zip files (.zip, *.7z)", 0, 0); // getting the filename (full path)
    QFile data(filename);
    if(data.open(QFile::WriteOnly |QFile::Truncate)) {
        QTextStream output(&data);

        while (i.hasNext()) {
            i.next();
            answer.append(i.key() + " " + i.value() + "\n");
            output << i.key() << "," << i.value().toFloat() <<  "\n";
            // //qDebug() << i.key() << ": " << i.value() << endl;
        }
    }
    ui->textEdit->setText(answer);
}

void MainWindow::on_exportHtmlButton_clicked()
{
    QString tmp = db->printAllRecords();
    QTextDocument mdoc(tmp);
    QString filename = QFileDialog::getSaveFileName(this, "Save to HTML", "journal.html","html files (.html);" , 0, 0); // getting the filename (full path)
    QFile data(filename);
    if(data.open(QFile::WriteOnly |QFile::Truncate)) {
        QTextStream output(&data);
        output.setCodec( "utf-8" );

        output << mdoc.toHtml("utf-8");
    }
}
