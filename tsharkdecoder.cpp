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

#include "tsharkdecoder.h"
#include <QTextStream>
#include <QTemporaryFile>
#include <QDir>
#include <QProcess>
#include <QStringList>
#include <QSettings>

TSharkDecoder::TSharkDecoder()
{
    QSettings settings;
    keepTmpFile = settings.value("wireshark/keep_temp_file", false).toBool();
    userDlt = settings.value("wireshark/dlt", 0).toInt();

    QString wiresharkDefaultPath;

#if defined(Q_OS_WIN)
    wiresharkDefaultPath = "C:\\Program Files\\Wireshark\\";
#elif defined(Q_OS_MACOS)
    wiresharkDefaultPath = "/Applications/Wireshark.app/Contents/MacOS/";
#elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    wiresharkDefaultPath = "/usr/bin";
#endif

    QString strCurrentWiresharkPath = settings.value("path/wireshark", wiresharkDefaultPath).toString();
    tsharkPath = strCurrentWiresharkPath;
}

TSharkDecoder::~TSharkDecoder()
{

}

/* Decoding starts here
 */

void TSharkDecoder::startDecoder(QString strEncodedData, QString strProtocol, QString fileName)
{
    QString strData;
    QTemporaryFile fileText2PcapTxt;
    QTemporaryFile fileTempPcap(QDir::tempPath() + QDir::separator() + "3GPPDecoder.XXXXXX.pcap");
    fileTempPcap.setAutoRemove(!keepTmpFile);

    strData = preformatData(strEncodedData);

    fileText2PcapTxt.open();
    fileText2PcapTxt.close();
    format_file_for_text2pcap(strData, fileText2PcapTxt.fileName());

    fileTempPcap.open();
    fileTempPcap.close();
    call_text2pcap(fileText2PcapTxt.fileName(), fileTempPcap.fileName());

    call_tshark(strProtocol, fileTempPcap.fileName(), fileName);

    tmpPcapFileName = fileTempPcap.fileName();
    clean_output(fileName);
}

/* Make the incoming packets in HEX format.
 * The packets are encoded in this format:
 * ab - ab
 * a - 0a
 * abcd - ab cd
 */

QString TSharkDecoder::preformatData(QString strEncodedData){
    QString ret;
    QTextStream out(&ret);
    QString t = strEncodedData.simplified(), d;

    t.replace(" ", "");

    if (t.length() % 2 == 1) {
        t.append('0');
    }

    int lines = t.length() / 32;
    int chars = t.length() / 2, curchar = 0;
    if (t.length() % 32 > 0) lines++;

    for (int i = 0; i < lines; i++) {
        d = QString("%1 ").arg(i * 16, 5, 16, QChar('0'));
        out << d;
        while (chars > 0) {
            out << t.mid(2 * curchar, 2) << ' ';
            curchar++;
            chars--;
            if (curchar % 16 == 0){
                out << '\n';
                break;
            }
        }
    }
    out << '\n';
    out.flush();
    return ret;
}

/* Create a Textfile which text2pcap can understand.
 * 0000 are added to the beginning of the HEX data
 * to make it text2pacp type
 */

void TSharkDecoder::format_file_for_text2pcap(QString strData, QString fileName) {
    QFile textFile(fileName);
    if (textFile.open(QIODevice::Text | QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&textFile);
        stream << "0000";
        stream << strData;
        stream.flush();
    }
    textFile.close();
}

/* Make a PCAP file from the textdata.
 * Function calls text2pack.exe from wireshark folder.
 */

void TSharkDecoder::call_text2pcap(QString textFileName, QString pcapFileName) {
    QString text2pcapCmd = QDir::cleanPath(tsharkPath + QDir::separator() + "text2pcap");
    QStringList text2pcapArgs;
    text2pcapArgs << "-F" << "pcap" << "-q" << "-l" << QString::number(userDlt + 147) << textFileName << pcapFileName;
    qDebug() << text2pcapArgs;

    QProcess *text2pcapProcess = new QProcess();
    text2pcapProcess->start(text2pcapCmd, text2pcapArgs);
    text2pcapProcess->waitForFinished();
}

/* Call Tshark to decode the PCAP file.
 * RRC messages are decoded directly
 * NAS messages embeded are also parsed by Tshark.
 */
void TSharkDecoder::call_tshark(QString strProtocol, QString pcapFileName, QString outputFileName) {
    QSettings settings;
    QString tsharkCmd = QDir::cleanPath(tsharkPath + QDir::separator() + "tshark");
    QString userdltsString = "uat:user_dlts:\"User %1 (DLT=%2)\",\"%3\",\"0\",\"\",\"0\",\"\"";
    QStringList tsharkArgs;
    tsharkArgs << "-o" << userdltsString.arg(userDlt).arg(userDlt + 147).arg(strProtocol);
    tsharkArgs << "-r" << pcapFileName << "-V" << "-l" << "-x";
    qDebug() << tsharkArgs;

    QProcess *tsharkProcess = new QProcess();
    tsharkProcess->setStandardOutputFile(outputFileName);
    tsharkProcess->start(tsharkCmd, tsharkArgs);
    tsharkProcess->waitForFinished();
}

/* After decode the first 15 lines are useless data for us.
 * These lines can be removed for a clean presentation
 */

void TSharkDecoder::clean_output(QString fileName)
{
    QFile f(fileName);
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        int i = 0;
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(i>=15)
                s.append(line + "\n");
            i++;
        }
        f.resize(0);
        t << s;
        if (keepTmpFile)
        {
            t << QString("\nKeeping temporary PCAP file at %1\n").arg(tmpPcapFileName);
        }
        f.close();
    }
}
