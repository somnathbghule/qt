#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QWindow>
#include <QWidget>
#include <executor.h>
#include <QDataStream>
#include <QPalette>

MyProcess::MyProcess(QObject *parent, TabWidget *widget, QString &processName):QProcess(parent)
{
	
    connect(this,SIGNAL(readyRead()),
            this,SLOT(myReadyRead()));
    connect(this,SIGNAL(readyReadStandardOutput()),
            this,SLOT(myReadyReadStandardOutput()));
    connect(this,SIGNAL(started()),
            this,SLOT(myProcessStarted()));
    connect(this,SIGNAL(winIdChanged(int)),
            widget,SLOT(createNewTab(int)));
    winId_ = 0;
    widget_ = widget;
    processName_ = processName;
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
    QObject::connect(this,SIGNAL(tabAdded()),
            this,SLOT(startProcess()));
	isProcessStarted = false;
    tab2Widget_ = new QWidget();
    layout_ = new QVBoxLayout(tab2Widget_);
    layout_->setMargin(0);
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
    //process_[1]->start( process_[1]->processName() );

}
void TabWidget::setProcess(MyProcess **process){
    process_[0] = process[0];
    process_[1] = process[1];
}
void TabWidget::createNewTab(int winId){
    //qDebug() << Q_FUNC_INFO;
    qDebug() << winId;
    qDebug()<<"current thread"<<QThread::currentThreadId() ;
    if ( globWinIds.size() == 1 ) {
        
        QWindow *window = QWindow::fromWinId( winId );
        addTab(QWidget::createWindowContainer(window), process_[0]->processName());
        addTab(tab2Widget_, "2");
        //showMaximized();
        //startProcess(1);
        //emit tabAdded();
        //process_[1]->waitForStarted(3000);
    	//process_[1]->start( process_[1]->processName() );
    }
    if ( globWinIds.size() == 2 ){
        QWindow *window = QWindow::fromWinId( winId );
        //window->hide();
        layout_->addWidget(QWidget::createWindowContainer(window));
        layout_->update();
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
    process_[index]->start(process_[index]->processName());
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
    tab->resize(500,500);
    QString program = "../../2ndpart/app1/app1";
    QString qbit = "../../qBittorrent-master/src/qbittorrent";
    MyProcess *myProcess [2];
    myProcess [0] = new MyProcess(tab, tab, qbit);
    myProcess [1] = new MyProcess(tab->tab2Widget(), tab, program);
   
    //myProcess[0]->start( myProcess[0]->processName() );
    tab->setProcess(myProcess);
    tab->startProcess(0);
    //tab->showFullScreen();
    tab->showMaximized();
    //tab->startProcess(1);
    return a.exec();
}


