#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QWindow>
#include <QWidget>
#include <executor.h>
#include <QDataStream>
#include <QPalette>

MyProcess::MyProcess(QObject *parent, TabWidget *widget, QString &processPath, QString name):QProcess(parent)
{
	
    connect(this,SIGNAL(readyRead()),
            this,SLOT(myReadyRead()));
    connect(this,SIGNAL(readyReadStandardOutput()),
            this,SLOT(myReadyReadStandardOutput()));
    connect(this,SIGNAL(started()),
            this,SLOT(myProcessStarted()));
    connect(this,SIGNAL(winIdChanged(int)),
            widget,SLOT(createNewTab(int)));
    name_ = name;
    winId_ = 0;
    widget_ = widget;
    processPath_ = processPath;
}

void MyProcess::myProcessStarted(){
    qDebug() << Q_FUNC_INFO;
}
void MyProcess::myReadyRead() {
    qDebug() << Q_FUNC_INFO;
}
//using namespace std;
void MyProcess::myReadyReadStandardOutput() {
    qDebug() << Q_FUNC_INFO;
    int winId = 0; 
    QByteArray out = this->readAllStandardOutput();
    QString str(out.toStdString().c_str());
    winId = str.toInt();
    globWinIds.push_back( winId );
    emit winIdChanged(winId);

}

TabWidget::TabWidget(QWidget *parent ):QTabWidget(parent){
    //setWindowState(Qt::WindowFullScreen);
    QObject::connect(this,SIGNAL(tabBarClicked(int)),
            this,SLOT(tabclicked(int)));
    //QObject::connect(this,SIGNAL(tabAdded()),
    //        this,SLOT(startProcess()));
    //QObject::connect(this,SIGNAL(tabAdded()),
    //        this,SLOT(showMaximized()));
	isProcessStarted = false;
    
    qbitWidget_ = new QWidget();
    qbitLayout_ = new QVBoxLayout(qbitWidget_);
    qbitLayout_->setMargin(0);
    
    lobstexWidget_ = new QWidget();
    lobstexLayout_ = new QVBoxLayout(lobstexWidget_);
    lobstexLayout_->setMargin(0);

    addTab(qbitWidget_, " ");
    addTab(lobstexWidget_, " ");
}

void TabWidget::tabclicked(int index ){
	qDebug()<<"tabbar clicled"<<index;
	if( index == 1 && !isProcessStarted ){
		isProcessStarted = true;
        startProcess( 1 );
	}
}
void TabWidget::startProcess(){
    qDebug() << Q_FUNC_INFO;

}
void TabWidget::setProcess(MyProcess **process){
    process_[0] = process[0];
    process_[1] = process[1];
    setTabText(0, process_[0]->name());
    setTabText(1, process_[1]->name());
}
void TabWidget::createNewTab(int winId){
    //qDebug() << Q_FUNC_INFO;
    qDebug() << winId;
    //showMaximized();
    qDebug()<<"current thread"<<QThread::currentThreadId() ;
    if ( globWinIds.size() == 1 ) {
        QWindow *window = QWindow::fromWinId( winId );
        qbitLayout_->addWidget(QWidget::createWindowContainer(window));
        qbitLayout_->update();
        //qbitWidget_->setLayout(qbitLayout_);
        //startProcess(1);
        //process_[1]->waitForStarted(3000);
    	//process_[1]->start( process_[1]->processPath() );
        //emit tabBarClicked(0);

        //tab1->show();
        //viewport()->repaint();
        //startProcess(1);
        //emit tabAdded();
    }
    if ( globWinIds.size() == 2 ){
        QWindow *window = QWindow::fromWinId( winId );
        //window->hide();
        lobstexLayout_->addWidget(QWidget::createWindowContainer(window));
        lobstexLayout_->update();
        //showMaximized();
    }
}

/*
void TabWidget::resizeEvent( QResizeEvent *rsz ){
    qDebug() << Q_FUNC_INFO;
    qDebug() << rsz;
    resize(rsz->size());
}*/

bool TabWidget :: event(QEvent *e) {
    //qDebug()<<Q_FUNC_INFO;
    //qDebug()<<e;
    return QWidget::event(e);
}
void TabWidget::closeEvent (QCloseEvent *event){
    //qDebug() << Q_FUNC_INFO;
    //qDebug()<<"globWinIds"<<globWinIds;
    process_[0]->close();
    process_[1]->close();
}

void TabWidget::startProcess(int index){
    process_[index]->start(process_[index]->processPath());
}
/*
void gui_wrapper::paintEvent(QPaintEvent* aEvent)
{
    QStyleOption Opt;
    Opt.init(this);
    QPainter p(this); 
    style()->drawPrimitive(QStyle::PE_Widget,&Opt,&p,this);
}
*/

#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TabWidget *tab=new TabWidget( nullptr );
    tab->resize(1080,700);
    QString lobstex = "../lobstex-2.0/src/qt/lobstex-qt";
    QString qbit = "../qBittorrent-4_1_x/src/qbittorrent";
    MyProcess *myProcess [2];
    myProcess [0] = new MyProcess(tab, tab, qbit, "qbittorrent");
    myProcess [1] = new MyProcess(tab->tab2Widget(), tab, lobstex, "lobstex-qt");
   
    //Tab stylesheet
    QFile stylesheet("formStyle.qss");
    stylesheet.open(QFile::ReadOnly);
    QString setSheet = QLatin1String(stylesheet.readAll());
    tab->setStyleSheet(setSheet);
    //end

    tab->setProcess(myProcess);
    tab->startProcess(0);
    //tab->showMaximized();
    tab->show();

    //QTimer::singleShot(6000, myProcess [1], SLOT(runProcess()));
    //tab->startProcess(1);
    return a.exec();
}


