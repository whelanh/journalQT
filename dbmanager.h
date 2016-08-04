#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QDate>
#include <QMap>
#include <QRegExp>
#include <QRegularExpressionMatchIterator>

/**
 * \class DbManager
 *
 * \brief SQL Database Manager class
 *
 * DbManager sets up the connection with SQL database
 * and performs some basics queries. The class requires
 * existing SQL database. You can create it with sqlite:
 * 1. $ sqlite3 journal.db
 * 2. sqilte> CREATE TABLE journal(ID, Date, Month, Day, Year, DayOfWeek, Entry);
 * 3. sqlite> .quit
 */
class DbManager
{
public:
    QString lastEntry;
    QString lastDate;
    int lastID;

    /**
     * @brief Constructor
     *
     * Constructor sets up connection with db and opens it
     * @param path - absolute path to db file
     */
    DbManager(const QString& path);

    /**
     * @brief Destructor
     *
     * Close the db connection
     */
    ~DbManager();

    bool isOpen() const;

    /**
     * @brief Returns a string with all of the records in db
     */
    QString printAllRecords() const;

    /**
     * @brief getLastRecord Entry and puts it in the text Edit box
     * @return
     */
    void getLastRecord();

    /**
     * @brief getLastRecordDate
     * Gets the most recent record from the database
     * @return
     */
    QString getLastRecordDate() const;

    /**
     * @brief getRecordOnDate
     * Returns record on a specific date
     * @return
     */
    void getRecordOnDate(const QDate &date);

    /**
     * @brief getWeightRecord
     * Returns map of dates and weights based on sql query using LIKE
     * Filters numbers (which could be adjusted based on your own weight history)
     * @return
     */
    QMap<QString, QString> getWeightRecord();

    /**
     * @brief writeRecord
     * Writes record to the database if it doesn't exist, otherwise updates all values
     * @param id
     * @param dt
     * @param mnth
     * @param dy
     * @param yr
     * @param entry
     */
    void writeRecord(int &id, QString &dt, QString &mnth, int &dy, int &yr,
                     QString &dyOfWk, QString &entry);

    /**
     * @brief similarDateQuery
     * Returns all entries withing plus/minus 3 days of the current time of year
     * @param date
     * @return
     */
    QString similarDateQuery(const QDate &date);

    /**
     * @brief searchTermQuery
     * Returns all journal entries that contain search term
     * @param term
     * @return
     */
    QString searchTermQuery(const QString &term);

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
