/* This file is part of 3GPP Decoder project.
 * Copyright (C) 2015  Prashant Panigrahi prashant@3glteinfo.com
 * Copyright (C) 2015-2025 Shinjo Park
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCursor>

#include "dissectormodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonDecode_clicked();
    void on_radioButtonGsm_toggled(bool checked);
    void on_radioButtonUmts_toggled(bool checked);
    void on_radioButtonLte_toggled(bool checked);
    void on_radioButton5GNr_toggled(bool checked);
    void on_radioButtonIP_toggled(bool checked);
    void on_action_Preference_triggered();
    void on_pushButtonClear_clicked();
    void on_action_Exit_triggered();
    void on_action_About_Decoder_triggered();

private:
    Ui::MainWindow *ui;
    void readfile(QString fileName);
    void setDefaultPreference();
    DissectorModel gsmModel, umtsModel, lteModel, nrModel, ipModel;
};

#endif // MAINWINDOW_H
