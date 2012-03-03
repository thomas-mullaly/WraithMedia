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
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QTime>
#include <QTextStream>

#include <wraith/gui/SliderWidget.hpp>
#include <wraith/gui/visualizer/VisualizerWidget.hpp>
#include <wraith/gui/equalizer/EqualizerWidget.hpp>

#include <wraith/core/ServiceManager.hpp>
#include <wraith/core/playback/PlaybackEngineService.hpp>

#include "MainWindow.hpp"

using namespace wraith;
using namespace wraith::playback;
using namespace wraith::gui;
using namespace wraith::gui::equalizer;
using namespace wraith::gui::visualizer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , timer_display(new QLabel(tr("0:00 / 0:00")))
    , play_pause()
    , viewEqualizer()
    , trackbar(new SliderWidget(Qt::Horizontal))
    , visualizer()
    , equalizer()
    , ignore_tick(false)
    , playbackEngine(0)
    , timer(0)
{ setupUI(); }

MainWindow::~MainWindow() { }

void MainWindow::setupUI() {
    ServiceManager *manager = ServiceManager::instance();
    PlaybackEngineService *playbackService =
        static_cast<PlaybackEngineService*>(
            manager->queryForService(
                PlaybackEngineService::playbackEngineServiceID
            )
        );

    playbackEngine = playbackService->defaultEngine();

    QFile style(":/styles/slider.qss");
    if(!style.open(QIODevice::ReadOnly))
        return;
        
    QTextStream stream(&style);
    trackbar->setStyleSheet(stream.readAll());
    trackbar->setTracking(false);

    QVBoxLayout *vbox = new QVBoxLayout();

    IVisualizationSource *visSource = dynamic_cast<IVisualizationSource*>(playbackEngine);
    if(visSource) {
        visualizer = new VisualizerWidget(visSource, this);
        visualizer->setMinimumSize(500,500);
        vbox->addWidget(visualizer);
    }
    vbox->addWidget(trackbar);

    play_pause = new QPushButton("Play", this);
    play_pause->setMaximumSize(60,20);
    vbox->addWidget(play_pause);
    connect(play_pause, SIGNAL(clicked(bool)), this, SLOT(onPlayPauseClicked(bool)));

    IEqualizer *eq = dynamic_cast<IEqualizer*>(playbackEngine);
    if(eq) {
        equalizer = new EqualizerWidget(eq, this);
        equalizer->setVisible(false);
        equalizer->setMinimumSize(800,200);
        equalizer->setMaximumSize(800,200);
        vbox->addWidget(equalizer);
    }

    QWidget *main = new QWidget(this);
    main->setLayout(vbox);
    main->setVisible(true);
    setCentralWidget(main);

    playbackEngine->registerEventsCallback(this);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onMediaTick()));

    connect(trackbar, SIGNAL(userChangedValue(int)), this, SLOT(onSliderSeek(int)));
    connect(trackbar, SIGNAL(sliderMoved(int)), this, SLOT(onSliderSeek(int)));
    connect(trackbar, SIGNAL(sliderPressed()), this, SLOT(onSliderPressed()));
    connect(trackbar, SIGNAL(sliderReleased()), this, SLOT(onSliderReleased()));
    statusBar()->addWidget(timer_display);
    setupMenus();
}

void MainWindow::setupMenus() {
    // Setup the file menu.
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("Open Media File"), this, SLOT(onOpenMediaFile()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("E&xit"), this, SLOT(close()));

    // Setup view menu.
    QMenu *viewMenu = menuBar()->addMenu(tr("View"));
    viewEqualizer = viewMenu->addAction(tr("Show Equalizer"), this, SLOT(onShowHideEqualizer()));
}

void MainWindow::onStateChanged(PlaybackState newState, PlaybackState oldState) {
    if(newState == PLAYBACK_STATE_PLAYING) {
        statusBar()->showMessage(tr("Playback Started"), 1000);
        play_pause->setText(tr("Pause"));
        timer->start();
    } else if(newState == PLAYBACK_STATE_PAUSED) {
        play_pause->setText(tr("Play"));
        statusBar()->showMessage(tr("Playback Paused"), 1000);
        timer->stop();
    }
}

void MainWindow::onPlaybackComplete() {    
    statusBar()->showMessage(tr("Playback Complete"), 5000);
}

void MainWindow::onOpenMediaFile() {
    // Open a media file.
    QString file = QFileDialog::getOpenFileName(this, tr("Open Media File"), tr(""),
                                                tr("Media Files (*.mp3)"));

    if(!file.isNull()) {
        // Prepend 'file://' to the filepath to make it a legit uri.
        file.prepend("file://");
        playbackEngine->openMedia(file.toStdString());
        playbackEngine->play();

        trackbar->setEnabled(true);
    }
}

void MainWindow::onMediaTick() {    
    boost::int64_t duration = playbackEngine->duration();
    boost::int64_t position = playbackEngine->position();
    
    if(!ignore_tick) {
        trackbar->setMaximum(duration);
        trackbar->setValue(position);
    }
    
    QString text(computeTimeString(position) + " / " + computeTimeString(duration));
    timer_display->setText(text);
}

void MainWindow::onSliderSeek(int value) {
   playbackEngine->setPosition(value);
}

void MainWindow::onPlayPauseClicked(bool checked) {
    playbackEngine->togglePlayPause();
}

void MainWindow::onShowHideEqualizer() {
    if(equalizer->isVisible()) {
        viewEqualizer->setText(tr("Show Equalizer"));
        equalizer->setVisible(false);
    } else {
        viewEqualizer->setText(tr("Hide Equalizer"));
        equalizer->setVisible(true);
    }
}

QString MainWindow::computeTimeString(qint64 value) {
    if(value < 1000l) {
        return QString(tr("0:00"));
    }
    
    QTime time(0, 0, 0);
    qint64 seconds = value / 1000;
    qint64 mins = seconds / 60;
    seconds = seconds - (mins * 60);
    time.setHMS(0, mins, seconds);
    return time.toString("m:ss");
}

void MainWindow::onSliderPressed() {
    ignore_tick = true;
}

void MainWindow::onSliderReleased() {
    ignore_tick = false;
}
