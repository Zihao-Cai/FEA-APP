#include "mainwindow.h"


QVector<QLineEdit*> linelist;//打包输入框栏
QVector<QTextBrowser*> rstlist;//打包结果标签栏
Opration op;
Element* ele;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

    rstlist.push_back(ui->strain0);
    rstlist.push_back(ui->strain1);
    rstlist.push_back(ui->stress0);
    rstlist.push_back(ui->stress1);

    ui->loadbox->hide();
    ui->resultbox->hide();
    ui->Eline->setPlaceholderText("输入参数E:");
    ui->vline->setPlaceholderText("输入参数v:");
    ui->tline->setPlaceholderText("输入参数t:");
    linelist.push_back(ui->n0xline);
    linelist.push_back(ui->n0yline);
    linelist.push_back(ui->n1xline);
    linelist.push_back(ui->n1yline);
    linelist.push_back(ui->n2xline);
    linelist.push_back(ui->n2yline);
    linelist.push_back(ui->n3xline);
    linelist.push_back(ui->n3yline);
    linelist.push_back(ui->Eline);
    linelist.push_back(ui->tline);
    linelist.push_back(ui->vline);
    connect(ui->exitbtn,SIGNAL(clicked()),this,SLOT(exitbtn_clicked()));
    connect(ui->loadbtn,SIGNAL(clicked()),this,SLOT(loadbtn_clicked()));
    connect(ui->analysebtn,SIGNAL(clicked()),this,SLOT(analysebtn_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::exitbtn_clicked()
{
    this->close();
}

void MainWindow::loadbtn_clicked()
{
    for(int i=0;i<linelist.size();i++){
        linelist[i]->clear();
    }
    op.setmodel(ui->modelBox->currentText());
    if(op.getmodel() == "矩形"){
        ui->loadbox->show();
        ui->resultbox->show();
        ui->resultpage->hide();
        QRegExp exp("^-?[0-9\\.]+$");
        QValidator *v = new QRegExpValidator(exp);
        for(int i=0;i<linelist.size();i++){
            linelist[i]->setValidator(v);
        }//设置输入框只允许输入实数
        op.setloaddone(true);
    }else{
        QMessageBox box;
        QPixmap img(":/confused.png");
        QIcon icon = QIcon(img);
        box.setWindowTitle("option");
        box.setWindowIcon(icon);
        box.setIcon(QMessageBox::Information);
        box.setStyleSheet("color:blue");
        box.setText("加载失败");
        ui->loadbox->hide();
        ui->resultbox->hide();
        op.setloaddone(false);
        box.exec();
    }
}

void MainWindow::analysebtn_clicked()
{
    if(op.isload()){
    //这里就是加载成功之后的计算、分析模块:
        if(op.getmodel() == "矩形"){
        ele = new Element[2]();
        ele[0].nodes = new Node[4]();
        ele[1].nodes = new Node[4]();
        ele[0].nodes[2].isvalid = false;
        ele[1].nodes[0].isvalid = false;
        for(int j=0;j<2;j++){
            ele[j].E = ui->Eline->text().toDouble();
            ele[j].v = ui->vline->text().toDouble();
            ele[j].t = ui->tline->text().toDouble();
            for(int i=0;i<4;i++){
                if(ele[j].nodes[i].isvalid){
                ele[j].nodes[i].fx = linelist[2*i]->text().toDouble();
                ele[j].nodes[i].fy = linelist[2*i+1]->text().toDouble();
                }
            }
        }
        //设定单元结点、面积等参数
        ele[0].nodes[0].x = 0;
        ele[0].nodes[0].y = 0;
        ele[0].nodes[1].x = 1;
        ele[0].nodes[1].y = 0;
        ele[0].nodes[3].x = 0;
        ele[0].nodes[3].y = 1;
        ele[1].nodes[1].x = 1;
        ele[1].nodes[1].y = 0;
        ele[1].nodes[2].x = 1;
        ele[1].nodes[2].y = 1;
        ele[1].nodes[3].x = 0;
        ele[1].nodes[3].y = 1;

        //建立单元等效结点
        ele[0].eq_nodes[0] = *(ele[0].nodes);
        ele[0].eq_nodes[1] = *(ele[0].nodes+1);
        ele[0].eq_nodes[2] = *(ele[0].nodes+3);
        ele[1].eq_nodes[0] = *(ele[1].nodes+1);
        ele[1].eq_nodes[1] = *(ele[1].nodes+2);
        ele[1].eq_nodes[2] = *(ele[1].nodes+3);
        ele[0].A = (ele[0].eq_nodes[0].x*ele[0].eq_nodes[1].y+ele[0].eq_nodes[1].x*ele[0].eq_nodes[2].y+ele[0].eq_nodes[2].x*ele[0].eq_nodes[0].y
                -ele[0].eq_nodes[2].x*ele[0].eq_nodes[1].y-ele[0].eq_nodes[1].x*ele[0].eq_nodes[0].y-ele[0].eq_nodes[0].x*ele[0].eq_nodes[2].y)/2;//A为单元面积
        ele[1].A = (ele[1].eq_nodes[0].x*ele[1].eq_nodes[1].y+ele[1].eq_nodes[1].x*ele[1].eq_nodes[2].y+ele[1].eq_nodes[2].x*ele[1].eq_nodes[0].y
                -ele[1].eq_nodes[2].x*ele[1].eq_nodes[1].y-ele[1].eq_nodes[1].x*ele[1].eq_nodes[0].y-ele[1].eq_nodes[0].x*ele[1].eq_nodes[2].y)/2;

        for(int m=0;m<3;m++){
            ele[0].eq_a[m] = ele[0].eq_nodes[(m+1)%3].x*ele[0].eq_nodes[(m+2)%3].y-ele[0].eq_nodes[(m+2)%3].x*ele[0].eq_nodes[(m+1)%3].y;
            ele[1].eq_a[m] = ele[1].eq_nodes[(m+1)%3].x*ele[1].eq_nodes[(m+2)%3].y-ele[1].eq_nodes[(m+2)%3].x*ele[1].eq_nodes[(m+1)%3].y;
            ele[0].eq_b[m] = ele[0].eq_nodes[(m+1)%3].y - ele[0].eq_nodes[(m+2)%3].y;
            ele[0].eq_c[m] = ele[0].eq_nodes[(m+2)%3].x - ele[0].eq_nodes[(m+1)%3].x;
            ele[1].eq_b[m] = ele[1].eq_nodes[(m+1)%3].y - ele[1].eq_nodes[(m+2)%3].y;
            ele[1].eq_c[m] = ele[1].eq_nodes[(m+2)%3].x - ele[1].eq_nodes[(m+1)%3].x;
        }

        //调用分析方法计算结果
        if(analyse(ele)){

            //下面调取各单元delta属性对指定点求位移,另通过ui显示分析结果
            QString strain0 = QString::number(ele[0].epson[0],'f',3) + ", " + QString::number(ele[0].epson[1],'f',3) +
                    ", " + QString::number(ele[0].epson[2],'f',3);
            QString strain1 = QString::number(ele[1].epson[0],'f',3) + ", " + QString::number(ele[1].epson[1],'f',3) +
                    ", " + QString::number(ele[1].epson[2],'f',3);
            QString stress0 = QString::number(ele[0].sigma[0],'f',3) + ", " + QString::number(ele[0].sigma[1],'f',3) +
                    ", " + QString::number(ele[0].sigma[2],'f',3);
            QString stress1 = QString::number(ele[1].sigma[0],'f',3) + ", " + QString::number(ele[1].sigma[1],'f',3) +
                    ", " + QString::number(ele[1].sigma[2],'f',3);
            ui->strain0->setText(strain0);
            ui->strain1->setText(strain1);
            ui->stress0->setText(stress0);
            ui->stress1->setText(stress1);
            ui->resultpage->setTabText(0,"单元0");
            ui->resultpage->setTabText(1,"单元1");
            ui->resultpage->setTabShape(QTabWidget::Triangular);
            //求指定结点2,3位移向量
            double n2[3],n3[3],n3_[3];
            for(int i=0;i<3;i++){
                n2[i] = (1.0/2.0/ele[1].A)*(ele[1].eq_a[i]+ele[1].eq_b[i]*ele[1].nodes[2].x+ele[1].eq_c[i]*ele[1].nodes[2].y);
                n3[i] = (1.0/2.0/ele[1].A)*(ele[1].eq_a[i]+ele[1].eq_b[i]*ele[1].nodes[3].x+ele[1].eq_c[i]*ele[1].nodes[3].y);
                n3_[i] = (1.0/2.0/ele[0].A)*(ele[0].eq_a[i]+ele[0].eq_b[i]*ele[0].nodes[3].x+ele[0].eq_c[i]*ele[0].nodes[3].y);
            }
            //N矩阵构造//
            double** N2 = new double*[2]();
            double** N3 = new double*[2]();
            double** N3_ = new double*[2]();
            for(int i=0;i<2;i++){
                N2[i] = new double[6]();
                N3[i] = new double[6]();
                N3_[i] = new double[6]();
            }
            for(int j=0;j<3;j++){
                *(*(N2+0)+2*j) = n2[j];*(*(N2+0)+2*j+1) = 0;
                *(*(N2+1)+2*j) = 0;*(*(N2+1)+2*j+1) = n2[j];
                *(*(N3+0)+2*j) = n3[j];*(*(N3+0)+2*j+1) = 0;
                *(*(N3+1)+2*j) = 0;*(*(N3+1)+2*j+1) = n3[j];
                *(*(N3_+0)+2*j) = n3_[j];*(*(N3_+0)+2*j+1) = 0;
                *(*(N3_+1)+2*j) = 0;*(*(N3_+1)+2*j+1) = n3_[j];
            }
            //N矩阵构造完成//
            double** tempdelta0 = new double*[6]();
            double** tempdelta1 = new double*[6]();
            double** disp2 = new double*[2]();
            double** disp3 = new double*[2]();
            double** disp3_ = new double*[2]();
            for(int i=0;i<6;i++){
                tempdelta0[i] = new double();
                tempdelta1[i] = new double();
                if(i<3){
                    disp2[i] = new double();
                    disp3[i] = new double();
                    disp3_[i] = new double();
                }
            }
            for(int i=0;i<6;i++){
                tempdelta0[i][0] = ele[0].delta[i];
                tempdelta1[i][0] = ele[1].delta[i];
            }
            matrix_multiplication(N2,tempdelta1,2,6,6,1,disp2);
            matrix_multiplication(N3,tempdelta1,2,6,6,1,disp3);
            matrix_multiplication(N3_,tempdelta0,2,6,6,1,disp3_);
            for(int i=0;i<2;i++){
                disp3[i][0] = (disp3[i][0]+disp3_[i][0])/2.0;
            }
            //位移求解完成
            QString dispnode2 = "结点2位移为:" + QString::number(disp2[0][0],'f',3) + ", " + QString::number(disp2[1][0],'f',3) + "\n";
            QString dispnode3 = "结点3位移为:" + QString::number(disp3[0][0],'f',3) + ", " + QString::number(disp3[1][0],'f',3) + "\n";
            QMessageBox boxt;
            boxt.setIcon(QMessageBox::Information);
            boxt.setText(QString(dispnode2).append(dispnode3));
            boxt.setTextInteractionFlags(Qt::TextSelectableByMouse);//设置该对话框文本可复制
            QFont font;font.setPointSize(10);
            boxt.setFont(font);

            QMessageBox box;
            QPixmap img(":/emoticon.png");
            QIcon icon = QIcon(img);
            box.setWindowTitle("option");
            box.setWindowIcon(icon);
            box.setIcon(QMessageBox::Information);
            box.setStyleSheet("color:blue");
            box.setText("分析完成");
            box.exec();
            ui->resultpage->show();
            boxt.exec();
        }else{
            QMessageBox box;
            QPixmap img(":/confused.png");
            QIcon icon = QIcon(img);
            box.setWindowTitle("option");
            box.setWindowIcon(icon);
            box.setIcon(QMessageBox::Information);
            box.setStyleSheet("color:blue");
            box.setText("分析失败");
            box.exec();
        }

        }

    }else{
        QMessageBox box;
        QPixmap img(":/confused.png");
        QIcon icon = QIcon(img);
        box.setWindowTitle("option");
        box.setWindowIcon(icon);
        box.setIcon(QMessageBox::Information);
        box.setStyleSheet("color:blue");
        box.setText("未加载模型");
        ui->loadbox->hide();
        ui->resultbox->hide();
        op.setloaddone(false);
        box.exec();
    }
}
