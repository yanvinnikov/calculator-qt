#include "calculator.h"
#include "ui_calculator.h"

#include <QRegularExpression>
#include <QJSEngine>
#include <QScrollBar>
#include <QFile>
#include <QClipboard>

namespace
{
const QString ch_minus = "−";
const QString ch_plus = "+";
const QString ch_multiply = "×";
const QString ch_divide = "÷";
const QString ch_pi = "π";
const QString ch_dot = ",";
const QString ch_e = "e";
const QString ch_power = "^";
const QString ch_leftparen = "(";
const QString ch_rightparen = ")";
const QString ch_infinity = "∞";
const QString ch_degree = "°";
const QString ch_inverse = "⁻¹";
const QString ch_sqrt = "√";

const QString color_orange = "rgb(255, 85, 0)";
}

void Calculator::Connect()
{
    connect(ui->b_0, &QPushButton::pressed, this, [this]{ AddText("0"); });
    connect(ui->b_1, &QPushButton::pressed, this, [this]{ AddText("1"); });
    connect(ui->b_2, &QPushButton::pressed, this, [this]{ AddText("2"); });
    connect(ui->b_3, &QPushButton::pressed, this, [this]{ AddText("3"); });
    connect(ui->b_4, &QPushButton::pressed, this, [this]{ AddText("4"); });
    connect(ui->b_5, &QPushButton::pressed, this, [this]{ AddText("5"); });
    connect(ui->b_6, &QPushButton::pressed, this, [this]{ AddText("6"); });
    connect(ui->b_7, &QPushButton::pressed, this, [this]{ AddText("7"); });
    connect(ui->b_8, &QPushButton::pressed, this, [this]{ AddText("8"); });
    connect(ui->b_9, &QPushButton::pressed, this, [this]{ AddText("9"); });

    connect(ui->b_dot, &QPushButton::pressed, this, [this]{ AddText(ch_dot, true); });
    connect(ui->b_multiply, &QPushButton::pressed, this, [this]{ AddText(ch_multiply, true); });
    connect(ui->b_divide, &QPushButton::pressed, this, [this]{ AddText(ch_divide, true); });
    connect(ui->b_plus, &QPushButton::pressed, this, [this]{ AddText(ch_plus, true); });
    connect(ui->b_minus, &QPushButton::pressed, this, [this]{ AddText(ch_minus, true); });
    connect(ui->b_power, &QPushButton::pressed, this, [this]{ AddText(ch_power, true); });

    connect(ui->b_br_open, &QPushButton::pressed, this, [this]{ AddText(ch_leftparen); });
    connect(ui->b_br_close, &QPushButton::pressed, this, [this]{ AddText(ch_rightparen); });
    connect(ui->b_pi, &QPushButton::pressed, this, [this]{ AddText(ch_pi); });
    connect(ui->b_e, &QPushButton::pressed, this, [this]{ AddText(ch_e); });

    connect(ui->b_clear, &QPushButton::pressed, this, &Calculator::Clear);
    connect(ui->b_delete, &QPushButton::pressed, this, &Calculator::Delete);
    connect(ui->b_enter, &QPushButton::pressed, this, &Calculator::Solve);

    connect(ui->b_percent, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::percent); });
    connect(ui->b_sin, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::sin); });
    connect(ui->b_cos, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::cos); });
    connect(ui->b_tan, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::tan); });
    connect(ui->b_sqrt, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::sqrt); });
    connect(ui->b_lg, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::lg); });
    connect(ui->b_ln, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::ln); });
    connect(ui->b_1divx, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::invert); });
    connect(ui->b_factorial, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::factorial); });
    connect(ui->b_floor, &QPushButton::pressed, this, [this]{ SolveFuncs(solvefuncs_e::floor); });

    connect(ui->b_anglemode, &QPushButton::pressed, this, &Calculator::AngleMode);
    connect(ui->b_inverse, &QPushButton::pressed, this, &Calculator::AngleInverse);

    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, &Calculator::MoveScrollDown);
    connect(ui->b_pin, &QPushButton::pressed, this, &Calculator::PinWindow);
    connect(ui->b_reload, &QPushButton::pressed, this, &Calculator::InitFuncs);
    connect(ui->opacity_slider, &QSlider::valueChanged, this, [this]{
        setWindowOpacity(ui->opacity_slider->value()/100.0);
    });
}

void Calculator::keyPressEvent(QKeyEvent* ev)
{
    QClipboard* clip (QApplication::clipboard());

    switch (ev->modifiers())
    {
        case Qt::ControlModifier:
        {
            switch (ev->key())
            {
            case Qt::Key_V:
                AddText(clip->text());
                return;
            case Qt::Key_C:
                clip->setText(ui->label->text());
                return;
            }
        } break;

        case Qt::ShiftModifier:
        {
            switch (ev->key())
            {
            case Qt::Key_Z:
                PinWindow();
                return;
            }
        } break;
    }

    switch(ev->key())
    {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
            AddText(ev->text());
            break;

    case Qt::Key_Minus: AddText(ch_minus, true); break;
    case Qt::Key_Plus: AddText(ch_plus, true); break;
    case Qt::Key_Slash: AddText(ch_divide, true); break;
    case Qt::Key_Asterisk: AddText(ch_multiply, true); break;
    case Qt::Key_AsciiCircum: AddText(ch_power, true); break;
    case Qt::Key_ParenLeft: AddText(ch_leftparen); break;
    case Qt::Key_ParenRight: AddText(ch_rightparen); break;
    case Qt::Key_P: AddText(ch_pi); break;
    case Qt::Key_E: AddText(ch_e); break;

    case Qt::Key_Period:
    case Qt::Key_Comma:
        AddText(ch_dot, true);
        break;

    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        Delete();
        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        Solve();
        break;

    case Qt::Key_C:
    case Qt::Key_Escape:
        Clear();
        break;

    case Qt::Key_Exclam: SolveFuncs(solvefuncs_e::factorial); break;
    case Qt::Key_R: SolveFuncs(solvefuncs_e::sqrt); break;
    case Qt::Key_V: SolveFuncs(solvefuncs_e::invert); break;
    case Qt::Key_A: SolveFuncs(solvefuncs_e::sin); break;
    case Qt::Key_S: SolveFuncs(solvefuncs_e::cos); break;
    case Qt::Key_D: SolveFuncs(solvefuncs_e::tan); break;
    case Qt::Key_Z: SolveFuncs(solvefuncs_e::lg); break;
    case Qt::Key_X: SolveFuncs(solvefuncs_e::ln); break;
    case Qt::Key_F: SolveFuncs(solvefuncs_e::floor); break;
    case Qt::Key_Percent: SolveFuncs(solvefuncs_e::percent); break;

    case Qt::Key_Q: AngleInverse(); break;
    case Qt::Key_W: AngleMode(); break;
    case Qt::Key_I: InitFuncs(); break;
    }
}

void Calculator::InitFuncs()
{
//    QApplication::applicationDirPath()
    QFile file(":js/rs/funcs.js");

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        AddHistory("Couldn't open funcs.js:<br>" + file.errorString(), "red");
        return;
    }

    QTextStream stream(&file);
    QString content;

    while (!stream.atEnd())
        content += stream.readLine() + "\n";

    if (jsEngine.evaluate(content).isError())
    {
        AddHistory("Syntax error in funcs.js", "red");
        return;
    }

    is_file_loaded = true;
    AddHistory("Loaded funcs.js");
}

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
    , is_zero(true)
    , is_operator(false)
    , is_radians(false)
    , is_inverse(false)
    , is_file_loaded(false)
    , is_pinned(false)
{
    ui->setupUi(this);

    Connect();
    InitFuncs();
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::AddHistory(QString _append)
{
    QString old_history = ui->history->text();

    if (old_history.size() > 0)
        old_history += "<br><br>";

    ui->history->setText(old_history + _append);
}

void Calculator::AddHistory(QString _append, QString color)
{
    AddHistory(
        "<span style='color:" + color + "'>" +
        _append +
        "</span>");
}

void Calculator::AddText(QString _append, bool set_oper)
{
    QString text = ui->label->text();

    if (is_zero && Error())
        Clear();

    if (is_zero && _append == "0")
        return;

    if (is_zero && !set_oper)
        ui->label->clear();

    if (is_operator && set_oper)
        Delete();

    is_operator = set_oper;
    is_zero = false;

    text = ui->label->text() + _append;
    ui->label->setText(text);

    if (_append == ch_dot &&
        QRegularExpression(R"(\d+)" + ch_dot + R"(\d+)" + ch_dot)
            .match(text).hasMatch())
    {
        Delete();
    }

    ui->b_clear->setText("C");
    ui->label->setStyleSheet(0);
}

void Calculator::Clear()
{
    if (is_zero && ui->label->text() == "0")
        ui->history->clear();

    is_operator = false;
    is_zero = true;

    ui->label->setText("0");
    ui->label->setStyleSheet(0);

    ui->b_clear->setText("AC");
}

void Calculator::Delete()
{
    QString text = ui->label->text();

    if (is_zero && Error())
        Clear();

    if (is_zero)
        return;

    text = ui->label->text();
    qsizetype size = text.size();

    if (size == 1 ||
        (size == 2 && text[0] == '0'))
    {
        Clear();
        return;
    }

    ui->label->setText(text.removeLast());

    QString last = ui->label->text().last(1);
    is_operator = !(last >= '0' && last <= '9');
}

void Calculator::FilterExprBefore(QString& text)
{
    text.replace(ch_power, "**");
    text.replace(ch_minus, "-");
    text.replace(ch_divide, "/");
    text.replace(ch_multiply, "*");
    text.replace(ch_infinity, "Infinity");
    text.replace(ch_dot, ".");

    text.replace(ch_pi, "Math.PI");

    text.replace(QRegularExpression(R"((\d+\.?\d*)e((\+|\-)?\d+))"), R"(\1E\2)");
    text.replace(ch_e, "Math.E");
}

void Calculator::FilterExprAfter(QString& text)
{
    text.replace(".", ch_dot);
    text.replace("Infinity", ch_infinity);
}

void Calculator::FilterHistory(QString& history)
{
    history.replace("*(Math.PI/180)", ch_degree);
    history.replace("*(180/Math.PI)", "rad");

    history.replace("Infinity", ch_infinity);
    history.replace(QRegularExpression(R"(Math\.(\w+\(.+\)))"), R"(\1)");
    history.replace(QRegularExpression(R"(factorial\((\-?(\d+|\w+|)" + ch_infinity + ")" + ch_dot + R"(?\d*(e\+\d+)?)\))"), R"(\1!)");
    history.replace(QRegularExpression(R"(sqrt\((\-?(\d+|\w+|)" + ch_infinity + ")" + ch_dot + R"(?\d*(e\+\d+)?)\))"), ch_sqrt + R"(\1)");

    history.replace("-", ch_minus);
    history.replace("/", ch_divide);
    history.replace("*", ch_multiply);

    history.replace("Math.PI", ch_pi);
    history.replace("Math.E", ch_e);

    history.replace(QRegularExpression("<" + ch_divide + R"((\w+)>)"), R"(</\1>)");
}

bool Calculator::Solve()
{
    if (is_operator)
        return false;

    QString text = ui->label->text();

    if (is_zero && Error())
    {
        text = "0";
        Clear();
    }

    QString old_text = text;
    QString history = text;

    FilterExprBefore(text);

    QJSValue eval;

    if (is_file_loaded)
        eval = jsEngine.evaluate("calc_main(" + text + ")");
    else
        eval = jsEngine.evaluate(text);

    text = eval.toString();

    if (text == old_text)
        return true;

    if (eval.isError())
    {
        Clear();
        ui->label->setText("Error");
        ui->label->setStyleSheet("color: red");
        return false;
    }

    is_zero = true;

    FilterExprAfter(text);
    ui->label->setText(text);

    bool add_history = (history != text);

    if (add_history)
    {
        history += "<br>= "
                   "<span style='color:" + color_orange + "'>" +
                   text +
                   "</span>";

        FilterHistory(history);
        AddHistory(history);
    }

    return true;
}

void Calculator::SolveFuncs(solvefuncs_e func)
{
    QString text = ui->label->text();

    if (is_zero && Error())
        Clear();

    if (is_operator)
        return;

    if (!Solve())
        return;

    text = ui->label->text();

    bool anglefunc = (
        solvefuncs_e::sin == func ||
        solvefuncs_e::cos == func ||
        solvefuncs_e::tan == func);

    if (!is_radians && anglefunc && !is_inverse)
        text = text + "*(Math.PI/180)";

    QString to_deg = (is_radians ? ")" : ")*(180/Math.PI)");

    switch (func)
    {
    case solvefuncs_e::sin:
        if (is_inverse)
            text = "Math.asin(" + text + to_deg;
        else
            text = "Math.sin(" + text + ")";
        break;
    case solvefuncs_e::cos:
        if (is_inverse)
            text = "Math.acos(" + text + to_deg;
        else
            text = "Math.cos(" + text + ")";
        break;
    case solvefuncs_e::tan:
        if (is_inverse)
            text = "Math.atan(" + text + to_deg;
        else
            text = "Math.tan(" + text + ")";
        break;

    case solvefuncs_e::sqrt:
        text = "Math.sqrt(" + text + ")";
        break;
    case solvefuncs_e::lg:
        text = "lg(" + text + ")";
    break;
    case solvefuncs_e::ln:
    text = "ln(" + text + ")";
        break;
    case solvefuncs_e::invert:
        text = "1/" + text;
        break;
    case solvefuncs_e::percent:
        text = text + "/100";
        break;
    case solvefuncs_e::factorial:
        text = "factorial(" + text + ")";
        break;
    case solvefuncs_e::floor:
        text = "Math.floor(" + text + ")";
        break;
    }

    ui->label->setText(text);
    Solve();
}

void Calculator::AngleMode()
{
    is_radians = !is_radians;
    ui->b_anglemode->setText(is_radians ? "rad" : "deg");
}

void Calculator::AngleInverse()
{
    is_inverse = !is_inverse;

    if (is_inverse)
    {
        ui->b_inverse->setStyleSheet("color:" + color_orange);

        ui->b_sin->setText("sin" + ch_inverse);
        ui->b_cos->setText("cos" + ch_inverse);
        ui->b_tan->setText("tan" + ch_inverse);
    }
    else
    {
        ui->b_inverse->setStyleSheet(0);

        ui->b_sin->setText("sin");
        ui->b_cos->setText("cos");
        ui->b_tan->setText("tan");
    }
}

void Calculator::MoveScrollDown()
{
    QScrollBar* sbar = ui->scrollArea->verticalScrollBar();
    sbar->setSliderPosition(sbar->maximum());
}

void Calculator::PinWindow()
{
    is_pinned = !is_pinned;

    ui->b_pin->setStyleSheet(is_pinned ? ("color:" + color_orange) : 0);

    setWindowFlag(Qt::WindowStaysOnTopHint, is_pinned);
    show();
}

bool Calculator::Error() const
{
    return ui->label->text() == "Error";
}
