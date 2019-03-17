
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
signals:
    void winIdChanged(int);
private:
    int winId_;
    TabWidget *widget_;
    QString processName_;
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
public slots:
	void tabclicked(int);
    void createNewTab(int);
private:
	bool isProcessStarted;
    QVBoxLayout *layout_;
    QWidget *tab2Widget_;
};


#endif 
