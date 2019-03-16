#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QWindow>
#include <QWidget>
#include <executor.h>
#include <QDataStream>

MyProcess::MyProcess(QObject *parent, TabWidget *widget):QProcess(parent)
{
	
    connect(this,SIGNAL(readyRead()),
            this,SLOT(myReadyRead()));
    connect(this,SIGNAL(readyReadStandardOutput()),
            this,SLOT(myReadyReadStandardOutput()));
    connect(this,SIGNAL(started()),
            this,SLOT(myProcessStarted()));
    winId_ = 0;
    widget_ = widget;
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
    // Note we need to add \n (it's like pressing enter key)
    //this->write(QString("myname" + QString("\n")).toLatin1());
    // Next line no required
    // qDebug() << this->readAll();
   
    QByteArray out = this->readAllStandardOutput();
    QString str(out.toStdString().c_str());
    winId_ = str.toInt();
    qDebug() << "output: "<< winId_;

    //***QWindow *window = QWindow::fromWinId(winId_);

    //**widget_ = QWidget::createWindowContainer(window);
    //widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //widget->resize(500,500);
    //widget->show();
    //parent_->addTab(widget, QString::number(processId()));
    //parent_->show();
    globWinIds.push_back(winId_);
    if ( globWinIds.size() == 1 ){
        QWindow *window = QWindow::fromWinId(winId_);
        widget_->addTab(QWidget::createWindowContainer(window), "1");
        //widget_->process_[1]->startDetached(widget_->process_[1]->program());
		QWidget *widget = new QWidget();
		widget->resize(500,500);
        widget_->addTab(widget, "2");
    	qDebug()<<"ready process 1"<<widget_->process_[0];
    	qDebug()<<"ready process 2"<<widget_->process_[1];
		winId_=0;
    }
    if( globWinIds.size() == 2 ){
		//widget_->hide();
		//widget_->removeTab(1);
        QWindow *window = QWindow::fromWinId(winId_);
        //widget_->addTab(QWidget::createWindowContainer(window), "2");
		QHBoxLayout *layout = new QHBoxLayout (widget_->currentWidget());
		layout->addWidget(QWidget::createWindowContainer(window));
		//widget_->insertTab(1, QWidget::createWindowContainer(window), "2");
		//widget_->show();
		winId_=0;
    }
}

int MyProcess::winId(){
    return winId_;
}

TabWidget::TabWidget(QWidget *parent, QVector <int> &winids):QTabWidget(parent){
    winIds_ = winids;        
    QObject::connect(this,SIGNAL(tabBarClicked(int)),
            this,SLOT(tabclicked(int)));
	isProcessStarted = false;
}
//void TabWidget::tabBarClicked(int index){
void TabWidget::tabclicked(int index ){
	qDebug()<<"tabbar clicled"<<index;
	if( index == 1 && !isProcessStarted ){
		isProcessStarted = true;
    	process_[1]->start( "../app1/app1" );
		//process_[1]->waitForFinished();
		//removeTab(1);
        //QWindow *window = QWindow::fromWinId(globWinIds[1]);
        //addTab( QWidget::createWindowContainer(window), "2" );
	}
}
void TabWidget::setProcess(MyProcess **process){
    process_[0] = process[0];
    process_[1] = process[1];
}
void TabWidget::closeEvent (QCloseEvent *event){
    qDebug() << Q_FUNC_INFO;
    qDebug()<<"globWinIds"<<globWinIds;
    process_[0]->close();
    process_[1]->close();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qRegisterMetaType< QVector<MyProcess *> >("QVector<MyProcess *>");
    TabWidget *tab=new TabWidget(nullptr, globWinIds);
    tab->resize(500,500);

    MyProcess *myProcess [2];
    myProcess [0] = new MyProcess(nullptr, tab);
    myProcess [1] = new MyProcess(nullptr, tab);
    QString program = "../app1/app1";
    //qDebug()<<"process 1"<<myProcess[0];
    //qDebug()<<"process 2"<<myProcess[1];
    
    //qDebug()<<"globWinIds local"<<globWinIds;
    myProcess[0]->start( program );
    //myProcess[1]->start( program );
	//myProcess[1]->waitForFinished();
    //myProcess[1]->start(program);
    tab->setProcess(myProcess);
    tab->show();
    return a.exec();
}


