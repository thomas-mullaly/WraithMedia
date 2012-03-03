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
#ifndef WRAITH_GUI_SLIDER_WIDGET_HPP
#define WRAITH_GUI_SLIDER_WIDGET_HPP

#include <QSlider>
#include <QMouseEvent>

namespace wraith { namespace gui {

class SliderWidget : public QSlider {
Q_OBJECT
public:
    SliderWidget(Qt::Orientation orientation, QWidget *parent = 0);
    virtual ~SliderWidget();

signals:
    void userChangedValue(int val);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    
};

} } // namespace wraith::gui

#endif // WRAITH_GUI_SLIDER_WIDGET_HPP
