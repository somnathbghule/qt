#ifndef executor_h
#define executor_h

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QTabWidget>
#include <QCloseEvent>
#include <QVector>
#include <QHBoxLayout>
#include <QThread>



class TabWidget;
static QVector <int> globWinIds(0);

class MyProcess : public QProcess
{
    Q_OBJECT

public:
    MyProcess(QObject *parent , TabWidget *widget , QString &processPath, QString name); 
    ~MyProcess() {}
    QString processPath() { return processPath_; }
    QString name() { return name_; }
public slots:
    void myReadyRead();
    void myReadyReadStandardOutput();
    void myProcessStarted();
signals:
    void winIdChanged(int);
private:
    int winId_;
    TabWidget *widget_;
    QString processPath_;
    QString name_;
};

class TabWidget : public QTabWidget{
    Q_OBJECT
public:
    TabWidget(QWidget *parent );
    void closeEvent ( QCloseEvent *event );
    void setProcess(MyProcess **process);
    MyProcess *process_[2];
    //void resizeEvent( QResizeEvent *resize );
    bool event(QEvent *e);
    void startProcess( int index );
    QWidget *tab2Widget() { return tab2Widget_; }
public slots:
	void tabclicked(int);
    void createNewTab(int);
    void startProcess();
    void runProcess () { startProcess(1); }
signals:
    void tabAdded();
private:
	bool isProcessStarted;
    QVBoxLayout *layout_;
    QWidget *tab2Widget_;
};


#endif 
