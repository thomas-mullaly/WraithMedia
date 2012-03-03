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
#ifndef WRAITH_GUI_EQUALIZER_EQUALIZER_BAND
#define WRAITH_GUI_EQUALIZER_EQUALIZER_BAND

#include <QLabel>
#include <QSlider>

#include "EqualizerBandLabel.hpp"

namespace wraith { namespace gui { namespace equalizer {

class EqualizerBand : public QWidget {
Q_OBJECT
public:
    EqualizerBand(QString const& text, int min, int max, int value, int bandNum, QWidget *parent = 0);
    virtual ~EqualizerBand();

    int     minimumBandRange() const;
    void    setMinimumBandRange(int min);

    int     maximumBandRange() const;
    void    setMaximumBandRange(int max);

    int     currentValue() const;
    void    setCurrentValue(int val);

    int     bandNumber() const;

    QString         text() const;
    void            setText(QString const& text);

private:
    void setupBand(QString const& txt);

signals:
    void    userChangedValue(int band, int val);

private slots:
    void    valueChanged(int value);

private:
    int min, max, value, bandNum;

    QSlider   *band;
    EqualizerBandLabel  *label;
};

} } } // namespace wraith::gui::equalizer
#endif
