#include "calculator.h"
#include "./ui_calculator.h"
#include <iomanip>
#include <sstream>

QString currentValue = "0";
bool addPressed = false;
bool subPressed = false;

Calculator::Calculator(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::Calculator){
    ui->setupUi(this);
    ui->Display->setText(currentValue);

    QPushButton *numberButtons[16];

    for (int i = 0; i<10; i++){
        QString buttonName = "Button" + QString::number(i);
        numberButtons[i] = Calculator::findChild<QPushButton *>(buttonName);
        connect(numberButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    QString letters[6] = {"A", "B", "C", "D", "E", "F"};
    for (int i = 0; i<6; i++){
        QString buttonName = "Button" + letters[i];
        numberButtons[i+10] = Calculator::findChild<QPushButton *>(buttonName);
        connect(numberButtons[i+10], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->OpAdd, SIGNAL(released()), this, SLOT(OperationPressed()));
    connect(ui->OpSub, SIGNAL(released()), this, SLOT(OperationPressed()));
    connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualsPressed()));
    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearPressed()));
}

Calculator::~Calculator()
{
    delete ui;
}

// exponential method because standard library does not contain.
int exponential(int number, int exp){
    int result = 1;
    for (int i = 0; i<exp; i++){
        result = result * number;
    }
    return result;
}

// converts hex number string to its decimal value as int.
int ConvertNumber(QString textValue){
    int value = 0;
    int length = textValue.length();

    for (int i = length-1; i>-1; i--){
        if (QString::compare(textValue[i], "0", Qt::CaseInsensitive) == 0) {
            value = value + 0;
        }
        else if (QString::compare(textValue[i], "1", Qt::CaseInsensitive) == 0) {
            value = value + 1*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "2", Qt::CaseInsensitive) == 0) {
            value = value + 2*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "3", Qt::CaseInsensitive) == 0) {
            value = value + 3*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "4", Qt::CaseInsensitive) == 0) {
            value = value + 4*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "5", Qt::CaseInsensitive) == 0) {
            value = value + 5*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "6", Qt::CaseInsensitive) == 0) {
            value = value + 6*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "7", Qt::CaseInsensitive) == 0) {
            value = value + 7*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "8", Qt::CaseInsensitive) == 0) {
            value = value + 8*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "9", Qt::CaseInsensitive) == 0) {
            value = value + 9*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "A", Qt::CaseInsensitive) == 0) {
            value = value + 10*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "B", Qt::CaseInsensitive) == 0) {
            value = value + 11*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "C", Qt::CaseInsensitive) == 0) {
            value = value + 12*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "D", Qt::CaseInsensitive) == 0) {
            value = value + 13*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "E", Qt::CaseInsensitive) == 0) {
            value = value + 14*exponential(16, -i+length-1);
        }
        else if (QString::compare(textValue[i], "F", Qt::CaseInsensitive) == 0) {
            value = value + 15*exponential(16, -i+length-1);
        }
    }
    if (QString::compare(textValue[0], "-", Qt::CaseInsensitive) == 0) {
                value = -value;
    }
    return value;
}

// performs adding operation.
QString OperationAdd(QString numberString1, QString numberString2){
    int value1 = ConvertNumber(numberString1);
    int value2 = ConvertNumber(numberString2);
    int resultInt = value1 + value2;
    QString result;

    if (resultInt>=0){
        std::stringstream stream;
        stream << std::hex << resultInt;
        std::string resultString(stream.str());
        result = QString::fromStdString(resultString);
    }
    else {
        resultInt = -resultInt;
        std::stringstream stream;
        stream << std::hex << resultInt;
        std::string resultString(stream.str());
        result = QString::fromStdString(resultString);
        result = "-"+result;
    }
    return result.toUpper();
}

// performs subtraction operation.
QString OperationSub (QString numberString1, QString numberString2){
    int value1 = ConvertNumber(numberString1);
    int value2 = ConvertNumber(numberString2);
    int resultInt = value1 - value2;
    QString result;

    if (resultInt>=0){
        std::stringstream stream;
        stream << std::hex << resultInt;
        std::string resultString(stream.str());
        result = QString::fromStdString(resultString);
    }
    else {
        resultInt = -resultInt;
        std::stringstream stream;
        stream << std::hex << resultInt;
        std::string resultString(stream.str());
        result = QString::fromStdString(resultString);
        result = "-"+result;
    }
    return result.toUpper();
}

// pressing number buttons triggers this method.
void Calculator::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString buttonValue = button->text();
    QString displayValue = ui->Display->text();

    if(displayValue == "0" || displayValue == "0.0" || displayValue == "" || QString::compare(displayValue[0], " ", Qt::CaseInsensitive)==0){
        ui->Display->setText(buttonValue);
    } else{
        QString newValue = displayValue+buttonValue;
        ui->Display->setText(newValue);
    }
}

// pressing add or subtract buttons triggers this method.
void Calculator::OperationPressed(){
    QString result = "0";

    if(addPressed || subPressed){
        if(addPressed){
            QString displayValue = ui->Display->text();
            if (QString::compare(displayValue, "", Qt::CaseInsensitive)==0){
                displayValue = "0";
                ui->Display->setText("");
            }
            else{
                if (QString::compare(displayValue[0], " ", Qt::CaseInsensitive) == 0) {
                    displayValue = "0";
                }
                result = OperationAdd(currentValue, displayValue);
                currentValue = result;
                ui->Display->setText(" "+currentValue);
            }

        }
        else if (subPressed){
            QString displayValue = ui->Display->text();
            if (QString::compare(displayValue, "", Qt::CaseInsensitive)==0){
                displayValue = "0";
                ui->Display->setText("");
            }
            else{
                if (QString::compare(displayValue[0], " ", Qt::CaseInsensitive) == 0) {
                    displayValue = "0";
                }
                result = OperationSub(currentValue, displayValue);
                currentValue = result;
                ui->Display->setText(" "+currentValue);
            }
        }
    }
    else {
        currentValue = ui->Display->text();
        ui->Display->setText("");
    }

    addPressed = false;
    subPressed = false;

    QPushButton *button = (QPushButton *)sender();
    QString operation = button->text();

    if(QString::compare(operation, "+", Qt::CaseInsensitive) == 0 ){
        addPressed = true;
    }
    else if(QString::compare(operation, "-", Qt::CaseInsensitive) == 0 ){
        subPressed = true;
    }
}

// pressing equals button triggers this method.
void Calculator::EqualsPressed(){
    QString result = "0";
    QString displayValue = ui->Display->text();

    if(addPressed || subPressed){
        if(addPressed){
            QString displayValue = ui->Display->text();
            if (QString::compare(displayValue, "", Qt::CaseInsensitive)==0){
                displayValue = "0";
            }
            if (QString::compare(displayValue[0], " ", Qt::CaseInsensitive) == 0) {
                displayValue = "0";
            }
            result = OperationAdd(currentValue, displayValue);
        }
        else {
            QString displayValue = ui->Display->text();
            if (QString::compare(displayValue, "", Qt::CaseInsensitive)==0){
                displayValue = "0";
            }
            if (QString::compare(displayValue[0], " ", Qt::CaseInsensitive) == 0) {
                displayValue = "0";
            }
            result = OperationSub(currentValue, displayValue);
        }
    } else {
        result = displayValue;
    }
    ui->Display->setText(result);
    addPressed = false;
    subPressed = false;
}

// pressing clr button triggers this method.
void Calculator::ClearPressed(){
    ui->Display->setText("0");
    currentValue = "0";
    addPressed = false;
    subPressed = false;
}
