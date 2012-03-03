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
#include "EqualizerBandLabel.hpp"

#include <QPainter>
#include <QDebug>

using namespace wraith::gui::equalizer;

EqualizerBandLabel::EqualizerBandLabel(QWidget *parent)
    : QLabel(parent)
{ resize(10, height()); }

EqualizerBandLabel::~EqualizerBandLabel() { }

void EqualizerBandLabel::paintEvent(QPaintEvent *evt) {
    QPainter *p = new QPainter(this);

    p->save();
    p->translate(width() / 2, height());
    p->rotate(-90);
    p->drawText(0, 0, text());
    p->restore();

    delete p;
}
