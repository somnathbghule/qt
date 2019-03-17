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
    layout_ = new QVBoxLayout ;
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

/*
    if ( globWinIds.size() == 1 ){
        QWindow *window = QWindow::fromWinId( winId );
        widget_->addTab(QWidget::createWindowContainer(window), processName_);
		QWidget *widget = new QWidget();
        //widget_->addTab(layout_, processName_);
        widget_->addTab(widget, processName_);

        //widget->resize(300,300);
        //widget->setBackgroundRole(QPalette::Base);
        //widget->setLayout(layout_);
    }
    if( globWinIds.size() == 2 ){
        qDebug() << "output: "<< winId;
        layout_->setMargin(0);
        QWindow *window = QWindow::fromWinId( winId );
        QWidget *pWidget = QWidget::createWindowContainer(window);
        QWidget *widget = widget_->currentWidget();
        //pWidget->setStyleSheet("background-color:red;");
        widget->setLayout(layout_);
        qDebug()<<"process2"<<pWidget;
		layout_->addWidget(pWidget);
        layout_->update();
        //widget_->addTab(pWidget, processName_);
        //pWidget->setFocus();
    }
    qDebug()<<"isEmpty()"<<layout_->isEmpty();
    */
}

TabWidget::TabWidget(QWidget *parent ):QTabWidget(parent){
    //setWindowState(Qt::WindowFullScreen);
    QObject::connect(this,SIGNAL(tabBarClicked(int)),
            this,SLOT(tabclicked(int)));
	isProcessStarted = false;
    tab2Widget_ = new QWidget();
    layout_ = new QVBoxLayout(tab2Widget_);
    layout_->setMargin(0);
}

void TabWidget::tabclicked(int index ){
	qDebug()<<"tabbar clicled"<<index;
	if( index == 1 && !isProcessStarted ){
		isProcessStarted = true;
    	process_[1]->start( process_[0]->processName() );
	}
}
void TabWidget::setProcess(MyProcess **process){
    process_[0] = process[0];
    process_[1] = process[1];
}
void TabWidget::createNewTab(int winId){
    qDebug() << Q_FUNC_INFO;
    qDebug() << winId;
    if ( globWinIds.size() == 1 ) {
        //QWidget *widget = new QWidget;
        //addTab(widget, process_[0]->processName());
        //QVBoxLayout *layout = new QVBoxLayout(widget);
        
        QWindow *window = QWindow::fromWinId( winId );

        addTab(QWidget::createWindowContainer(window), process_[0]->processName());
        //layout->addWidget(QWidget::createWindowContainer(window));
        addTab(tab2Widget_, "2");
    }
    if ( globWinIds.size() == 2 ){
        QWindow *window = QWindow::fromWinId( winId );
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
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TabWidget *tab=new TabWidget( nullptr );
    tab->resize(500,500);

    QString program = "../../2ndpart/app1/app1";
    QString qbit = "../../qBittorrent-master/src/qbittorrent";
    MyProcess *myProcess [2];
    myProcess [0] = new MyProcess(tab, tab, program);
    myProcess [1] = new MyProcess(tab, tab, program);
    
    myProcess[0]->start( myProcess[0]->processName() );
    tab->setProcess(myProcess);
    tab->show();
    //myProcess[1]->start( myProcess[1]->processName() );
    return a.exec();
}


