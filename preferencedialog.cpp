/* This file is part of 3GPP Decoder project.
 * Copyright (C) 2015  Prashant Panigrahi prashant@3glteinfo.com
 * Copyright (C) 2021-2025 Shinjo Park
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

#include "preferencedialog.h"
#include "ui_preferencedialog.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    setDefaults();
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::setDefaults()
{
    QSettings settings;
    QString wiresharkDefaultPath;

#if defined(Q_OS_WIN)
    wiresharkDefaultPath = "C:\\Program Files\\Wireshark\\";
#elif defined(Q_OS_MACOS)
    wiresharkDefaultPath = "/Applications/Wireshark.app/Contents/MacOS/";
#elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    wiresharkDefaultPath = "/usr/bin";
#endif

    QString strCurrentWiresharkPath = settings.value("path/wireshark", wiresharkDefaultPath).toString();
    ui->lineEditWireshark->setText(strCurrentWiresharkPath);
    ui->lblDefaultPath->setText(tr("Default: %1").arg(wiresharkDefaultPath));
}

void PreferenceDialog::on_buttonBox_accepted()
{
    QSettings settings;
    QDir wsdir(ui->lineEditWireshark->text());
    if (wsdir.exists()) {
        settings.setValue("path/wireshark", wsdir.canonicalPath());
    }
}

void PreferenceDialog::on_btnBrowse_clicked() {
    QSettings settings;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    ui->lineEditWireshark->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEditWireshark->setText(dir);
}
