
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Валидаторы на на текстовые поля
    ui->p->setValidator(new QIntValidator(2, 99999999));
    ui->q->setValidator(new QIntValidator(2, 99999999));
    ui->openKeyN->setValidator(new QIntValidator(2, 99999999));
    ui->openKeyE->setValidator(new QIntValidator(2, 99999999));
    ui->secretKeyD->setValidator(new QIntValidator(2, 99999999));
    ui->openKeyN_2->setValidator(new QIntValidator(2, 99999999));
    ui->openKeyE_2->setValidator(new QIntValidator(2, 99999999));
    ui->secretKeyN->setValidator(new QIntValidator(2, 99999999));
    ui->secretKeyD_2->setValidator(new QIntValidator(2, 99999999));

    // Конекты на кнопки
    connect(ui->btnGenerate, SIGNAL(clicked(bool)), this, SLOT(generate()));
    connect(ui->btnClean, SIGNAL(clicked(bool)), this, SLOT(cleanResult()));
    connect(ui->btnEncrypt, SIGNAL(clicked(bool)), this, SLOT(encrypt()));
    connect(ui->btnUnencrypt, SIGNAL(clicked(bool)), this, SLOT(unencrypt()));
    connect(ui->btnCleanEncrypted, SIGNAL(clicked(bool)), this, SLOT(cleanEncrypted()));
    connect(ui->btnCleanUnencrypted, SIGNAL(clicked(bool)), this, SLOT(cleanUnencrypted()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Генерация ключей
void MainWindow::generate() {
    int p = ui->p->text().toInt();
    int q = ui->q->text().toInt();

    // Проверка на простые числа
    if (not(isPrime(p) and isPrime(q))){
        QMessageBox::information(nullptr, "Ошибка", "p и q непростые числа!");
        return;
    }

    int n = p * q;
    if (n < ALPHABET.length()) {
        QMessageBox::information(nullptr, "Ошибка", "n меньше мощности алфавита!");
        return;
    }

    ui->openKeyN->setText(QString::number(n));
    ui->openKeyN_2->setText(QString::number(n));
    ui->secretKeyN->setText(QString::number(n));

    if (p == 2 and q == 2)
        return;
    int euler = (q-1)*(p-1);

    ui->euler->setText(QString::number(euler));

    int e = 2;
    while (euler % e == 0 or not(isPrime(e))) {
        e++;
    }

    if (n < e)
        return;

    ui->openKeyE->setText(QString::number(e));
    ui->openKeyE_2->setText(QString::number(e));

    int d = modInverse(e, euler);
    if (d == -1) {
        QMessageBox::information(nullptr, "Ошибка", "Обратного числа не существует!");
        return;
    }

    ui->secretKeyD->setText(QString::number(d));
    ui->secretKeyD_2->setText(QString::number(d));
}


// Очисить генерацию
void MainWindow::cleanResult() {
    ui->p->clear();
    ui->q->clear();
    ui->openKeyN->clear();
    ui->openKeyE->clear();
    ui->secretKeyD->clear();
    ui->euler->clear();
}


// Зашифрофать
void MainWindow::encrypt() {
    int n = ui->openKeyN_2->text().toInt(0);
    int e = ui->openKeyE_2->text().toInt(0);

    QString text = ui->textToEncrypt->text();

    if (not(n or e))
        return;

    QList<int> textCode;

    for (QChar letter : text) {
        if (ALPHABET.contains(letter)) {
            int T = ALPHABET.indexOf(letter);
            textCode.append(generateKey(T, e, n));
        }
        else {
            QMessageBox::information(nullptr, "Ошибка", "Текст содержит неподерживаемый символ");
            return;
        }
    }

    QString letters;
    for (QChar letter : text) {
        letters += QString::number(ALPHABET.indexOf(letter) + 1) + " ";
    }

    QString encryptedText;
    for (int code : textCode) {
        encryptedText += QString::number(code) + " ";
    }

    ui->encryptedTextCode->setText(letters);
    ui->encryptedText->setText(encryptedText);
}


// Расшифровать
void MainWindow::unencrypt() {
    int n = ui->secretKeyN->text().toInt(0);
    int d = ui->secretKeyD_2->text().toInt(0);

    QString text = ui->textToUnencrypt->toPlainText();

    if (not(n or d))
        return;

    QStringList textCode = text.split(" ");
    textCode.removeLast();
    QStringList letters;

    for (QString C : textCode) {
        letters.append(QString::number(generateKey(C.toInt(), d, n) + 1));
    }

    QString lettersText;
    for (QString letter : letters) {
        lettersText += letter + " ";
    }

    QString unencryptedText;
    for (QString letter : letters) {
        unencryptedText += ALPHABET[letter.toInt() - 1];
    }

    ui->unencryptedTextCode->setText(lettersText);
    ui->unencryptedText->setText(unencryptedText);
}


// Очистить зашифрованное
void MainWindow::cleanEncrypted() {
    ui->textToEncrypt->clear();
    ui->encryptedText->clear();
    ui->encryptedTextCode->clear();
}


// Очистить расшифрованное
void MainWindow::cleanUnencrypted() {
    ui->textToUnencrypt->clear();
    ui->unencryptedText->clear();
    ui->unencryptedTextCode->clear();
}


// Генерировать ключ
int MainWindow::generateKey(unsigned long long D, unsigned long long P, unsigned long long key) {
    unsigned long result = 1;
    D %= key;
    while(P > 0) {
        if (P % 2 == 1) {
            result = (result * D) % key;
        }
        P /= 2;
        D = (D*D) % key;
    }
    return result;
}

// Проверка на простое число
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

// Расширенный алгоритм Евклида
int MainWindow::extendedEuclidean(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    int x1, y1;
    int gcd = extendedEuclidean(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

// Функция для нахождения обратного числа по модулю n
int MainWindow::modInverse(int a, int n) {
    int x, y;
    int gcd = extendedEuclidean(a, n, x, y);

    if (gcd != 1) {
        return -1; // обратного элемента не существует
    } else {
        // Обрабатываем отрицательное значение x
        return (x % n + n) % n;
    }
}
