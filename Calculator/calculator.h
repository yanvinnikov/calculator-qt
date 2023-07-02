#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QJSEngine>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    enum solvefuncs_e
    {
        sin = 0,
        cos,
        tan,
        sqrt,
        lg,
        ln,
        invert,
        percent,
        factorial,
        floor
    };

    void AddText(QString, bool set_oper = false);
    void AddHistory(QString);
    void AddHistory(QString, QString color);
    void Clear();
    void Delete();

    bool Solve();
    void SolveFuncs(solvefuncs_e);

    void AngleMode();
    void AngleInverse();

    void Connect();
    void InitFuncs();

    void FilterExprBefore(QString&);
    void FilterExprAfter(QString&);
    void FilterHistory(QString&);

    void MoveScrollDown();
    void PinWindow();

private:
    bool Error() const;

protected:
    void keyPressEvent(QKeyEvent*) override;

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

    QJSEngine jsEngine;

    bool is_zero;
    bool is_operator;
    bool is_radians;
    bool is_inverse;
    bool is_file_loaded;
    bool is_pinned;
};

#endif // CALCULATOR_H
