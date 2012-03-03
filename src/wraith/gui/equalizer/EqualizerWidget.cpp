/*
 * Copyright 2010 Thomas Mullaly
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
#include "EqualizerWidget.hpp"

#include <QDebug>

using namespace wraith::playback;
using namespace wraith::gui::equalizer;

EqualizerWidget::EqualizerWidget(IEqualizer *equalizer, QWidget *parent)
    : QWidget(parent)
    , eq(equalizer)
    , vbox()
    , hbox()
    , enable_box()
    , bands()
    , default_preamp()
{ setup(); }

EqualizerWidget::~EqualizerWidget()
{ }

IEqualizer* EqualizerWidget::equalizer() const {
    return eq;
}

void EqualizerWidget::bandChanged(int band, int value) {
    if(enable_box->checkState() == Qt::Checked)
        eq->setBandGain(band, value);
}

void EqualizerWidget::preampChanged(int /* band */, int value) {
    if(enable_box->checkState() == Qt::Checked)
        eq->setAmplifierLevel(double(value) / 10.0);
}

void EqualizerWidget::enabledChanged(int state) {
    if(state == Qt::Checked) {
        eq->setAmplifierLevel(double(preamp->currentValue()) / 10.0);
        for(int i = 0; i < bands.size(); ++i) {
            EqualizerBand *band = bands[i];
            eq->setBandGain(i, band->currentValue());
        }
    } else {
        // Restore all the values to middle.
        std::pair<int,int> range = eq->bandRanges();
        int mid = (range.first + range.second) / 2;
        eq->setAmplifierLevel(default_preamp);
        for(int i = 0; i < eq->numberOfBands(); ++i)
            eq->setBandGain(i, mid);
    }
}

void EqualizerWidget::setup() {
    vbox = new QVBoxLayout(this);

    enable_box = new QCheckBox("Enabled", this);
    enable_box->setCheckState(Qt::Unchecked);
    connect(enable_box, SIGNAL(stateChanged(int)), this, SLOT(enabledChanged(int)));

    vbox->addWidget(enable_box);
    vbox->setSpacing(0);

    hbox = new QHBoxLayout();
    hbox->setSpacing(0);

    std::vector<double> freqs = eq->equalizerFrequencies();
    std::pair<int,int> range = eq->bandRanges();

    int ranges[3];
    ranges[0] = range.first;
    ranges[1] = (range.first + range.second) / 2;
    ranges[2] = range.second;

    preamp = new EqualizerBand("Preamp", ranges[0] * 10, ranges[2] * 10, ranges[1] * 10, 0, this);
    default_preamp = eq->amplifierLevel();
    connect(preamp, SIGNAL(userChangedValue(int,int)), this, SLOT(preampChanged(int,int)));
    hbox->addWidget(preamp);

    QVBoxLayout *bandInfoLayout = new QVBoxLayout();
    bandInfoLayout->setSpacing(0);
    QWidget *bandInfo = new QWidget(this);
    QList<QLabel*> labels;

    for(int i = 2; i >= 0; --i) {
        QString txt;
        if(ranges[i] > 0)
            txt = "+" + QString::number(ranges[i]) + " dB";
        else
            txt = QString::number(ranges[i]) + " dB";

        QLabel *lbl = new QLabel(txt, bandInfo);
        bandInfoLayout->addWidget(lbl, 30, Qt::AlignRight);
        labels.push_back(lbl);
    }

    labels[0]->setAlignment(Qt::AlignTop);
    labels[1]->setAlignment(Qt::AlignVCenter);
    labels[2]->setAlignment(Qt::AlignBottom);

    bandInfo->setLayout(bandInfoLayout);
    hbox->addWidget(bandInfo);

    for(size_t i = 0; i < eq->numberOfBands(); ++i) {
        QString text = "";
        if(freqs[i] >= 1000) {
            text += QString::number(int(freqs[i] / 1000));
            text += " kHz";
        } else {
            text += QString::number(int(freqs[i]));
            text += " Hz";
        }

        EqualizerBand *band = new EqualizerBand(text, range.first, range.second, int(eq->bandGain(i)), i, this);
        connect(band, SIGNAL(userChangedValue(int,int)), this, SLOT(bandChanged(int,int)));

        hbox->addWidget(band);
        bands.push_back(band);
    }

    QWidget *tmp = new QWidget(this);
    tmp->setLayout(hbox);
    vbox->addWidget(tmp);
    setLayout(vbox);
}
