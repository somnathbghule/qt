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
#include <QMainWindow>


class TabWidget;
static QVector <int> globWinIds(0);

class MyProcess : public QProcess
{
    Q_OBJECT

public:
    MyProcess(QObject *parent , TabWidget *widget , QString &processPath, QString name, QIcon icon);
    ~MyProcess() {}
    QString processPath() { return processPath_; }
    QString name() { return name_; }
    QIcon icon() { return icon_; }
public slots:
    void myReadyRead();
    void myReadyReadStandardOutput();
    void myProcessStarted();
    void runProcess () { start(processPath_); }
signals:
    void winIdChanged(int);
private:
    int winId_;
    TabWidget *widget_;
    QString processPath_;
    QString name_;
    QIcon icon_;
};

class TabWidget : public QTabWidget{
    Q_OBJECT
public:
    TabWidget(QWidget *parent );
    void closeEvent( QCloseEvent *event );
    void setProcess(MyProcess **process);
    MyProcess *process_[2];
    //void resizeEvent( QResizeEvent *resize );
    bool event(QEvent *e);
    void startProcess( int index );
    QWidget *tab2Widget() { return lobstexWidget_; }
public slots:
	void tabclicked(int);
    void createNewTab(int);
    void startProcess();
    void stopProcess();
signals:
    void tabAdded();
private:
	bool isProcessStarted;
    QVBoxLayout *lobstexLayout_;
    QWidget *lobstexWidget_;
    QVBoxLayout *qbitLayout_;
    QWidget *qbitWidget_;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0 );
protected:
    void closeEvent(QCloseEvent* event);
signals:
    void exitApplication();

};

#endif 
