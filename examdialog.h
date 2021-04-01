#ifndef EXAMDIALO_H
#define EXAMDIALO_H

#include <QDialog>
#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QGridLayout>
#include <QCheckBox>
#include <QTextEdit>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QButtonGroup>
#include <QPushButton>

class examdialog : public QDialog
{
    Q_OBJECT
public:
    examdialog(QWidget* parent = 0);
    void init();

private slots:
    void timeoutHandle();
    void showScore();

private:
    void initLayout();
    bool initTextEdit();
    void initButtons();
    bool CheckSelect();

private:
    QTimer* m_timer;
    int m_time;
    QGridLayout* m_layout;
    QRadioButton m_radiobtns[32];
    QLabel m_labels[10];
    QCheckBox m_checkBox[4];
    QRadioButton m_A;
    QRadioButton m_B;
    QTextEdit* m_textEdit;
    QStringList m_answer;
    QButtonGroup *m_btnGroup[9];
};

#endif // EXAMDIALO_H
