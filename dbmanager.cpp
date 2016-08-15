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
       //  qDebug() << "Error: connection with database failed";
    } else
    {
       //  qDebug() << "Database: connection ok";
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

bool DbManager::isValidDatabase()
{
    bool success = true;

    QSqlQuery query("SELECT ID, Date, Entry FROM journal ORDER BY Date DESC LIMIT 1;");
    success = query.exec();
    if(!success) {
       success = false;
    }
    return success;
}

QString DbManager::printAllRecords()
{
    // qDebug() << "all records db:";
    bool success = false;

    QSqlQuery query("SELECT Date, DayOfWeek, Entry FROM journal Order by Date DESC");
    QString answer;
    success = query.exec();

    if (success) {
        answer = queryAnswerFormatter(query);
    }

    return answer;
}

void DbManager::getLastRecord()
{
    // qDebug() << "Last entry in journal:";
    bool success = false;

    QSqlQuery query("SELECT ID, Date, Entry FROM journal ORDER BY Date DESC LIMIT 1;");
    success = query.exec();
    if(!success) {
        // qDebug() << "query failed";
    } else {
        assignClassVariablesWithOneQueryResult(query);
    }

}

void DbManager::assignClassVariablesWithOneQueryResult(QSqlQuery &query) {
    lastEntry = "";
    lastID = 0;
    lastDate = "";
    while (query.next()) {
        int idName = query.record().indexOf("Entry");
        lastEntry = query.value(idName).toString();
        idName = query.record().indexOf("Date");
        lastDate = query.value(idName).toString();
        idName = query.record().indexOf("ID");
        lastID = query.value(idName).toInt();
        // qDebug() << "last ID:" << lastID;
    }
}

void DbManager::getRecordOnDate(const QDate &date)
{
    bool success = false;

    // qDebug() << "Entry on Date:";
    QString dt = date.toString("yyyy-MM-dd");
    // qDebug() << dt;
    QSqlQuery query;
    query.prepare("SELECT ID, Date, Entry FROM journal WHERE Date = (:dt)");
    query.bindValue(":dt", dt);
    success = query.exec();

    if(!success) {
        // qDebug() << "query failed";
    } else {
        assignClassVariablesWithOneQueryResult(query);
    }
}


QString DbManager::searchTermQuery(const QString &term)
{
    bool success = false;

    QSqlQuery query;
    QStringList terms = term.split(",");
    int termNum = terms.size();

    QString q("SELECT Date, DayOfWeek, Entry FROM journal WHERE (journal.Entry LIKE ?");

    if (termNum > 1) {
        for (int i=1;i<termNum;i++) {
            q.append(" AND journal.Entry LIKE ?");
        }
    }
    q.append(") ORDER BY Date");
    query.prepare(q);

    for (int i=0;i<termNum;i++) {
        query.addBindValue(QString("\%" + terms.at(i) + "\%"));
    }

    qDebug() << q;


    success = query.exec();
    QString answer;

    if (success) {
        answer = queryAnswerFormatter(query);
    }

    return answer;
}


QString DbManager::queryAnswerFormatter(QSqlQuery &qry)
{
    QString answer;
    while(qry.next()) {
        int idName = qry.record().indexOf("Entry");
        QString entry = qry.value(idName).toString();
        idName = qry.record().indexOf("Date");
        QString dtString = qry.value(idName).toString();
        idName = qry.record().indexOf("DayOfWeek");
        QString dowString = qry.value(idName).toString();
        answer.append(dtString + " " + dowString + "\n");
        answer.append(entry + "\n");
        answer.append("--------------------------------------------------");
        answer.append("\n");
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
        answer = queryAnswerFormatter(query);
    }
    return answer;
}

void DbManager::updateQuery(QString item, QString val, int id)
{
    bool success = false;

    QSqlQuery qupdate;
    QString q("UPDATE journal SET ");
    q.append(item);
    q.append("= (:etry) WHERE ID = (:idin);");
    qupdate.prepare(q);
    qupdate.bindValue(":etry", val);
    qupdate.bindValue(":idin", id);
    success = qupdate.exec();

    if (success) {
        // qDebug() << "existing record updated";
    } else {
        // qDebug() << "existing record update failed" << qupdate.lastError();
    }
}

void DbManager::updateQuery(QString item, int val, int id)
{
    bool success = false;

    QSqlQuery qupdate;
    QString q("UPDATE journal SET ");
    q.append(item);
    q.append("= (:etry) WHERE ID = (:idin);");
    qupdate.prepare(q);
    qupdate.bindValue(":etry", val);
    qupdate.bindValue(":idin", id);
    success = qupdate.exec();

    if (success) {
        // qDebug() << "existing record updated";
    } else {
        // qDebug() << "existing record update failed" << qupdate.lastError();
    }

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
            updateQuery("Entry",entry,id);
            updateQuery("Date", dt, id);
            updateQuery("Month", mnth, id);
            updateQuery("Day", dy, id);
            updateQuery("Year", yr, id);
            updateQuery("DayOfWeek", dyOfWk, id);

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
                // qDebug() << "added new record";
            } else {
                // qDebug() << "new record add failed" << query.lastError();
            }
        }
    } else {
        // qDebug() << "check query failed" << checkQuery.lastError();
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
                    // // qDebug() << "Date:" << date << ": " << word;
                }
            }
        }
    }
    return wtMap;
}




