
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <math.h>
#include <QMessageBox>
//#include <boost/multiorecision/cpp_int.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generate();                                                // Генерация ключей
    void cleanResult();                                             // Очисить генерацию

    void encrypt();                                                 // Зашифрофать
    void unencrypt();                                               // Расшифровать

    void cleanEncrypted();                                          // Очистить зашифрованное
    void cleanUnencrypted();                                        // Очистить расшифрованное

    int generateKey(unsigned long long, unsigned long long, unsigned long long);   // Генерировать ключ

    bool isPrime(int);                                              // Проверка на простое число

    int extendedEuclidean(int a, int b, int&, int&);                // Расширенный алгоритм Евклида
    int modInverse(int a, int n);                                   // Функция для нахождения обратного числа по модулю n

private:
    Ui::MainWindow *ui;

    const QString ALPHABET = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
                             "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789"
                             "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ ";
};

#endif // MAINWINDOW_H
