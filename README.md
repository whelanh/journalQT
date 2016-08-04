# journalQT #

### Introduction
journalQT is a Qt Project that provides a GUI front end to add, read, and query personal
journal entries stored in a sqlite database.

### Features
* Write or revise your journal entry in the large text box.
* Entries are added or revised when you press the write button.
* Changing the date in the date selector moves to an existing or new entry corresponding
to that date.

![alt text](screenshots/textEditor8.png "Write your journal entry and insert or update record by pushing the Write button")

* You can see all of your entries from the same time of the year (i.e., what were you doing
last August, or the year before that)
![alt text](screenshots/textEditor10.png "See all entries from the similar period in prior years")

* You can search your journal for all entries with the search term.
![alt text](screenshots/textEditor12.png "See all entries from the similar period in prior years")

* If you record your weight in your journal like weight = 174 or 'My weight was 174 this morning',
the Weight Hist button will create a list of dates and weights.  It will display it and save it
to a csv file.
![alt text](screenshots/textEditor11.png "See all entries from the similar period in prior years")


 
### Dependencies
The class requires an existing SQL database. You can create it with sqlite:
1. $ sqlite3 journal.db
2. sqilte> CREATE TABLE journal(ID, Date, Month, Day, Year, DayOfWeek, Entry);
3. sqlite> .quit

When you run journalQT it will ask for the location of your database, but then remember
it from then on.


### TODO



### License






