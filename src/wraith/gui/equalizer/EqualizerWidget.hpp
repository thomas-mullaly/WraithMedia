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
#ifndef WRAITH_GUI_EQUALIZER_EQUALIZER_WIDGET
#define WRAITH_GUI_EQUALIZER_EQUALIZER_WIDGET

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QList>

#include <wraith/core/playback/IEqualizer.hpp>

#include "EqualizerBand.hpp"

namespace wraith { namespace gui { namespace equalizer {

class EqualizerWidget : public QWidget {
Q_OBJECT
public:
    EqualizerWidget(playback::IEqualizer *equalizer, QWidget *parent = 0);
    virtual ~EqualizerWidget();

    playback::IEqualizer*  equalizer() const;

private slots:
    void bandChanged(int band, int value);
    void preampChanged(int, int value);
    void enabledChanged(int state);

private:
    void setup();

    playback::IEqualizer *eq;

    QVBoxLayout             *vbox;
    QHBoxLayout             *hbox;
    QCheckBox               *enable_box;
    QList<EqualizerBand*>   bands;
    EqualizerBand           *preamp;
    double                  default_preamp;
};

} } } // namespace wraith::gui::equalizer

#endif // WRAITH_GUI_EQUALIZER_EQUALIZER_WIDGET
