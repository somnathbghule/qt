
#ifndef executor_h
#define executor_h

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QTabWidget>
#include <QCloseEvent>
#include <QVector>
#include <QHBoxLayout>

class TabWidget;
static QVector <int> globWinIds(0);

class MyProcess : public QProcess
{
    Q_OBJECT

public:
    MyProcess(QObject *parent , TabWidget *widget , QString &processName); 
    ~MyProcess() {}
    QString processName() { return processName_; }
public slots:
    void myReadyRead();
    void myReadyReadStandardOutput();
    void myProcessStarted();
private:
    int winId_;
    TabWidget *widget_;
    QString processName_;
    QVBoxLayout *layout_;
};


class TabWidget : public QTabWidget{
    Q_OBJECT
public:
    TabWidget(QWidget *parent );
    void closeEvent ( QCloseEvent *event );
    void setProcess(MyProcess **process);
    MyProcess *process_[2];
public slots:
	void tabclicked(int);
private:
	bool isProcessStarted;
};


#endif 
