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
#include <cmath>
#include <algorithm>

#include <QDebug>
#include <QMetaObject>

#include "VisualizerWidget.hpp"

using namespace wraith::playback;
using namespace wraith::visualization;

namespace wraith { namespace gui { namespace visualizer {

VisualizerWidget::VisualizerWidget(IVisualizationSource *dataSource, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
    , spectrum()
    , floats()
    , pcm_data()
    , mutex()
    , current_color_val()
    , prev_color_val()
    , float_height(0.5f)
    , current_color(red)
    , threshold(60.0f)
    , source(dataSource)
{
    if(source->isVisualizationSupported()) {
        //threshold = source->visualizationDataThreshold();
        source->setVisualizationEnabled(true);
        source->registerVisualizationDataCallback(this);
    }
}

VisualizerWidget::~VisualizerWidget() {
    if(source->isVisualizationSupported())
        source->unregisterVisualizationDataCallback(this);
}

void VisualizerWidget::mergeSpectrum(VisualizationData::SpectrumData const& spec, int bands) {
    mutex.lock();
    const bool is_new = (spectrum.size() == 0);
    for(int i = 0; i < bands; ++i) {
        if(is_new) {
            floats.push_back(threshold);
            spectrum.push_back(spec[i]);
        } else {
            if(spec[i] >= spectrum[i]) {
                spectrum[i] = spec[i];
                if(spec[i] > floats[i]) {
                    floats[i] = spec[i];
                } else {
                    floats[i] = floats[i] - 0.002f;
                }
            } else {
                if(spectrum[i] < 1.0f) {
                    spectrum[i] = spectrum[i] - 0.005f;
                }

                if(floats[i] < 1.0f) {
                    floats[i] = floats[i] - 0.002f;
                }
            }
        }
    }
    mutex.unlock();
}

void VisualizerWidget::updatePCMData(VisualizationData::PCMData const& pcmData, int samples) {
    VisualizationData::ChannelData const& data = pcmData[0];

    mutex.lock();
    const bool is_new = (pcm_data.size() == 0);
    for(int i = 0; i < samples; ++i) {
        if(is_new)
            pcm_data.push_back(data[i]);
        else
            pcm_data[i] = data[i];
    }
    mutex.unlock();
}

void VisualizerWidget::initializeGL() {
    glDisable(GL_DEPTH_TEST);
}

void VisualizerWidget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void VisualizerWidget::paintGL() {
    emit draw();

    if(false) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,1);

        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        glOrtho (0, width(), 0, 1, 0, 1);
        glMatrixMode (GL_MODELVIEW);

        float spec_len = 0.0f;
        if(spectrum.size() > 0) {
            spec_len = float(width()) / float(spectrum.size());
        }

        switch(current_color) {
        case red:
            glColor3f(float(current_color_val)/255.0f,0.0f,float(prev_color_val)/255.0f);
            break;
        case green:
            glColor3f(float(prev_color_val)/255.0f,float(current_color_val)/255.0f,0.0f);
            break;
        case blue:
            glColor3f(0.0f,float(prev_color_val)/255.0f,float(current_color_val)/255.0f);
            break;
        }

        glBegin(GL_QUADS);

        for(int i = 0; i < spectrum.size(); ++i) {
            float x1 = -1.0f + float(i) * spec_len;
            float x2 = -1.0f + spec_len * (i + 1.0f);
            float h = spectrum[i];
            float float_bottom = floats[i];

            glVertex2f(x1,h);
            glVertex2f(x2,h);
            glVertex2f(x2,-1);
            glVertex2f(x1,-1);

            if(float_bottom > 0) {
                glVertex2f(x1,float_bottom);
                glVertex2f(x2,float_bottom);
                glVertex2f(x2,float_bottom+float_height);
                glVertex2f(x1,float_bottom+float_height);
            }
        }

        glEnd();
    } else {
        /*glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,1);*/

        glShadeModel(GL_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0, 0, 0.095f);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        /*glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();*/

        glBegin(GL_QUADS);

        glVertex2i(-1, -1);
        glVertex2i( 1, -1);
        glVertex2i( 1,  1);
        glVertex2i(-1,  1);

        glEnd();

        glPopMatrix();

        /*glMatrixMode(GL_PROJECTION);
        glPopMatrix();*/

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glLineWidth(3);

        glTranslatef(-1, 0, 0);
        glScalef(2, 1, 1);

        switch(current_color) {
        case red:
            glColor3f(float(current_color_val)/255.0f,0.0f,float(prev_color_val)/255.0f);
            break;
        case green:
            glColor3f(float(prev_color_val)/255.0f,float(current_color_val)/255.0f,0.0f);
            break;
        case blue:
            glColor3f(0.0f,float(prev_color_val)/255.0f,float(current_color_val)/255.0f);
            break;
        }

        glBegin(GL_LINE_STRIP);

        for (int i = 0; i < pcm_data.size(); i++) {
            glVertex2f((float) i / (pcm_data.size() - 1), pcm_data[i]);
        }

        glEnd();
        glPopMatrix();

        glFlush();
    }
}

void VisualizerWidget::onVisualizationData(VisualizationDataPtr visData) {
    if(isVisible()) {
        if(current_color_val == 255.0f) {
            current_color_val = 0.0f;
            prev_color_val = 255.0f;
            switch(current_color) {
            case red:
                current_color = green;
                break;
            case green:
                current_color = blue;
                break;
            case blue:
                current_color = red;
                break;
            }
        } else {
            --prev_color_val;
            ++current_color_val;
        }

        mergeSpectrum(visData->spectrum(), visData->bands());
        updatePCMData(visData->pcm(), visData->samples());

        // In order to repaint the openGL widget, we must be running from
        // the GUIs thread.
        QMetaObject::invokeMethod(this, "updateGL", Qt::AutoConnection);
    }
}

} } }
