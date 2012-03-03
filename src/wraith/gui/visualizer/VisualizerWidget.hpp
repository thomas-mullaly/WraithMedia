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
#ifndef WRAITH_GUI_VISUALIZER_VISUALIZER_WIDGET_HPP
#define WRAITH_GUI_VISUALIZER_VISUALIZER_WIDGET_HPP

#include <QWidget>
#include <QGLWidget>
#include <QList>
#include <QMutex>

#include <wraith/core/playback/IVisualizationSource.hpp>

namespace wraith { namespace gui { namespace visualizer {

class VisualizerWidget : public QGLWidget, public playback::IVisualizationDataCallback {
Q_OBJECT
public:
    VisualizerWidget(playback::IVisualizationSource *dataSource, QWidget *parent = 0);
    virtual ~VisualizerWidget();

    virtual void onVisualizationData(VisualizationDataPtr visData);

private:
    void mergeSpectrum(visualization::VisualizationData::SpectrumData const& spectrum, int bands);
    void updatePCMData(visualization::VisualizationData::PCMData const& pcmData, int samples);

signals:
    void draw();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    enum color {
        red, green, blue
    };

    QList<float> spectrum;
    QList<float> floats;
    QList<float> pcm_data;
    QMutex      mutex;
    int current_color_val, prev_color_val;
    float float_height;
    color current_color;
    int threshold;
    playback::IVisualizationSource *source;
};

} } } // namespace wraith::gui::visualizer

#endif // WRAITH_GUI_VISUALIZER_WIDGET_HPP
