#include "examdialog.h"

examdialog::examdialog(QWidget* parent):QDialog(parent)
{
    setWindowTitle("当前考试已用时 0 分 0 秒");
    init();

    if(!initTextEdit()){
        QMessageBox::critical(this,"提示","题库初始化失败");
        QTimer::singleShot(0,qApp,SLOT(qiut()));
    }

    initLayout();
    resize(800,900);
    initButtons();

}

void examdialog::init(){
    m_time = 0;
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();

    connect(m_timer,&QTimer::timeout,this,&examdialog::timeoutHandle);
}

void examdialog::timeoutHandle()
{
    m_time++;
    int min = m_time / 60;
    int sec = m_time % 60;
    QString str = QString("当前考试已用时 %1 分 %2 秒").arg(min).arg(sec);
    setWindowTitle(str);
}

void examdialog::showScore()
{
    if(CheckSelect()){
        QMessageBox::information(this,"提示","您有为完成的题目，请继续答题","继续答题");
        return;
    }

    int score = 0;

    for(int i = 0; i < 10; i++){

        qDebug() << m_answer.at(i);

        if(i < 8){
            if(m_btnGroup[i]->checkedButton()->text() == m_answer.at(i)) score += 10;
            qDebug() << QString("%1 单选正确").arg(i+1);
        }

        if(i == 8){
            QString tmp = m_answer.at(8);
            bool hasA = tmp.contains("A") ? true : false;
            bool hasB = tmp.contains("B") ? true : false;
            bool hasC = tmp.contains("C") ? true : false;
            bool hasD = tmp.contains("D") ? true : false;

            bool checkA = m_checkBox[0].checkState() ? true : false;
            bool checkB = m_checkBox[1].checkState() ? true : false;
            bool checkC = m_checkBox[2].checkState() ? true : false;
            bool checkD = m_checkBox[3].checkState() ? true : false;

            if(hasA != checkA) continue;
            if(hasB != checkB) continue;
            if(hasC != checkC) continue;
            if(hasD != checkD) continue;

            score += 10;
        }

        if(i == 9){
            if(m_btnGroup[8]->checkedButton()->text() == m_answer.at(i)) {
                //qDebug() << "判断题";
                score += 10;
            }
        }
    }

    int ret = QMessageBox::information(this,"提交",QString("您当前成绩为%1分,是否重新考试").arg(score),QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes) return;
    else close();
}

void examdialog::initLayout()
{
    m_layout = new QGridLayout(this);
    m_layout->setMargin(10);
    m_layout->setSpacing(10);
    m_layout->addWidget(m_textEdit,0,0,1,10);
}

bool examdialog::initTextEdit()
{
    m_textEdit = new QTextEdit(this);
    m_textEdit->setReadOnly(true);
    QFile file("../exam.txt");
    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,"提示","题库文件打开失败");
        return false;
    }

    QString strText;
    QStringList strList;
    int nLines = 0;
    QString tmpline;

    while(!stream.atEnd()){

        //qDebug() << nLines;

        if(nLines == 0){ //过滤首行
            tmpline = stream.readLine();
            nLines++;
            continue;
        }

        if((nLines >= 6 && nLines <= 6 * 9 && nLines % 6 == 0) || (nLines == 6 * 9 + 4)){
            tmpline = stream.readLine();
            strList = tmpline.split(" ");
            //qDebug() << strList.at(1);
            m_answer.append(strList.at(1));
            strText += "\n";
            nLines++;
            continue;
        }


        strText += stream.readLine();
        strText += "\n";
        nLines++;
    }
    file.close();
    //qDebug() << strText;
    m_textEdit->setText(strText);

    return true;
}

void examdialog::initButtons()
{
    QStringList option = {"A","B","C","D"};

    for(int i = 0; i < 10; i++){
        QString label = QString("第%1题").arg(i+1);
        m_labels[i].setText(label);
        m_layout->addWidget(&m_labels[i],1,i,1,1);

        if(i == 8){
            for(int k = 0; k < 4; k++){
                m_checkBox[k].setText(option.at(k));
                m_layout->addWidget(&m_checkBox[k],k + 2,8,1,1);
            }

            continue;
        }

        if(i == 9){
            m_btnGroup[8] = new QButtonGroup(this);
            m_A.setText("正确");
            m_B.setText("错误");
            m_layout->addWidget(&m_A,2,9,1,1);
            m_layout->addWidget(&m_B,3,9,1,1);

            m_btnGroup[8]->addButton(&m_A);
            m_btnGroup[8]->addButton(&m_B);

            break;
        }

        if(i <= 7) m_btnGroup[i] = new QButtonGroup(this);
        for(int j = 0; j < 4; j++){
            m_radiobtns[4 * i + j].setText(option.at(j));
            m_layout->addWidget(&m_radiobtns[4 * i + j],2+j,i,1,1);
            m_btnGroup[i]->addButton(&m_radiobtns[4 * i + j]);
        }
    }

    QPushButton* commitBtn = new QPushButton("提交");
    commitBtn->setFixedSize(100,35);
    m_layout->addWidget(commitBtn,6,9,1,1);
    connect(commitBtn,SIGNAL(clicked()),this,SLOT(showScore()));
}

bool examdialog::CheckSelect()
{
    int radioCount = 0;
    int checkBoxCount = 0;

    for(int i = 0; i < 10; i++){
        if(i < 8){
            if(m_btnGroup[i]->checkedButton()) radioCount++;
        }

        if(i == 8){
            for(int j = 0; j < 4; j++){
                if(m_checkBox[j].isChecked()) checkBoxCount++;
            }
        }

        if(i == 9){
            if(!m_A.isChecked() && !m_B.isChecked()) return true;
        }
    }

    if(radioCount < 8) return true;
    if(checkBoxCount < 2) return true;
    return false;
}



