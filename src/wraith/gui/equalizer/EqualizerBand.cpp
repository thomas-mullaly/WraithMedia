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
#include "EqualizerBand.hpp"

#include <QHBoxLayout>
#include <QDebug>

using namespace wraith::gui::equalizer;

EqualizerBand::EqualizerBand(QString const& label, int min, int max, 
                             int value, int bandNum, QWidget *parent)
    : QWidget(parent)
    , min(min)
    , max(max)
    , value(value)
    , bandNum(bandNum)
    , band()
    , label()
{ setupBand(label); }

EqualizerBand::~EqualizerBand() { }

int EqualizerBand::minimumBandRange() const {
    return min;
}

void EqualizerBand::setMinimumBandRange(int min) {
    this->min = min;
}

int EqualizerBand::maximumBandRange() const {
    return max;
}

void EqualizerBand::setMaximumBandRange(int max) {
    this->max = max;
}

int EqualizerBand::currentValue() const {
    return band->value();
}

void EqualizerBand::setCurrentValue(int value) {
    band->setValue(value);
}

int EqualizerBand::bandNumber() const {
    return bandNum;
}

QString EqualizerBand::text() const {
    return label->text();
}

void EqualizerBand::setText(QString const& text) {
    label->setText(text);
}

void EqualizerBand::setupBand(QString const& text) {
    QHBoxLayout *layout = new QHBoxLayout(this);

    label = new EqualizerBandLabel(this);
    label->setText(text);
    label->setMaximumWidth(20);
    label->resize(10, height());

    band = new QSlider(Qt::Vertical, this);
    band->setTickPosition(QSlider::NoTicks);
    band->setMaximum(max);
    band->setMinimum(min);
    band->setValue(value);
    band->resize(20, height());

    connect(band, SIGNAL(sliderMoved(int)), this, SLOT(valueChanged(int)));

    layout->addWidget(band);
    layout->addWidget(label);
    layout->setSpacing(1);

    setLayout(layout);

    setMaximumWidth(60);
}

void EqualizerBand::valueChanged(int value) {
    emit userChangedValue(bandNum, value);
}
