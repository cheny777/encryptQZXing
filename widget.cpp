#include "widget.h"
#include "ui_widget.h"
#include <QImage>
#include <QDebug>
#include <QFileDialog>
#include <sstream>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);



}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,"open","./","tr(*.jpg *.png)");
    QImage imageToDecode(filename);
    QZXing decoder;
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    m_strmessage = decoder.decodeImage(imageToDecode);
    ui->textEdit->clear();
    ui->textEdit->append(m_strmessage);
}

void Widget::on_pushButton_2_clicked()
{
    string ss = encryption(m_strmessage.toStdString());
    QByteArray bytePwd = ss.c_str();
    QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
    QString strPwdMd5 = bytePwdMd5.toHex();
    ui->lineEdit->setText(strPwdMd5);
}
int BitChanged( int iOrg,int iMove )
{
    int iRes = iOrg;
    int iTemp = 0XFFFFFFFF;
    int iLow = 0;
    if (iMove>0)
    {
        iLow = iOrg<<iMove;
        iTemp = iTemp<<iMove;
        iTemp = iTemp^0xFFFFFFFF;
        iRes = iOrg>>32-iMove;
        iRes = iTemp &iRes;
        iRes = iLow | iRes;
    }
    else
    {
        iMove = -iMove;
        iLow = iOrg>>iMove;
        iTemp = iTemp<<32-iMove;
        iTemp = iTemp^0xFFFFFFFF;
        iLow = iLow & iTemp;
        iTemp = iOrg<<32-iMove;
        iRes = iLow|iTemp;
    }
    return iRes;
}
string Widget::encryption(string str)
{
    char *cCPUID = NULL;
    int iFch =0xFFFFFFFF;
    int iCPUID = 0;
    int iTemp = 0;
    int  iMove = 0;
    int iMch  = 0;
    cCPUID = const_cast<char *>(str.c_str());
    iCPUID = str.length();

    stringstream infile;

    srand((unsigned)time(NULL));
    /*保存文件长度*/
    iTemp = BitChanged(iCPUID,23);
    iTemp = iTemp^iFch;
    infile<<iTemp<<',';

    /*字符循环*/
    for (int i = 0;i<iCPUID;i++)
    {
        /*偏移位数*/
        iMove = i*3%32;
        /*改变字节*/
        iTemp = BitChanged(cCPUID[i],iMove);
        /*取反*/
        iTemp = iTemp^iFch;
        /*保存*/
        //fwrite(&iTemp,sizeof(int),1,fp);
        infile<<iTemp<<',';

    }
//    for (int i =0;i<64;i++)
//    {
//        /*随机数*/
//        iTemp = rand();
//        //fwrite(&iTemp,sizeof(int),1,fp);
//        infile<<iTemp<<',';
//    }
    return infile.str();
}

void Widget::on_pushButton_3_clicked()
{
    string ss = encryption(m_strmessage.toStdString());
    stringstream outfile(ss);
    char dian;
    int nNum;
    outfile>>nNum>>dian;
    int iFch =0xFFFFFFFF;

    nNum = nNum^iFch;
    nNum = BitChanged(nNum, -23);
    int  iMove = 0;
    int iMch  = 0;
    int iTemp = 0;

    string sc;
    for (int i =0;i<nNum;i++)
    {
        //infile.read((char*)&iTemp, sizeof(int));
        outfile>>iTemp>>dian;
        /*偏移位数*/
        iMove = i*3%32;
        /*取反*/
        iTemp = iTemp^iFch;
        /*改变字节*/
        iTemp = BitChanged(iTemp,-iMove);
        // strall[i] = iTemp;
        //cout<<strall[i]<<endl;
        sc.push_back(iTemp);
        /*CPUID = "006FF1BC9191922F000000000000000000000000"*/
    }

    ui->textEdit->clear();
    ui->textEdit->append(sc.c_str());

}
