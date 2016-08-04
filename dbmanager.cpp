#include "dbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDate>



DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}




QString DbManager::printAllRecords() const
{
    qDebug() << "all records db:";
    QSqlQuery query("SELECT Date, DayOfWeek, Entry FROM journal Order by Date DESC");
    QString answer;
    while (query.next())
    {
        int id = query.record().indexOf("Date");
        QString date = query.value(id).toString();
        id = query.record().indexOf("DayOfWeek");
        QString dayOfWk = query.value(id).toString();
        id = query.record().indexOf("Entry");
        QString entry = query.value(id).toString();
        answer.append(date + " " + dayOfWk + "\n");
        answer.append(entry + "\n");
        answer.append("--------------------------------------------------");
        answer.append("\n");
    }

    return answer;
}

void DbManager::getLastRecord()
{
    qDebug() << "Last entry in journal:";
    QSqlQuery query("SELECT ID, Date, Entry FROM journal ORDER BY Date DESC LIMIT 1;");
    int idName = query.record().indexOf("Entry");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        qDebug() << "===" << name;
        lastEntry = name;
        idName = query.record().indexOf("Date");
        QString lastdate = query.value(idName).toString();
        lastDate = lastdate;
        idName = query.record().indexOf("ID");
        int lastid = query.value(idName).toInt();
        lastID = lastid;
        qDebug() << "last ID:" << lastID;
    }
}

void DbManager::getRecordOnDate(const QDate &date)
{
    bool success = false;

    qDebug() << "Entry on Date:";
    QString dt = date.toString("yyyy-MM-dd");
    qDebug() << dt;
    QSqlQuery query;
    query.prepare("SELECT ID, Date, Entry FROM journal WHERE Date = (:dt)");
    query.bindValue(":dt", dt);
    success = query.exec();

    if(!success) {
        qDebug() << "query failed";
    } else {
        lastEntry = "";
        lastID = 0;
        lastDate = dt;
        while (query.next()) {
            int idName = query.record().indexOf("Entry");
            QString name = query.value(idName).toString();
            qDebug() << "===" << name;
            lastEntry = name;
            idName = query.record().indexOf("Date");
            QString lastdate = query.value(idName).toString();
            lastDate = lastdate;
            idName = query.record().indexOf("ID");
            int lastid = query.value(idName).toInt();
            lastID = lastid;
            qDebug() << "last ID:" << lastID;
        }
    }

}

QString DbManager::searchTermQuery(const QString &term)
{

    bool success = false;

    QSqlQuery query;

    QString q("SELECT Date, DayOfWeek, Entry from journal where (journal.Entry LIKE ?) order by Date");
    query.prepare(q);
    query.addBindValue(QString("\%" + term + "\%"));
    qDebug() << q  << " term:" << term;
    success = query.exec();
    QString answer;

    if (success) {
        while(query.next()) {
            int idName = query.record().indexOf("Entry");
            QString entry = query.value(idName).toString();
            idName = query.record().indexOf("Date");
            QString dtString = query.value(idName).toString();
            idName = query.record().indexOf("DayOfWeek");
            QString dowString = query.value(idName).toString();
            answer.append(dtString + " " + dowString + "\n");
            answer.append(entry + "\n");
            answer.append("--------------------------------------------------");
            answer.append("\n");
        }
    }

    return answer;
}

QString DbManager::similarDateQuery(const QDate &date)
{

    bool success = false;

    QSqlQuery query;
    query.prepare("SELECT Date, DayOfWeek, Entry from journal where Month = (:mnt) "
                  "and Day <= (:dyMax) and Day > (:dyMin) order by Date");
    QString month(QDate::longMonthName(date.month()));
    int dayMax = date.day() + 3;
    int dayMin = date.day() - 3;
    query.bindValue(":mnt", month);
    query.bindValue(":dyMax", dayMax);
    query.bindValue(":dyMin", dayMin);

    success = query.exec();
    QString answer;

    if (success) {
        while(query.next()) {
            int idName = query.record().indexOf("Entry");
            QString entry = query.value(idName).toString();
            idName = query.record().indexOf("Date");
            QString dtString = query.value(idName).toString();
            idName = query.record().indexOf("DayOfWeek");
            QString dowString = query.value(idName).toString();
            answer.append(dtString + " " + dowString + "\n");
            answer.append(entry + "\n");
            answer.append("--------------------------------------------------");
            answer.append("\n");
        }
    }

    return answer;
}


void DbManager::writeRecord(int &id, QString &dt, QString &mnth, int &dy, int &yr,
                            QString &dyOfWk, QString &entry)
{

    bool success = false;
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID, Date FROM journal WHERE ID = (:idin)");
    checkQuery.bindValue(":idin", id);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {

            QSqlQuery qupdate;
            qupdate.prepare("UPDATE journal SET Entry = (:etry) WHERE ID = (:idin);");
            qupdate.bindValue(":idin", id);
            qupdate.bindValue(":etry", entry);
            qupdate.exec();

            qupdate.prepare("UPDATE journal SET Date = (:etry) WHERE ID = (:idin);");
            qupdate.bindValue(":etry", dt);
            qupdate.bindValue(":idin", id);
            qupdate.exec();

            qupdate.prepare("UPDATE journal SET Month = (:etry) WHERE ID = (:idin);");
            qupdate.bindValue(":etry", mnth);
            qupdate.bindValue(":idin", id);
            qupdate.exec();

            qupdate.prepare("UPDATE journal SET Day = (:etry) WHERE ID = (:idin);");
            qupdate.bindValue(":etry", dy);
            qupdate.bindValue(":idin", id);
            qupdate.exec();

            qupdate.prepare("UPDATE journal SET Year = (:etry) WHERE ID = (:idin);");
            qupdate.bindValue(":etry", yr);
            qupdate.bindValue(":idin", id);
            qupdate.exec();

            qupdate.prepare("UPDATE journal SET DayOfWeek = (:etry) WHERE ID = (:idin);");
            qupdate.bindValue(":etry", dyOfWk);
            qupdate.bindValue(":idin", id);
            success = qupdate.exec();

            if (success) {
                qDebug() << "existing record updated";
            } else {
                qDebug() << "existing record update failed" << qupdate.lastError();
            }
        }
        else
        {
            QSqlQuery query;
            query.prepare("INSERT INTO journal(`ID`,`Date`,`Month`,`Day`,`Year`,`DayOfWeek`,`Entry`) "
                          "VALUES (:idin, :dtin, :mnin, :dyin, :yrin, :dowin, :ein);");
            query.bindValue(":idin", id);
            query.bindValue(":dtin", dt);
            query.bindValue(":mnin", mnth);
            query.bindValue(":dyin", dy);
            query.bindValue(":yrin", yr);
            query.bindValue(":dowin", dyOfWk);
            query.bindValue(":ein", entry);
            success = query.exec();
            if (success) {
                qDebug() << "added new record";
            } else {
                qDebug() << "new record add failed" << query.lastError();
            }
        }

    } else {
        qDebug() << "check query failed" << checkQuery.lastError();
    }
}


QMap<QString, QString> DbManager::getWeightRecord()
{
    bool success = false;
    QSqlQuery query("SELECT Date, Entry FROM journal ORDER BY Date;");
    success = query.exec();
    QMap<QString, QString> wtMap;
    if (success) {
        while (query.next()) {
            int idName = query.record().indexOf("Entry");
            QString entry = query.value(idName).toString();
            if (entry.contains("weight", Qt::CaseInsensitive)) {
                QRegularExpression regex("\\d{3}(\\.\\d{1})*");
                QRegularExpressionMatchIterator i = regex.globalMatch(entry);
                int idName = query.record().indexOf("Date");
                QString date = query.value(idName).toString();
                while (i.hasNext()) {
                    QRegularExpressionMatch match = i.next();
                    float myWt = match.captured(0).toFloat();
                    if (myWt < 200.0 && myWt > 140.0 ) {
                        wtMap[date] = match.captured(0);
                        break;
                    }
                    // qDebug() << "Date:" << date << ": " << word;
                }
            }
        }
    }
    return wtMap;
}

QString DbManager::getLastRecordDate() const
{
    QString name;
    qDebug() << "last date in journal";
    QSqlQuery query("SELECT Date, Entry FROM journal ORDER BY Date DESC LIMIT 1;");
    int idName = query.record().indexOf("Date");
    while (query.next())
    {
        name = query.value(idName).toString();

        qDebug() << "===" << name;

    }
    return name;

}


