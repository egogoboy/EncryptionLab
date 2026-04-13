#pragma once

#include <QDialog>
#include <QLineEdit>

class KeyDialog : public QDialog {
    Q_OBJECT

   public:
    explicit KeyDialog(QWidget* parent = nullptr);

    void reset_input();

   signals:
    void key_submitted(const QString& key);

   private slots:
    void on_submit();
    void on_reject();

   private:
    QLineEdit* _input;
    QPushButton* _submit_button;
    QPushButton* _reject_button;
};
