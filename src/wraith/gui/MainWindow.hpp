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
#ifndef WRAITH_GUI_MAIN_WINDOW_HPP
#define WRAITH_GUI_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QTimer>

#include <wraith/core/playback/PlaybackEngine.hpp>

namespace wraith { namespace gui {

// Forward declarations.
class SliderWidget;
namespace visualizer {
    class VisualizerWidget;
}

namespace equalizer {
    class EqualizerWidget;
}

class MainWindow : public QMainWindow, public playback::IPlaybackEventsCallback {
Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void setupUI();
    void setupMenus();

    virtual void onPlaybackComplete();
    virtual void onStateChanged(wraith::playback::PlaybackState newState, wraith::playback::PlaybackState oldState);

private slots:
    /**
     * Called when the "Open Media File" menu item is clicked.
     */
    void onOpenMediaFile();

    void onMediaTick();
    void onSliderSeek(int newValue);
    void onSliderPressed();
    void onSliderReleased();
    void onPlayPauseClicked(bool checked);
    void onShowHideEqualizer();
    
private:
    QString computeTimeString(qint64 value);

private:
    QLabel *timer_display;
    QPushButton *play_pause;
    QAction *viewEqualizer;
    SliderWidget *trackbar;
    visualizer::VisualizerWidget *visualizer;
    equalizer::EqualizerWidget *equalizer;
    playback::PlaybackEngine *playbackEngine;
    bool ignore_tick;
    QTimer *timer;
};

} } // namespace wraith::gui

#endif
