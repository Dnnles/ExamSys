#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>

loginDialog::loginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    setFixedSize(ui->label->size());
    setWindowTitle("科目一考试系统");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_pushButton_login_clicked()
{
    QRegExp re("^[A-Za-z0-9]+(_[a-zA-Z0-9]+)*@[a-zA-Z0-9\-]+\.{1,1}[a-zA-Z]{2,6}$");
    if(re.exactMatch(ui->lineEdit_account->text())){

        QFile file("../account.txt");
        QTextStream stream(&file);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::information(this,"提示","账号文件打开失败");
            return;
        }

        QString account = ui->lineEdit_account->text();
        QString code = ui->lineEdit_code->text();

        QString strlen,rAccount,rCode;
        QStringList strlist;

        while(!stream.atEnd()){

            strlen = stream.readLine();
            strlist = strlen.split(",");
            rAccount = strlist.at(0);
            rCode = strlist.at(1);

            if(account == rAccount){
                if(code == rCode){
                    QMessageBox::information(this,"提示","登录成功");
                    file.close();
                    return;
                }else{
                    QMessageBox::information(this,"提示","密码错误");
                    ui->lineEdit_code->clear();
                    ui->lineEdit_code->setFocus();
                    file.close();
                    return;
                }
            }
        }

        QMessageBox::information(this,"提示","邮箱不存在");
        file.close();
        ui->lineEdit_account->clear();
        ui->lineEdit_account->setFocus();
        ui->lineEdit_code->clear();
        return;
    }else{
        QMessageBox::information(this,"提示","非法的邮箱地址");
        ui->lineEdit_account->clear();
        ui->lineEdit_account->setFocus();
    }
}
