
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->D->setValidator(new QIntValidator(2, 99999999));
    ui->P->setValidator(new QIntValidator(2, 99999999));
    ui->secretKeyAlice->setValidator(new QIntValidator(2, 99999999));
    ui->secretKeyBob->setValidator(new QIntValidator(2, 99999999));

    connect(ui->btnGenerate, SIGNAL(clicked(bool)), this, SLOT(generate()));
    connect(ui->btnClean, SIGNAL(clicked(bool)), this, SLOT(cleanResult()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate() {
    int D = ui->D->text().toInt();
    int P = ui->P->text().toInt();

    // Проверка на простые числа
    if (not(isPrime(D) and isPrime(P)))
        return;

    int secretKeyAlice = ui->secretKeyAlice->text().toInt();
    int secretKeyBob = ui->secretKeyBob->text().toInt();

    // Проверка закрытых ключей
    if (not(secretKeyAlice > 1 and secretKeyBob > 1))
        return;

    // Генерация открытых ключей
    int openKeyAlice = generateKey(D, P, secretKeyAlice);
    int openKeyBob = generateKey(D, P, secretKeyBob);

    // Генерация общих ключей
    int globalKeyAlice = generateKey(openKeyBob, P, secretKeyAlice);
    int globalKeyBob = generateKey(openKeyAlice, P, secretKeyBob);

    // Отображение
    ui->openKeyAlice->setText(QString::number(openKeyAlice));
    ui->openKeyBob->setText(QString::number(openKeyBob));
    ui->globalKeyAlice->setText(QString::number(globalKeyAlice));
    ui->globalKeyBob->setText(QString::number(globalKeyBob));
}

void MainWindow::cleanResult() {
    ui->D->clear();
    ui->P->clear();
    ui->secretKeyAlice->clear();
    ui->secretKeyBob->clear();
    ui->openKeyAlice->clear();
    ui->openKeyBob->clear();
    ui->globalKeyAlice->clear();
    ui->globalKeyBob->clear();
}

int MainWindow::generateKey(int D, int P, int key) {
    return ((int)pow(D, key)) % P;
}

bool MainWindow::isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
