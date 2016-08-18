#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dbmanager.h"
#include <QMainWindow>
#include <QSettings>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /**
     * @brief invalidMessage
     */
    void invalidMessage();

    /**
     * @brief getLastRecordAndSetUi
     */
    void getLastRecordAndSetUi(bool newDatabase);

    /**
     * @brief MainWindow::on_writeButton_clicked
     * writes record to database
     */
    void on_writeButton_clicked();

    /**
     * @brief on_dateEdit_dateChanged
     * @param date
     * Changes textEdit display to match date selected in the dateSelector
     */
    void on_dateEdit_dateChanged(const QDate &date);

    /**
     * @brief on_similarDatesButton_clicked
     * Selects all journal entries from the same period of the calendar year to
     *  see what you were doing in the past
     */
    void on_similarDatesButton_clicked();

    /**
     * @brief on_searchButton_clicked
     * Queries data base for all journal entries containing something like the search term
     */
    void on_searchButton_clicked();

    /**
     * @brief on_weightHistoryButton_clicked
     * If you record your weight in your journal, this will try and retrieve all date/weight pairs
     */
    void on_weightHistoryButton_clicked();

    /**
     * @brief on_exportHtmlButton_clicked
     * Exports journal to html
     */
    void on_exportHtmlButton_clicked();

    /**
     * @brief on_todayButton_clicked
     * Sets date and entry based on today's date
     */
    void on_todayButton_clicked();

private:
    Ui::MainWindow *ui;

    DbManager * db;

    QSettings MySettings;

    QString PATH;

    QString SelectedFile;
};

#endif // MAINWINDOW_H
