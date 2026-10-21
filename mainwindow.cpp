#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "matrix.h"
#include <QTableWidget>
#include <QLabel>

// ── Helpers ───────────────────────────────────────────────────

static QString fmt(double x) {
    if (std::fabs(x) < 1e-10) x = 0;
    return QString::number(x, 'g', 6);
}

static Matrix readTable(QTableWidget* t, bool* ok = nullptr) {
    Matrix m(t->rowCount(), std::vector<double>(t->columnCount(), 0));
    bool allOk = true;
    for (int i = 0; i < t->rowCount(); i++)
        for (int j = 0; j < t->columnCount(); j++) {
            auto it = t->item(i, j);
            if (!it || it->text().trimmed().isEmpty()) continue;
            bool good;
            m[i][j] = it->text().replace(',', '.').toDouble(&good);
            if (!good) allOk = false;
        }
    if (ok) *ok = allOk;
    return m;
}

static void setTable(QTableWidget* t, const Matrix& r) {
    if (isEmpty(r)) return;
    t->setRowCount(r.size());
    t->setColumnCount(r[0].size());
    for (size_t i = 0; i < r.size(); i++)
        for (size_t j = 0; j < r[0].size(); j++)
            t->setItem(i, j, new QTableWidgetItem(fmt(r[i][j])));
}

static void showResult(QTableWidget* out, QLabel* err, const Matrix& r, const QString& msg) {
    err->setStyleSheet("");
    if (isEmpty(r)) { err->setText(msg); return; }
    err->setText("");
    setTable(out, r);
}

static void showDet(QTableWidget* t, QLabel* err, const QString& name) {
    double det;
    err->setStyleSheet("");
    if (!determinant(readTable(t), det)) { err->setText("Error: matrix must be square!"); return; }
    err->setText("det(" + name + ") = " + fmt(det));
}

// ── Construtor ────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #1a1a2e;
            color: #e2e8f0;
            font-family: "Segoe UI";
        }
        QLabel {
            color: #e2e8f0;
            font-size: 13px;
        }
        QLabel#lbErrorText {
            color: #e86c6c;
            font-size: 12px;
            font-weight: bold;
        }
        QTableWidget#tableA, QTableWidget#tableB, QTableWidget#tableAnswer {
            background-color: #16213e;
            border: 1px solid #2d3561;
            border-radius: 8px;
            gridline-color: #2d3561;
            color: #e2e8f0;
            font-size: 13px;
        }
        QTableWidget#tableA::item, QTableWidget#tableB::item, QTableWidget#tableAnswer::item {
            color: #e2e8f0;
            padding: 4px;
        }
        QTableWidget#tableA::item:selected, QTableWidget#tableB::item:selected,
        QTableWidget#tableAnswer::item:selected {
            background-color: #7c6af0;
            color: white;
        }
        QTableWidget#tableResult {
            background-color: #0d2137;
            border: 1px solid #4caf8740;
            border-radius: 8px;
            gridline-color: #1a3a52;
            color: #4caf87;
            font-family: "Courier New";
            font-size: 13px;
            font-weight: bold;
        }
        QTableWidget#tableResult::item {
            color: #4caf87;
            padding: 4px;
        }
        QHeaderView::section {
            background-color: #0f3460;
            color: #7c6af0;
            border: 1px solid #2d3561;
            font-size: 11px;
            font-weight: bold;
            padding: 4px;
        }
        QTableCornerButton::section {
            background-color: #0f3460;
            border: 1px solid #2d3561;
        }
        QSpinBox {
            background-color: #0f3460;
            border: 1px solid #7c6af0;
            border-radius: 6px;
            color: #7c6af0;
            font-size: 14px;
            font-weight: bold;
            padding: 4px 8px;
            min-width: 50px;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            background-color: #2d3561;
            border-radius: 3px;
            width: 16px;
        }
        QPushButton#btnSum {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #7c6af0, stop:1 #5a4fcf);
            border: none; border-radius: 8px; color: white;
            font-size: 13px; font-weight: bold; padding: 10px 20px;
        }
        QPushButton#btnSum:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #9a8af5, stop:1 #7c6af0);
        }
        QPushButton#btnSubtract {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #e86c6c, stop:1 #c94040);
            border: none; border-radius: 8px; color: white;
            font-size: 13px; font-weight: bold; padding: 10px 20px;
        }
        QPushButton#btnSubtract:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #f08080, stop:1 #e05050);
        }
        QPushButton#btnMultiply {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #4caf87, stop:1 #2e8b6a);
            border: none; border-radius: 8px; color: white;
            font-size: 13px; font-weight: bold; padding: 10px 20px;
        }
        QPushButton#btnMultiply:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #5dc99e, stop:1 #3da07a);
        }
        QPushButton#btnSum, QPushButton#btnSubtract, QPushButton#btnMultiply {
            min-width: 80px;
        }
        QPushButton#btTransposeA, QPushButton#btTransposeB,
        QPushButton#btMultiplyA,  QPushButton#btMultiplyB,
        QPushButton#btDeterminantA, QPushButton#btDeterminantB,
        QPushButton#btInverseA, QPushButton#btInverseB,
        QPushButton#btNewExercise, QPushButton#btSolution {
            background-color: #16213e;
            border: 1px solid #2d3561;
            border-radius: 6px;
            color: #a8b4d8;
            font-size: 12px;
            padding: 6px 14px;
            min-width: 90px;
        }
        QPushButton#btTransposeA:hover, QPushButton#btTransposeB:hover,
        QPushButton#btMultiplyA:hover,  QPushButton#btMultiplyB:hover,
        QPushButton#btDeterminantA:hover, QPushButton#btDeterminantB:hover,
        QPushButton#btInverseA:hover, QPushButton#btInverseB:hover,
        QPushButton#btNewExercise:hover, QPushButton#btSolution:hover {
            background-color: #2d3561;
            color: #e2e8f0;
        }
        QPushButton#btTransposeA:pressed, QPushButton#btTransposeB:pressed,
        QPushButton#btMultiplyA:pressed,  QPushButton#btMultiplyB:pressed,
        QPushButton#btDeterminantA:pressed, QPushButton#btDeterminantB:pressed,
        QPushButton#btInverseA:pressed, QPushButton#btInverseB:pressed,
        QPushButton#btNewExercise:pressed, QPushButton#btSolution:pressed {
            background-color: #0f3460;
        }
        QPushButton#btCheck {
            background-color: #4caf87;
            border: none;
            border-radius: 6px;
            color: white;
            font-size: 12px;
            font-weight: bold;
            padding: 6px 14px;
            min-width: 90px;
        }
        QPushButton#btCheck:hover { background-color: #5dc99e; }
        QLineEdit#leAnswer {
            background-color: #0f3460;
            border: 1px solid #7c6af0;
            border-radius: 6px;
            color: #e2e8f0;
            padding: 6px;
        }
        QComboBox#cbLevel {
            background-color: #0f3460;
            border: 1px solid #7c6af0;
            border-radius: 6px;
            color: #e2e8f0;
            padding: 4px 8px;
        }
        QComboBox QAbstractItemView {
            background-color: #16213e;
            color: #e2e8f0;
            selection-background-color: #7c6af0;
        }
        QLabel#lbScore { color: #a8b4d8; }
    )");

    // estado inicial: tabelas 2x2 e painel de exercícios limpo
    ui->spinRowsA->setValue(2); ui->spinColsA->setValue(2);
    ui->spinRowsB->setValue(2); ui->spinColsB->setValue(2);
    ui->tableA->setRowCount(2); ui->tableA->setColumnCount(2);
    ui->tableB->setRowCount(2); ui->tableB->setColumnCount(2);
    ui->leAnswer->hide();
    ui->tableAnswer->hide();
    ui->lbErrorText->setText("");
    ui->lbScore->setText("Acertos: 0 | Erros: 0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ── Calculadora ───────────────────────────────────────────────

void MainWindow::on_btnSum_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               sum(readTable(ui->tableA), readTable(ui->tableB)),
               "Error: dimensions must be equal!");
}

void MainWindow::on_btnSubtract_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               sum(readTable(ui->tableA), readTable(ui->tableB), -1),
               "Error: dimensions must be equal!");
}

void MainWindow::on_btnMultiply_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               multiply(readTable(ui->tableA), readTable(ui->tableB)),
               "Error: cols(A) must equal rows(B)!");
}

void MainWindow::on_btMultiplyA_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               scalar(readTable(ui->tableA), ui->spinValueA->value()),
               "Error: matrix A is empty!");
}

void MainWindow::on_btMultiplyB_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               scalar(readTable(ui->tableB), ui->spinValueB->value()),
               "Error: matrix B is empty!");
}

void MainWindow::on_btTransposeA_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               transpose(readTable(ui->tableA)), "Error: matrix A is empty!");
}

void MainWindow::on_btTransposeB_clicked(){
    showResult(ui->tableResult, ui->lbErrorText,
               transpose(readTable(ui->tableB)), "Error: matrix B is empty!");
}

void MainWindow::on_btDeterminantA_clicked(){ showDet(ui->tableA, ui->lbErrorText, "A"); }
void MainWindow::on_btDeterminantB_clicked(){ showDet(ui->tableB, ui->lbErrorText, "B"); }

void MainWindow::on_btInverseA_clicked(){
    Matrix inv;
    inverse(readTable(ui->tableA), inv);
    showResult(ui->tableResult, ui->lbErrorText, inv,
               "Error: matrix must be square and invertible (det ≠ 0)!");
}

void MainWindow::on_btInverseB_clicked(){
    Matrix inv;
    inverse(readTable(ui->tableB), inv);
    showResult(ui->tableResult, ui->lbErrorText, inv,
               "Error: matrix must be square and invertible (det ≠ 0)!");
}

void MainWindow::on_spinRowsA_valueChanged(int v){ ui->tableA->setRowCount(v); }
void MainWindow::on_spinColsA_valueChanged(int v){ ui->tableA->setColumnCount(v); }
void MainWindow::on_spinRowsB_valueChanged(int v){ ui->tableB->setRowCount(v); }
void MainWindow::on_spinColsB_valueChanged(int v){ ui->tableB->setColumnCount(v); }

// ── Exercícios ────────────────────────────────────────────────

void MainWindow::on_btNewExercise_clicked() {
    currentEx = generateExercise(ui->cbLevel->currentIndex() + 1);
    hasExercise = true;

    ui->spinRowsA->setValue(currentEx.A.size());
    ui->spinColsA->setValue(currentEx.A[0].size());
    setTable(ui->tableA, currentEx.A);

    if (!currentEx.B.empty()) {
        ui->spinRowsB->setValue(currentEx.B.size());
        ui->spinColsB->setValue(currentEx.B[0].size());
        setTable(ui->tableB, currentEx.B);
    } else {
        ui->tableB->clearContents();
    }

    ui->lbExercise->setText(currentEx.statement);
    ui->lbErrorText->setStyleSheet("");
    ui->lbErrorText->setText("");
    ui->tableResult->clear();

    ui->leAnswer->setVisible(currentEx.isNumeric);
    ui->tableAnswer->setVisible(!currentEx.isNumeric);
    ui->leAnswer->clear();
    if (!currentEx.isNumeric) {
        ui->tableAnswer->clear();
        ui->tableAnswer->setRowCount(currentEx.answer.size());
        ui->tableAnswer->setColumnCount(currentEx.answer[0].size());
    }
}

void MainWindow::on_btCheck_clicked() {
    ui->lbErrorText->setStyleSheet("");
    if (!hasExercise) {
        ui->lbErrorText->setText("Gere um exercício primeiro!");
        return;
    }

    bool ok = true;
    Matrix given;
    double num = 0;
    if (currentEx.isNumeric) num = ui->leAnswer->text().replace(',', '.').toDouble(&ok);
    else given = readTable(ui->tableAnswer, &ok);

    if (!ok) {
        ui->lbErrorText->setText("Preencha a resposta com números válidos.");
        return;
    }

    bool correct = checkAnswer(currentEx, given, num);
    if (correct) hits++; else misses++;
    ui->lbScore->setText(QString("Acertos: %1 | Erros: %2").arg(hits).arg(misses));

    if (correct) {
        ui->lbErrorText->setStyleSheet("color:#4caf87; font-weight:bold;");
        ui->lbErrorText->setText("Correto! Parabéns!");
    } else {
        ui->lbErrorText->setText("Resposta incorreta. Tente de novo ou veja a resolução.");
    }
}

void MainWindow::on_btSolution_clicked() {
    if (!hasExercise) return;
    ui->lbErrorText->setStyleSheet("");
    QString txt = currentEx.solution;
    if (currentEx.isNumeric) txt += "\nResposta: " + fmt(currentEx.answerNumber);
    ui->lbErrorText->setText(txt);
    if (!currentEx.isNumeric) setTable(ui->tableResult, currentEx.answer);
}
