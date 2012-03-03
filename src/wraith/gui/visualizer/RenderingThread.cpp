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
#include <stdexcept>

#include <wraith/gui/visualizer/WraithPlaybackInfo.hpp>

#include "RenderingThread.hpp"

namespace wraith { namespace gui { namespace visualizer {

RenderingThread::RenderingThread(QObject *parent)
    : QThread(parent)
    , m_visualizerWidget()
    , m_setup(false)
    , m_dataQueue()
    , m_dataQueueMutex()
    , m_stopRendering(false)
    , m_stopRenderingMutex()
    , m_playbackInfo()
{ }

RenderingThread::~RenderingThread()
{ }

void RenderingThread::setup(VisualizerWidget *visualizerWidget) {
    if(!visualizerWidget)
        throw std::invalid_argument("\"visualizerWidget\" cannot be null");

    m_visualizerWidget = visualizerWidget;

    connect(m_visualizerWidget, SIGNAL(draw), this, SLOT(onDraw));

    m_playbackInfo.reset(new WraithPlaybackInfo());

    m_stopRenderingMutex.lock();
    m_stopRendering = false;
    m_stopRenderingMutex.unlock();

    m_setup = true;
}

void RenderingThread::stopRendering() {
    m_stopRenderingMutex.lock();
    m_stopRendering = true;
    m_stopRenderingMutex.unlock();
}

bool RenderingThread::isRendering() {
    return !m_stopRendering;
}

visualization::IPlaybackInfo::PlaybackInfoPtr
RenderingThread::playbackInfo() {
    return m_playbackInfo;
}

void RenderingThread::render() {
}

int RenderingThread::height() {
    return m_visualizerWidget->height();
}

int RenderingThread::width() {
    return m_visualizerWidget->width();
}

visualization::IRenderer* RenderingThread::renderer() {
    return 0;
}

void RenderingThread::setRenderer(visualization::IRenderer *renderer) {
}

void RenderingThread::onDraw() {
    render();
}

void RenderingThread::run() {
    while(true) {
        m_stopRenderingMutex.lock();
        if(m_stopRendering) {
            m_stopRenderingMutex.unlock();
            break;
        }
        m_stopRenderingMutex.unlock();

    }
}

} } } // namespace wraith::gui::visualizer
