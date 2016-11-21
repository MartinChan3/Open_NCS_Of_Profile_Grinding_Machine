#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QPalette>
#include <QAxObject>
#include <QLabel>

#include "trioactivex.h"

namespace Ui {
class MainWindow;
}

enum BUTTON_GROUP_TYPE{MAIN,SUB,SUB2,ALL_SUB,ALL};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *mainButton[6],*helpButton,*subButton[5],*sub2Button[8];
    QStackedWidget *mainStack,*editStack;
    QButtonGroup *ButtonGroup_main,*ButtonGroup_sub,*ButtonGroup_sub2;
    QPalette Palette_Unpressed,Palette_Pressed,Palette_Unconnected,Palette_Connected;
    QLabel *Label_Connection_Status;
    int current_ButtonID_main,current_ButtonID_sub,current_ButtonID_sub2;

    void button_pressed(QPushButton *button);
    void button_unpressed(QPushButton *button);
    void clear_button_text(BUTTON_GROUP_TYPE);

    TrioPCLib::TrioPC *trio;

private slots:
    void pressed_mainButtonGroup(int i);
    void pressed_subButtonGroup(int i);
    void pressed_sub2ButtonGroup(int i);

};

#endif // MAINWINDOW_H
