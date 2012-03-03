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
#ifndef WRAITH_GUI_EQUALIZER_EQUALIZER_BAND_LABEL_HPP
#define WRAITH_GUI_EQUALIZER_EQUALIZER_BAND_LABEL_HPP

#include <QLabel>

namespace wraith { namespace gui { namespace equalizer {

class EqualizerBandLabel : public QLabel {
Q_OBJECT
public:
    EqualizerBandLabel(QWidget *parent = 0);
    virtual ~EqualizerBandLabel();

    virtual void paintEvent(QPaintEvent *evt);
};

} } } // namespace wraith::widgets::equalizer

#endif
