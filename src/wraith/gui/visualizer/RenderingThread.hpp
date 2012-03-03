/*
 * Copyright 2011 Thomas Mullaly
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
#ifndef WRAITH_GUI_VISUALIZER_RENDERING_THREAD_HPP
#define WRAITH_GUI_VISUALIZER_RENDERING_THREAD_HPP

#include <queue>

#include <QThread>
#include <QTime>

#include <wraith/core/visualization/IRenderingController.hpp>

#include <wraith/gui/visualizer/VisualizerWidget.hpp>

namespace wraith { namespace gui { namespace visualizer {

struct DataSlice {
    visualization::VisualizationData::VisualizationDataPtr data;
    QTime sliceTime;
};

class RenderingThread : public QThread, public visualization::IRenderingController {
public:
    RenderingThread(QObject *parent = 0);
    virtual ~RenderingThread();

    void setup(VisualizerWidget *visualizerWidget);

    void stopRendering();
    bool isRendering();

    virtual visualization::IPlaybackInfo::PlaybackInfoPtr playbackInfo();

    virtual void render();

    virtual int height();
    virtual int width();

    virtual visualization::IRenderer*   renderer();
    virtual void                        setRenderer(visualization::IRenderer *renderer);

private slots:
    void onDraw();

protected:
    virtual void run();

private:
    typedef std::queue<DataSlice> DataQueue;

    VisualizerWidget*   m_visualizerWidget;
    bool                m_setup;
    DataQueue           m_dataQueue;
    QMutex              m_dataQueueMutex;
    bool                m_stopRendering;
    QMutex              m_stopRenderingMutex;
    visualization::IPlaybackInfo::PlaybackInfoPtr m_playbackInfo;
};

} } } // namespace wraith::gui::visualizer

#endif
