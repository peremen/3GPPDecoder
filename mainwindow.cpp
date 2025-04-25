/* This file is part of 3GPP Decoder project.
 * Copyright (C) 2015  Prashant Panigrahi prashant@3glteinfo.com
 * Copyright (C) 2018-2025 Shinjo Park
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencedialog.h"
#include "tsharkdecoder.h"
#include "umtsrlcdecoder.h"
#include "aboutdialog.h"
#include <QTemporaryFile>
#include <QDir>

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QTextCodec>
#else
#include <QStringConverter>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    on_radioButtonLte_toggled(true);

    /* GSM dissectors */
    gsmModel.addData("GAN.TCP", "umatcp");
    gsmModel.addData("GAN.UDP", "umaudp");
    gsmModel.addData("LLC", "llcgprs");
    gsmModel.addData("NAS/RR", "gsm_a_dtap");
    gsmModel.addData("RR_dl_with_pseudolength", "gsm_a_ccch");
    gsmModel.addData("RR_ul_with_pseudolength", "gsm_a_sacch");
    gsmModel.addData("SNDCP", "sndcp");
    gsmModel.addData("SNDCPXID", "sndcpxid");
    gsmModel.addData("LAPDm", "lapdm");
    gsmModel.addData("RLC MAC DL", "gsm_rlcmac_dl");
    gsmModel.addData("RLC MAC UL", "gsm_rlcmac_ul");

    /* UMTS dissectors */
    umtsModel.addData("L3", "gsm_a_dtap");
    umtsModel.addData("RLC-AM", "RLC-AM");
    umtsModel.addData("RLC-UM", "RLC-UM");
    umtsModel.addData("RRC.BCCH.BCH", "rrc.bcch.bch");
    umtsModel.addData("RRC.BCCH.FACH", "rrc.bcch.fach");
    umtsModel.addData("RRC.DL.CCCH", "rrc.dl.ccch");
    umtsModel.addData("RRC.DL.DCCH", "rrc.dl.dcch");
    umtsModel.addData("RRC.DL.SHCCH", "rrc.dl.shcch");
    umtsModel.addData("RRC.MCCH", "rrc.mcch");
    umtsModel.addData("RRC.MSCH", "rrc.msch");
    umtsModel.addData("RRC.PCCH", "rrc.pcch");
    umtsModel.addData("RRC.UL.CCCH", "rrc.ul.ccch");
    umtsModel.addData("RRC.UL.DCCH", "rrc.ul.dcch");
    umtsModel.addData("RRC.UL.SHCCH", "rrc.ul.shcch");
    umtsModel.addData("RRC.SI.MIB", "rrc.si.mib");
    umtsModel.addData("RRC.SI.SB1", "rrc.sb1");
    umtsModel.addData("RRC.SI.SB2", "rrc.sb2");
    umtsModel.addData("RRC.SI.SIB1", "rrc.si.sib1");
    umtsModel.addData("RRC.SI.SIB2", "rrc.si.sib2");
    umtsModel.addData("RRC.SI.SIB3", "rrc.si.sib3");
    umtsModel.addData("RRC.SI.SIB4", "rrc.si.sib4");
    umtsModel.addData("RRC.SI.SIB5", "rrc.si.sib5");
    umtsModel.addData("RRC.SI.SIB5bis", "rrc.si.sib5bis");
    umtsModel.addData("RRC.SI.SIB6", "rrc.si.sib6");
    umtsModel.addData("RRC.SI.SIB7", "rrc.si.sib7");
    umtsModel.addData("RRC.SI.SIB8", "rrc.si.sib8");
    umtsModel.addData("RRC.SI.SIB9", "rrc.si.sib9");
    umtsModel.addData("RRC.SI.SIB10", "rrc.si.sib10");
    umtsModel.addData("RRC.SI.SIB11", "rrc.si.sib11");
    umtsModel.addData("RRC.SI.SIB11bis", "rrc.si.sib11bis");
    umtsModel.addData("RRC.SI.SIB12", "rrc.si.sib12");
    umtsModel.addData("RRC.SI.SIB13", "rrc.si.sib13");
    umtsModel.addData("RRC.SI.SIB13-1", "rrc.si.sib13-1");
    umtsModel.addData("RRC.SI.SIB13-2", "rrc.si.sib13-2");
    umtsModel.addData("RRC.SI.SIB13-3", "rrc.si.sib13-3");
    umtsModel.addData("RRC.SI.SIB13-4", "rrc.si.sib13-4");
    umtsModel.addData("RRC.SI.SIB14", "rrc.si.sib14");
    umtsModel.addData("RRC.SI.SIB15", "rrc.si.sib15");
    umtsModel.addData("RRC.SI.SIB15bis", "rrc.si.sib15bis");
    umtsModel.addData("RRC.SI.SIB15-1", "rrc.si.sib15-1");
    umtsModel.addData("RRC.SI.SIB15-1bis", "rrc.si.sib15-1bis");
    umtsModel.addData("RRC.SI.SIB15-2", "rrc.si.sib15-2");
    umtsModel.addData("RRC.SI.SIB15-2bis", "rrc.si.sib15-2bis");
    umtsModel.addData("RRC.SI.SIB15-3", "rrc.si.sib15-3");
    umtsModel.addData("RRC.SI.SIB15-3bis", "rrc.si.sib15-3bis");
    umtsModel.addData("RRC.SI.SIB15-4", "rrc.si.sib15-4");
    umtsModel.addData("RRC.SI.SIB15-5", "rrc.si.sib15-5");
    umtsModel.addData("RRC.SI.SIB15-6", "rrc.si.sib15-6");
    umtsModel.addData("RRC.SI.SIB15-7", "rrc.si.sib15-7");
    umtsModel.addData("RRC.SI.SIB15-8", "rrc.si.sib15-8");
    umtsModel.addData("RRC.SI.SIB16", "rrc.si.sib16");
    umtsModel.addData("RRC.SI.SIB17", "rrc.si.sib17");
    umtsModel.addData("RRC.SI.SIB18", "rrc.si.sib18");
    umtsModel.addData("RRC.SI.SIB19", "rrc.si.sib19");
    umtsModel.addData("RRC.SI.SIB20", "rrc.si.sib20");
    umtsModel.addData("RRC.SI.SIB21", "rrc.si.sib21");
    umtsModel.addData("RRC.SI.SIB22", "rrc.si.sib22");
    umtsModel.addData("RRC.SYSINFO.CONT", "rrc.sysinfo.cont");

    /* LTE dissectors */
    lteModel.addData("LTE-RRC.BCCH.BCH", "lte-rrc.bcch.bch");
    lteModel.addData("LTE-RRC.BCCH.DL.SCH", "lte-rrc.bcch.dl.sch");
    lteModel.addData("LTE-RRC.DL.CCCH", "lte-rrc.dl.ccch");
    lteModel.addData("LTE-RRC.DL.DCCH", "lte-rrc.dl.dcch");
    lteModel.addData("LTE-RRC.PCCH", "lte-rrc.pcch");
    lteModel.addData("LTE-RRC.MCCH", "lte-rrc.mcch");
    lteModel.addData("LTE-RRC.UL.CCCH", "lte-rrc.ul.ccch");
    lteModel.addData("LTE-RRC.UL.DCCH", "lte-rrc.ul.dcch");

    lteModel.addData("LTE-RRC.BCCH.BCH-NB", "lte-rrc.bcch.bch.nb");
    lteModel.addData("LTE-RRC.BCCH.DL.SCH-NB", "lte-rrc.bcch.dl.sch.nb");
    lteModel.addData("LTE-RRC.DL.CCCH-NB", "lte-rrc.dl.ccch.nb");
    lteModel.addData("LTE-RRC.DL.DCCH-NB", "lte-rrc.dl.dcch.nb");
    lteModel.addData("LTE-RRC.PCCH-NB", "lte-rrc.pcch.nb");
    lteModel.addData("LTE-RRC.SC-MCCH-NB", "lte-rrc.sc.mcch.nb");
    lteModel.addData("LTE-RRC.UL.CCCH-NB", "lte-rrc.ul.ccch.nb");
    lteModel.addData("LTE-RRC.UL.DCCH-NB", "lte-rrc.ul.dcch.nb");

    lteModel.addData("NAS-EPS", "nas-eps");
    lteModel.addData("NAS-EPS (Plain)", "nas-eps_plain");
    lteModel.addData("LTE MAC", "mac-lte");
    lteModel.addData("LTE MAC (framed)", "mac-lte-framed");
    lteModel.addData("LTE PDCP", "pdcp-lte");

    /* 5GNR dissectors */
    nrModel.addData("NR-RRC.BCCH.BCH", "nr-rrc.bcch.bch");
    nrModel.addData("NR-RRC.BCCH.DL.SCH", "nr-rrc.bcch.dl.sch");
    nrModel.addData("NR-RRC.DL.CCCH", "nr-rrc.dl.ccch");
    nrModel.addData("NR-RRC.DL.DCCH", "nr-rrc.dl.dcch");
    nrModel.addData("NR-RRC.PCCH", "nr-rrc.pcch");
    nrModel.addData("NR-RRC.UL.CCCH", "nr-rrc.ul.ccch");
    nrModel.addData("NR-RRC.UL.CCCH1", "nr-rrc.ul.ccch1");
    nrModel.addData("NR-RRC.UL.DCCH", "nr-rrc.ul.dcch");
    nrModel.addData("NR-RRC.SBCCH.SL.BCH", "nr-rrc.sbcch.sl.bch");
    nrModel.addData("NR-RRC.SCCH", "nr-rrc.scch");
    nrModel.addData("NR-RRC.UERadioPagingInformation", "nr-rrc.ue_radio_paging_info");
    nrModel.addData("NR-RRC.UERadioAccessCapabilityInformation", "nr-rrc.ue_radio_access_cap_info");
    nrModel.addData("NR-RRC.RRCReconfiguration", "nr-rrc.rrc_reconf");
    nrModel.addData("NR-RRC.RRCReconfigurationComplete", "nr-rrc.rrc_reconf_compl");
    nrModel.addData("NR-RRC.UEMRDCCapability", "nr-rrc.ue_mrdc_cap");
    nrModel.addData("NR-RRC.UENRCapability", "nr-rrc.ue_nr_cap");
    nrModel.addData("NR-RRC.SIB1", "nr-rrc.sib1");
    nrModel.addData("NR-RRC.SIB2", "nr-rrc.sib2");
    nrModel.addData("NR-RRC.SIB3", "nr-rrc.sib3");
    nrModel.addData("NR-RRC.SIB4", "nr-rrc.sib4");
    nrModel.addData("NR-RRC.SIB5", "nr-rrc.sib5");
    nrModel.addData("NR-RRC.SIB6", "nr-rrc.sib6");
    nrModel.addData("NR-RRC.SIB7", "nr-rrc.sib7");
    nrModel.addData("NR-RRC.SIB8", "nr-rrc.sib8");
    nrModel.addData("NR-RRC.SIB9", "nr-rrc.sib9");
    nrModel.addData("NR-RRC.SIB10", "nr-rrc.sib10");
    nrModel.addData("NR-RRC.SIB11", "nr-rrc.sib11");
    nrModel.addData("NR-RRC.SIB12", "nr-rrc.sib12");
    nrModel.addData("NR-RRC.SIB13", "nr-rrc.sib13");
    nrModel.addData("NR-RRC.SIB14", "nr-rrc.sib14");
    nrModel.addData("NR-RRC.SIB15", "nr-rrc.sib15");
    nrModel.addData("NR-RRC.SIB16", "nr-rrc.sib16");
    nrModel.addData("NR-RRC.SIB17", "nr-rrc.sib17");
    nrModel.addData("NR-RRC.SIB18", "nr-rrc.sib18");
    nrModel.addData("NR-RRC.SIB19", "nr-rrc.sib19");
    nrModel.addData("NR-RRC.SIB20", "nr-rrc.sib20");
    nrModel.addData("NR-RRC.SIB21", "nr-rrc.sib21");
    nrModel.addData("NR-RRC.SIB22", "nr-rrc.sib22");
    nrModel.addData("NR-RRC.SIB23", "nr-rrc.sib23");
    nrModel.addData("NR-RRC.SIB24", "nr-rrc.sib24");
    nrModel.addData("NR-RRC.SIB25", "nr-rrc.sib25");
    nrModel.addData("NR-RRC.SIB17bis", "nr-rrc.sib17bis");

    nrModel.addData("NAS-5GS", "nas-5gs");
    nrModel.addData("NR MAC", "mac-nr");
    nrModel.addData("NR PDCP", "pdcp-nr");

    /* IP dissectors */
    ipModel.addData("IPv4", "ip");
    ipModel.addData("IPv6", "ipv6");
    ipModel.addData("ICMP", "icmp");
    ipModel.addData("RTSP", "rtsp");
    ipModel.addData("RTP", "rtp");
    ipModel.addData("UDP", "udp");
    ipModel.addData("GSMTAP", "gsmtap");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonDecode_clicked()
{
    QString encoded_pdu;
    QString protocol_selected;
    int pdu_length = 0;
    QMessageBox* message_box = new QMessageBox();
    QTemporaryFile fileResult(QDir::tempPath() + QDir::separator() + "3GPPDecoder.dec.XXXXXX.pcap");

    encoded_pdu = ui->lineEditData->text();
    protocol_selected = ui->comboBoxProtocol->currentData(Qt::UserRole).toString();
    pdu_length = encoded_pdu.length();

    if (pdu_length == 0) {
        message_box->setText("Enter a HEX string to decode");
        message_box->setWindowTitle("Error...");
        message_box->show();
        return;
    }

    fileResult.open();
    fileResult.close();
    if (protocol_selected != QStringLiteral("")) {
        if (protocol_selected.contains("RLC")) {
            encoded_pdu = encoded_pdu.remove(" ");
            encoded_pdu = encoded_pdu.remove("\n");
            if ((encoded_pdu.length()%2) != 0) {
                message_box->setText("Check entered data: RLC PDUs are octect aligned.");
            } else {
                if(ui->radioButtonUmts->isChecked()) {
                    UmtsRlcDecoder* umts_rlc_decoder = new UmtsRlcDecoder();
                    umts_rlc_decoder->start_decoder(encoded_pdu, protocol_selected, fileResult.fileName());
                }
            }
        } else {
            //Decode as RRC with TShark
            TSharkDecoder* tshark_decoder = new TSharkDecoder();
            tshark_decoder->startDecoder(encoded_pdu, protocol_selected, fileResult.fileName());
        }
        readfile(fileResult.fileName());
    }
}

void MainWindow::readfile(QString fileName){
    QByteArray ba;
    QFile file(fileName);

    if (!file.exists()) {
        qDebug() << "No file found "<< fileName;
        return;
    } else {
        qDebug() << fileName <<" File read sucess...";
    }

    ui->textEditDecoded->clear();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ba = file.readAll();
        file.close();
    }
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QTextCodec *c = QTextCodec::codecForLocale();
    ui->textEditDecoded->setText(c->toUnicode(ba));
#else
    auto c = QStringDecoder(QStringDecoder::System);
    ui->textEditDecoded->setText(c(ba));
#endif
}

void MainWindow::on_radioButtonGsm_toggled(bool checked)
{
    if (checked) {
        ui->comboBoxProtocol->setModel(&gsmModel);
    }
}

void MainWindow::on_radioButtonUmts_toggled(bool checked)
{
    if (checked) {
        ui->comboBoxProtocol->setModel(&umtsModel);
    }
}


void MainWindow::on_radioButtonLte_toggled(bool checked)
{
    if (checked) {
        ui->comboBoxProtocol->setModel(&lteModel);
    }
}

void MainWindow::on_radioButton5GNr_toggled(bool checked)
{
    if (checked) {
        ui->comboBoxProtocol->setModel(&nrModel);
    }

}

void MainWindow::on_radioButtonIP_toggled(bool checked) {
    if (checked) {
        ui->comboBoxProtocol->setModel(&ipModel);
    }
}

void MainWindow::on_action_Preference_triggered()
{
    PreferenceDialog prefrence;
    prefrence.setModal(true);
    prefrence.exec();
}

void MainWindow::on_pushButtonClear_clicked()
{
    ui->textEditDecoded->clear();
    ui->lineEditData->clear();
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_About_Decoder_triggered()
{
    AboutDialog about;
    about.setModal(true);
    about.exec();
}
